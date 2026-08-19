#include "csg.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "material.h"

namespace
{

  const f32 CSG_EPS = 0.0005f;      // plane comparison tolerance
  const f32 CSG_MERGE = 0.0001f;    // vertex welding tolerance
  const f32 CSG_MERGE2 = CSG_MERGE * CSG_MERGE;
  const f32 CSG_SLIVER = 1e-8f;     // degenerate area threshold (Newell length squared)
  const f32 CSG_FAR = 1e7f;         // far end for parity rays

} // namespace

// One convex face of the boundary soup, oriented outward from the solid.
struct CSGFace
{
  core::array<vector3df> verts; // convex polygon, wound outward
  vector3df normal;             // unit outward normal
  f32 d;                        // plane: normal dot x = d
  vector3df origin;             // a point on the plane (verts[0])
  vector3df u, v;               // orthonormal basis spanning the plane
  aabbox3df box;
  ITexture *texture;            // surface texture (shared per primitive), NULL for default
  f32 uoffset;                  // texture U offset in texels
  f32 voffset;                  // texture V offset in texels
  f32 rotation;                 // texture rotation in degrees
  f32 uscale;                   // 1 = 1 texel per world unit
  f32 vscale;
};

struct CSGBuilder
{
  core::array<CSGFace> fills;  // faces bounding added solids (CSG_ADD)
  core::array<CSGFace> carves; // faces bounding carved cavities (CSG_SUBTRACT)
};

namespace
{

  // Build the (u,v,n) basis used for planar point-in-polygon tests.
  void InitBasis(CSGFace &f)
  {
    const vector3df &n = f.normal;
    vector3df axis(1.f, 0.f, 0.f);
    f32 best = fabsf(n.X);
    if (fabsf(n.Y) < best)
    {
      axis.set(0.f, 1.f, 0.f);
      best = fabsf(n.Y);
    }
    if (fabsf(n.Z) < best)
      axis.set(0.f, 0.f, 1.f);
    f.u = axis.crossProduct(n);
    f.u.normalize();
    f.v = n.crossProduct(f.u);
    f.origin = f.verts[0];
  }

  // Newell normal of a polygon (length is proportional to the area).
  vector3df NewellNormal(const core::array<vector3df> &p)
  {
    vector3df n(0.f, 0.f, 0.f);
    const int np = (int)p.size();
    for (int i = 0; i < np; ++i)
    {
      const vector3df &a = p[i];
      const vector3df &b = p[(i + 1) % np];
      n.X += (a.Y - b.Y) * (a.Z + b.Z);
      n.Y += (a.Z - b.Z) * (a.X + b.X);
      n.Z += (a.X - b.X) * (a.Y + b.Y);
    }
    return n;
  }

  // Welds near-coincident consecutive vertices and builds a validated face.
  // Returns a face with an empty vertex list if the polygon is degenerate.
  CSGFace MakeFace(const core::array<vector3df> &in, const vector3df &parentNormal,
                   ITexture *texture = NULL, f32 uoffset = 0.f, f32 voffset = 0.f,
                   f32 rotation = 0.f, f32 uscale = 1.f, f32 vscale = 1.f)
  {
    CSGFace f;
    f.texture = texture;
    f.uoffset = uoffset;
    f.voffset = voffset;
    f.rotation = rotation;
    f.uscale = uscale;
    f.vscale = vscale;
    core::array<vector3df> pts;
    for (u32 i = 0; i < in.size(); ++i)
    {
      const vector3df &v = in[i];
      if (!pts.empty() && (v - pts.getLast()).getLengthSQ() < CSG_MERGE2)
        continue;
      pts.push_back(v);
    }
    while (pts.size() > 1 && (pts[0] - pts.getLast()).getLengthSQ() < CSG_MERGE2)
      pts.set_used(pts.size() - 1);
    if (pts.size() < 3)
      return f;
    vector3df nm = NewellNormal(pts);
    const f32 len = nm.getLength();
    if (len * len < CSG_SLIVER)
      return f;
    nm /= len;
    if (nm.dotProduct(parentNormal) < 0.f)
      nm = -nm;
    f.verts = pts;
    f.normal = nm;
    f.d = nm.dotProduct(f.verts[0]);
    f.box.reset(f.verts[0]);
    for (u32 i = 1; i < f.verts.size(); ++i)
      f.box.addInternalPoint(f.verts[i]);
    InitBasis(f);
    return f;
  }

  bool SameNormal(const vector3df &a, const vector3df &b)
  {
    return a.dotProduct(b) > 1.f - CSG_EPS;
  }

  // Clips a convex polygon by a plane, keeping the side where dist >= -CSG_EPS
  // (half-open toward the positive side) or dist <= +CSG_EPS.
  void ClipKeep(const core::array<vector3df> &in, const vector3df &n, f32 d,
                bool positive, core::array<vector3df> &out)
  {
    const int np = (int)in.size();
    for (int i = 0; i < np; ++i)
    {
      const vector3df &a = in[i];
      const vector3df &b = in[(i + 1) % np];
      const f32 da = n.dotProduct(a) - d;
      const f32 db = n.dotProduct(b) - d;
      const f32 la = positive ? da : -da;
      const f32 lb = positive ? db : -db;
      const bool ina = la >= -CSG_EPS;
      const bool inb = lb >= -CSG_EPS;
      if (inb)
      {
        if (ina)
        {
          out.push_back(b);
        }
        else
        {
          const f32 t = da / (da - db);
          out.push_back(a + (b - a) * t);
          out.push_back(b);
        }
      }
      else if (ina)
      {
        const f32 t = da / (da - db);
        out.push_back(a + (b - a) * t);
      }
    }
  }

  // Splits a convex polygon by a plane into front and back pieces. A fully
  // coplanar polygon is emitted to the front side only (half-open convention).
  void SplitPolyByPlane(const core::array<vector3df> &in, const vector3df &n, f32 d,
                        core::array<vector3df> &front, core::array<vector3df> &back)
  {
    f32 minD = 0.f, maxD = 0.f;
    for (u32 i = 0; i < in.size(); ++i)
    {
      const f32 dist = n.dotProduct(in[i]) - d;
      if (i == 0)
      {
        minD = maxD = dist;
      }
      else
      {
        if (dist < minD) minD = dist;
        if (dist > maxD) maxD = dist;
      }
    }
    if (minD >= -CSG_EPS && maxD <= CSG_EPS)
    {
      front = in;
      return;
    }
    ClipKeep(in, n, d, true, front);
    ClipKeep(in, n, d, false, back);
  }

  // Splits the cells of a list by a plane, replacing them with the convex pieces.
  void SplitCellsByPlane(core::array<CSGFace> &cells, const CSGFace &plane)
  {
    core::array<CSGFace> next;
    for (u32 i = 0; i < cells.size(); ++i)
    {
      const CSGFace &c = cells[i];
      if (c.verts.empty())
        continue;
      if (!c.box.intersectsWithBox(plane.box))
      {
        next.push_back(c);
        continue;
      }
      core::array<vector3df> front, back;
      SplitPolyByPlane(c.verts, plane.normal, plane.d, front, back);
      CSGFace ff = MakeFace(front, c.normal, c.texture, c.uoffset, c.voffset, c.rotation,
                            c.uscale, c.vscale);
      if (!ff.verts.empty())
        next.push_back(ff);
      CSGFace fb = MakeFace(back, c.normal, c.texture, c.uoffset, c.voffset, c.rotation,
                            c.uscale, c.vscale);
      if (!fb.verts.empty())
        next.push_back(fb);
    }
    cells = next;
  }

  // Clips existing faces against a primitive volume, keeping everything outside
  // (including flush coplanar pieces per the half-open convention).
  void ClipFacesByPrim(const core::array<CSGFace> &src, const core::array<CSGFace> &prim,
                       core::array<CSGFace> &dst)
  {
    for (u32 i = 0; i < src.size(); ++i)
    {
      CSGFace cell = src[i];
      core::array<CSGFace> cells;
      cells.push_back(cell);
      for (u32 j = 0; j < prim.size(); ++j)
        SplitCellsByPlane(cells, prim[j]);
      for (u32 k = 0; k < cells.size(); ++k)
        if (!cells[k].verts.empty())
          dst.push_back(cells[k]);
    }
  }

  // Reverses a face so its normal points the other way across the same plane.
  CSGFace FlipFace(const CSGFace &f)
  {
    CSGFace r;
    r.verts = f.verts;
    for (u32 i = 0; i < r.verts.size() / 2; ++i)
    {
      const u32 j = r.verts.size() - 1 - i;
      const vector3df tmp = r.verts[i];
      r.verts[i] = r.verts[j];
      r.verts[j] = tmp;
    }
    r.normal = -f.normal;
    r.d = r.normal.dotProduct(r.verts[0]);
    r.origin = f.origin;
    r.u = f.u;
    r.v = f.v;
    r.box = f.box;
    r.texture = f.texture;
    r.uoffset = f.uoffset;
    r.voffset = f.voffset;
    r.rotation = f.rotation;
    r.uscale = f.uscale;
    r.vscale = f.vscale;
    return r;
  }

  // Planar point-in-convex-polygon test (all edge cross products share a sign).
  bool PointInConvex2D(const CSGFace &f, const vector3df &p)
  {
    const vector3df delta = p - f.origin;
    const f32 pu = delta.dotProduct(f.u);
    const f32 pv = delta.dotProduct(f.v);
    int sign = 0;
    const u32 n = f.verts.size();
    for (u32 i = 0; i < n; ++i)
    {
      const vector3df a = f.verts[i] - f.origin;
      const vector3df b = f.verts[(i + 1) % n] - f.origin;
      const f32 au = a.dotProduct(f.u);
      const f32 av = a.dotProduct(f.v);
      const f32 bu = b.dotProduct(f.u);
      const f32 bv = b.dotProduct(f.v);
      const f32 cross = (bu - au) * (pv - av) - (bv - av) * (pu - au);
      if (cross > CSG_EPS)
      {
        if (sign < 0)
          return false;
        sign = 1;
      }
      else if (cross < -CSG_EPS)
      {
        if (sign > 0)
          return false;
        sign = -1;
      }
    }
    return true;
  }

  const vector3df &RayDir()
  {
    static const vector3df dir = (vector3df(1.f, 0.7f, 0.5f)).normalize();
    return dir;
  }

  // Removes carve faces that lie flush against a primitive face inside the
  // primitive's footprint. Two cavities sharing a coplanar surface merge into
  // one void, so the shared surface must vanish (e.g. a hallway opening cut
  // through a room wall). Fragments outside the footprint are preserved.
  void PruneFlushCarve(core::array<CSGFace> &cells, const core::array<CSGFace> &prim)
  {
    core::array<CSGFace> next;
    for (u32 i = 0; i < cells.size(); ++i)
    {
      CSGFace &cell = cells[i];
      if (cell.verts.empty())
        continue;
      vector3df centroid(0.f, 0.f, 0.f);
      for (u32 k = 0; k < cell.verts.size(); ++k)
        centroid += cell.verts[k];
      centroid /= (f32)cell.verts.size();
      bool prune = false;
      for (u32 j = 0; j < prim.size() && !prune; ++j)
      {
        const CSGFace &f = prim[j];
        const f32 dist = f.normal.dotProduct(centroid) - f.d;
        if (fabsf(dist) > CSG_EPS)
          continue;
        if (PointInConvex2D(f, centroid))
          prune = true;
      }
      if (!prune)
        next.push_back(cell);
    }
    cells = next;
  }

  // Parity test: is the point inside the closed surface set?
  bool InSetParity(const core::array<CSGFace> &set, const vector3df &p)
  {
    const vector3df dir = RayDir();
    u32 cross = 0;
    for (u32 i = 0; i < set.size(); ++i)
    {
      const CSGFace &f = set[i];
      const f32 denom = f.normal.dotProduct(dir);
      if (fabsf(denom) < CSG_EPS)
        continue; // ray parallel to the face plane
      const f32 t = (f.d - f.normal.dotProduct(p)) / denom;
      if (t <= CSG_EPS || t >= CSG_FAR)
        continue;
      const vector3df ip = p + dir * t;
      if (PointInConvex2D(f, ip))
        ++cross;
    }
    return (cross % 2) != 0;
  }

  // Is the point inside the level solid? Carved rooms bound void, fills bound
  // solid; at infinity the world is solid.
  bool Solid(const CSGBuilder *b, const vector3df &p)
  {
    return InSetParity(b->fills, p) || !InSetParity(b->carves, p);
  }

  // Drops primitive fragments that lie flush against an existing face (the
  // existing surface covers the area, avoiding a double wall).
  bool FlushAbsorbed(const CSGFace &cell, const core::array<CSGFace> &soup, const vector3df &centroid)
  {
    for (u32 i = 0; i < soup.size(); ++i)
    {
      const CSGFace &f = soup[i];
      aabbox3df box = f.box;
      box.MinEdge -= CSG_EPS;
      box.MaxEdge += CSG_EPS;
      if (!box.isPointInside(centroid))
        continue;
      const f32 dist = f.normal.dotProduct(centroid) - f.d;
      if (fabsf(dist) <= CSG_EPS && PointInConvex2D(f, centroid))
        return true;
    }
    return false;
  }

  struct Group
  {
    vector3df normal;
    f32 d;
    core::array<vector3df> tris; // flattened triangle soup
  };

  struct E
  {
    vector3df a, b;
    bool interior;
    bool used;
  };

  void ExtractFaces(IMesh *mesh, core::array<CSGFace> &out)
  {
    const u32 mbCount = mesh->getMeshBufferCount();
    for (u32 mi = 0; mi < mbCount; ++mi)
    {
      IMeshBuffer *buf = mesh->getMeshBuffer(mi);
      if (buf->getVertexType() != EVT_STANDARD || buf->getIndexType() != EIT_16BIT)
        continue;
      const S3DVertex *verts = (const S3DVertex *)buf->getVertices();
      const u16 *indices = buf->getIndices();
      const u32 triCount = buf->getIndexCount() / 3;
      core::array<Group> groups;
      for (u32 t = 0; t < triCount; ++t)
      {
        const u16 ia = indices[t * 3 + 0];
        const u16 ib = indices[t * 3 + 1];
        const u16 ic = indices[t * 3 + 2];
        const vector3df &pa = verts[ia].Pos;
        const vector3df &pb = verts[ib].Pos;
        const vector3df &pc = verts[ic].Pos;
        vector3df n = (pb - pa).crossProduct(pc - pa);
        const f32 len = n.getLength();
        if (len < CSG_EPS)
          continue;
        n /= len;
        const f32 d = n.dotProduct(pa);
        int gi = -1;
        for (u32 g = 0; g < groups.size(); ++g)
        {
          if (SameNormal(groups[g].normal, n) && fabsf(groups[g].d - d) < CSG_EPS)
          {
            gi = (int)g;
            break;
          }
        }
        if (gi < 0)
        {
          Group ng;
          ng.normal = n;
          ng.d = d;
          groups.push_back(ng);
          gi = (int)groups.size() - 1;
        }
        groups[gi].tris.push_back(pa);
        groups[gi].tris.push_back(pb);
        groups[gi].tris.push_back(pc);
      }
      for (u32 g = 0; g < groups.size(); ++g)
      {
        // Collect oriented edges and find interior (shared) edges.
        core::array<E> edges;
        for (u32 t = 0; t < groups[g].tris.size(); t += 3)
        {
          const vector3df *v = &groups[g].tris[t];
          for (int e = 0; e < 3; ++e)
          {
            E ed;
            ed.a = v[e];
            ed.b = v[(e + 1) % 3];
            ed.interior = false;
            ed.used = false;
            edges.push_back(ed);
          }
        }
        for (u32 i = 0; i < edges.size(); ++i)
        {
          for (u32 j = 0; j < edges.size(); ++j)
          {
            if (i == j)
              continue;
            if ((edges[j].a - edges[i].b).getLengthSQ() < CSG_MERGE2 &&
                (edges[j].b - edges[i].a).getLengthSQ() < CSG_MERGE2)
            {
              edges[i].interior = true;
              edges[j].interior = true;
            }
            else if ((edges[j].a - edges[i].a).getLengthSQ() < CSG_MERGE2 &&
                     (edges[j].b - edges[i].b).getLengthSQ() < CSG_MERGE2)
            {
              edges[i].interior = true;
              edges[j].interior = true;
            }
          }
        }
        // Stitch the boundary into a single loop.
        core::array<vector3df> loop;
        int start = -1;
        for (u32 i = 0; i < edges.size(); ++i)
          if (!edges[i].interior)
          {
            start = (int)i;
            break;
          }
        if (start >= 0)
        {
          loop.push_back(edges[start].a);
          loop.push_back(edges[start].b);
          edges[start].used = true;
          vector3df cur = edges[start].b;
          for (u32 guard = 0; guard < edges.size() && !loop.empty(); ++guard)
          {
            int found = -1;
            for (u32 j = 0; j < edges.size(); ++j)
            {
              if (edges[j].used || edges[j].interior)
                continue;
              if ((edges[j].a - cur).getLengthSQ() < CSG_MERGE2)
              {
                found = (int)j;
                break;
              }
            }
            if (found < 0)
              break;
            loop.push_back(edges[found].b);
            edges[found].used = true;
            cur = edges[found].b;
          }
          if (loop.size() > 1 && (loop.getLast() - loop[0]).getLengthSQ() < CSG_MERGE2)
            loop.set_used(loop.size() - 1);
        }
        if (loop.size() >= 3)
        {
          CSGFace f = MakeFace(loop, groups[g].normal);
          if (!f.verts.empty())
            out.push_back(f);
        }
      }
    }
  }

  // Classic Quake axial projection: pick the world axis plane closest to the face,
  // then rotate, scale and shift the (U,V) basis. Returns the two mapping vectors so
  // u = dot(p, vecs[0]) + vecs[0][3], v = dot(p, vecs[1]) + vecs[1][3] (texels).
  void TextureAxes(const CSGFace &f, vector3df vecs[2])
  {
    static const vector3df baseaxis[18] = {
      vector3df(0.f, 0.f, 1.f), vector3df(1.f, 0.f, 0.f), vector3df(0.f, -1.f, 0.f),  // floor
      vector3df(0.f, 0.f, -1.f), vector3df(1.f, 0.f, 0.f), vector3df(0.f, -1.f, 0.f), // ceiling
      vector3df(1.f, 0.f, 0.f), vector3df(0.f, 1.f, 0.f), vector3df(0.f, 0.f, -1.f),  // west wall
      vector3df(-1.f, 0.f, 0.f), vector3df(0.f, 1.f, 0.f), vector3df(0.f, 0.f, -1.f), // east wall
      vector3df(0.f, 1.f, 0.f), vector3df(1.f, 0.f, 0.f), vector3df(0.f, 0.f, -1.f),  // south wall
      vector3df(0.f, -1.f, 0.f), vector3df(1.f, 0.f, 0.f), vector3df(0.f, 0.f, -1.f)  // north wall
    };
    f32 best = -1.f;
    int bestaxis = 0;
    for (int i = 0; i < 6; ++i)
    {
      const f32 dist = f.normal.dotProduct(baseaxis[i * 3]);
      const f32 adist = fabsf(dist);
      if (adist > best)
      {
        best = adist;
        bestaxis = i;
      }
    }
    vector3df u = baseaxis[bestaxis * 3 + 1];
    vector3df v = baseaxis[bestaxis * 3 + 2];
    const f32 r = f.rotation * core::DEGTORAD;
    const f32 s = sinf(r);
    const f32 c = cosf(r);
    vector3df tu = u * c + v * s;
    vector3df tv = u * -s + v * c;
    const f32 us = f.uscale != 0.f ? 1.f / f.uscale : 1.f;
    const f32 vs = f.vscale != 0.f ? 1.f / f.vscale : 1.f;
    vecs[0] = tu * us;
    vecs[1] = tv * vs;
  }

  matrix4 Compose(const vector3df &pre, const vector3df &scale, const vector3df &rot, const vector3df &pos)
  {
    matrix4 m;
    m.setScale(scale);
    matrix4 p;
    p.setTranslation(pre);
    m = m * p;
    matrix4 r;
    r.setRotationDegrees(rot);
    m = r * m;
    matrix4 t;
    t.setTranslation(pos);
    m = t * m;
    return m;
  }

  void TransformFaces(core::array<CSGFace> &faces, const matrix4 &m, const matrix4 &rot)
  {
    for (u32 i = 0; i < faces.size(); ++i)
    {
      CSGFace &f = faces[i];
      for (u32 j = 0; j < f.verts.size(); ++j)
        m.transformVect(f.verts[j]);
      vector3df ref = f.normal;
      rot.rotateVect(ref);
      vector3df nm = NewellNormal(f.verts);
      const f32 len = nm.getLength();
      if (len < CSG_EPS)
        continue;
      nm /= len;
      if (nm.dotProduct(ref) < 0.f)
        nm = -nm;
      f.normal = nm;
      f.d = nm.dotProduct(f.verts[0]);
      f.box.reset(f.verts[0]);
      for (u32 j = 1; j < f.verts.size(); ++j)
        f.box.addInternalPoint(f.verts[j]);
      f.origin = f.verts[0];
    }
  }

  void BuildWedge(core::array<CSGFace> &faces)
  {
    // Unit wedge, half extents; scaled by the caller.
    const f32 hw = 0.5f, hh = 0.5f, hd = 0.5f;
    const vector3df A(-hw, -hh, -hd); // back-bottom-left
    const vector3df B(hw, -hh, -hd);  // back-bottom-right
    const vector3df C(-hw, hh, -hd);  // back-top-left
    const vector3df D(hw, hh, -hd);   // back-top-right
    const vector3df E(-hw, -hh, hd);  // front-bottom-left
    const vector3df F(hw, -hh, hd);   // front-bottom-right
    const vector3df ny(0.f, -1.f, 0.f);
    const vector3df nz(0.f, 0.f, -1.f);
    const vector3df mx(-1.f, 0.f, 0.f);
    const vector3df px(1.f, 0.f, 0.f);
    vector3df slope(0.f, 1.f, 1.f);
    slope.normalize();
    core::array<vector3df> poly;

    poly.push_back(A); poly.push_back(B); poly.push_back(F); poly.push_back(E); // bottom
    faces.push_back(MakeFace(poly, ny));
    poly.set_used(0);
    poly.push_back(D); poly.push_back(B); poly.push_back(A); poly.push_back(C); // back
    faces.push_back(MakeFace(poly, nz));
    poly.set_used(0);
    poly.push_back(C); poly.push_back(E); poly.push_back(F); poly.push_back(D); // slope
    faces.push_back(MakeFace(poly, slope));
    poly.set_used(0);
    poly.push_back(C); poly.push_back(A); poly.push_back(E); // left
    faces.push_back(MakeFace(poly, mx));
    poly.set_used(0);
    poly.push_back(D); poly.push_back(F); poly.push_back(B); // right
    faces.push_back(MakeFace(poly, px));
  }

  void ApplyOp(CSGBuilder *b, const core::array<CSGFace> &prim, int op)
  {
    // Existing faces keep the fragments outside the primitive volume.
    core::array<CSGFace> newFills;
    core::array<CSGFace> newCarves;
    ClipFacesByPrim(b->fills, prim, newFills);
    ClipFacesByPrim(b->carves, prim, newCarves);

    // Primitive faces are split against the current level boundary, classified
    // by centroid and kept (re-oriented for a carve) per the operation.
    core::array<CSGFace> soup;
    for (u32 i = 0; i < b->fills.size(); ++i)
      soup.push_back(b->fills[i]);
    for (u32 i = 0; i < b->carves.size(); ++i)
      soup.push_back(b->carves[i]);

    core::array<CSGFace> added;
    for (u32 p = 0; p < prim.size(); ++p)
    {
      core::array<CSGFace> cells;
      cells.push_back(prim[p]);
      for (u32 s = 0; s < soup.size(); ++s)
        SplitCellsByPlane(cells, soup[s]);
      for (u32 c = 0; c < cells.size(); ++c)
      {
        const CSGFace &cell = cells[c];
        if (cell.verts.empty())
          continue;
        vector3df centroid(0.f, 0.f, 0.f);
        for (u32 k = 0; k < cell.verts.size(); ++k)
          centroid += cell.verts[k];
        centroid /= (f32)cell.verts.size();
        if (FlushAbsorbed(cell, soup, centroid))
          continue;
        const bool solid = Solid(b, centroid);
        if (op == CSG_ADD)
        {
          if (!solid)
            added.push_back(cell);
        }
        else
        {
          if (solid)
            added.push_back(FlipFace(cell));
        }
      }
    }

    if (op == CSG_ADD)
    {
      b->fills = newFills;
      for (u32 i = 0; i < added.size(); ++i)
        b->fills.push_back(added[i]);
    }
    else
    {
      PruneFlushCarve(newCarves, prim);
      b->carves = newCarves;
      for (u32 i = 0; i < added.size(); ++i)
        b->carves.push_back(added[i]);
    }
  }

  void AddBox(CSGBuilder *b, int op, ITexture *texture, f32 x, f32 y, f32 z,
              f32 pitch, f32 yaw, f32 roll, f32 w, f32 h, f32 d,
              f32 uoffset, f32 voffset, f32 rotation, f32 uscale, f32 vscale)
  {
    IMesh *mesh = _Device()->getSceneManager()->getGeometryCreator()->createCubeMesh(
        vector3df(1.f, 1.f, 1.f), ECMT_1BUF_24VTX_NP);
    if (!mesh)
      return;
    core::array<CSGFace> faces;
    ExtractFaces(mesh, faces);
    mesh->drop();
    if (faces.empty())
      return;
    for (u32 i = 0; i < faces.size(); ++i)
    {
      faces[i].texture = texture;
      faces[i].uoffset = uoffset;
      faces[i].voffset = voffset;
      faces[i].rotation = rotation;
      faces[i].uscale = uscale;
      faces[i].vscale = vscale;
    }
    matrix4 rot;
    rot.setRotationDegrees(vector3df(pitch, yaw, roll));
    matrix4 m = Compose(vector3df(0.f, 0.f, 0.f), vector3df(w, h, d), vector3df(pitch, yaw, roll),
                        vector3df(x, y, z));
    TransformFaces(faces, m, rot);
    ApplyOp(b, faces, op);
  }

  void AddCylinder(CSGBuilder *b, int op, ITexture *texture, int segments,
                   f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, f32 radius, f32 height,
                   f32 uoffset, f32 voffset, f32 rotation, f32 uscale, f32 vscale)
  {
    if (segments < 3)
      segments = 3;
    IMesh *mesh = _Device()->getSceneManager()->getGeometryCreator()->createCylinderMesh(
        0.5f, 1.f, (u32)segments);
    if (!mesh)
      return;
    core::array<CSGFace> faces;
    ExtractFaces(mesh, faces);
    mesh->drop();
    if (faces.empty())
      return;
    for (u32 i = 0; i < faces.size(); ++i)
    {
      faces[i].texture = texture;
      faces[i].uoffset = uoffset;
      faces[i].voffset = voffset;
      faces[i].rotation = rotation;
      faces[i].uscale = uscale;
      faces[i].vscale = vscale;
    }
    matrix4 rot;
    rot.setRotationDegrees(vector3df(pitch, yaw, roll));
    const f32 f = radius / 0.5f;
    matrix4 m = Compose(vector3df(0.f, -0.5f, 0.f), vector3df(f, height, f),
                        vector3df(pitch, yaw, roll), vector3df(x, y, z));
    TransformFaces(faces, m, rot);
    ApplyOp(b, faces, op);
  }

  void AddCone(CSGBuilder *b, int op, ITexture *texture, int segments,
               f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, f32 radius, f32 height,
               f32 uoffset, f32 voffset, f32 rotation, f32 uscale, f32 vscale)
  {
    if (segments < 3)
      segments = 3;
    IMesh *mesh = _Device()->getSceneManager()->getGeometryCreator()->createConeMesh(
        0.5f, 1.f, (u32)segments);
    if (!mesh)
      return;
    core::array<CSGFace> faces;
    ExtractFaces(mesh, faces);
    mesh->drop();
    if (faces.empty())
      return;
    for (u32 i = 0; i < faces.size(); ++i)
    {
      faces[i].texture = texture;
      faces[i].uoffset = uoffset;
      faces[i].voffset = voffset;
      faces[i].rotation = rotation;
      faces[i].uscale = uscale;
      faces[i].vscale = vscale;
    }
    matrix4 rot;
    rot.setRotationDegrees(vector3df(pitch, yaw, roll));
    const f32 f = radius / 0.5f;
    matrix4 m = Compose(vector3df(0.f, -0.5f, 0.f), vector3df(f, height, f),
                        vector3df(pitch, yaw, roll), vector3df(x, y, z));
    TransformFaces(faces, m, rot);
    ApplyOp(b, faces, op);
  }

  void AddWedge(CSGBuilder *b, int op, ITexture *texture, f32 x, f32 y, f32 z,
                f32 pitch, f32 yaw, f32 roll, f32 w, f32 h, f32 d,
                f32 uoffset, f32 voffset, f32 rotation, f32 uscale, f32 vscale)
  {
    core::array<CSGFace> faces;
    BuildWedge(faces);
    if (faces.empty())
      return;
    for (u32 i = 0; i < faces.size(); ++i)
    {
      faces[i].texture = texture;
      faces[i].uoffset = uoffset;
      faces[i].voffset = voffset;
      faces[i].rotation = rotation;
      faces[i].uscale = uscale;
      faces[i].vscale = vscale;
    }
    matrix4 rot;
    rot.setRotationDegrees(vector3df(pitch, yaw, roll));
    matrix4 m = Compose(vector3df(0.f, 0.f, 0.f), vector3df(w, h, d), vector3df(pitch, yaw, roll),
                        vector3df(x, y, z));
    TransformFaces(faces, m, rot);
    ApplyOp(b, faces, op);
  }

} // namespace

extern "C"
{

  CSGBuilder *CALL CreateCsg()
  {
    return new CSGBuilder();
  }

  void CALL FreeCsg(CSGBuilder *csg)
  {
    delete csg;
  }

  void CALL ClearCsg(CSGBuilder *csg)
  {
    csg->fills.clear();
    csg->carves.clear();
  }

  void CALL AddCsgBox(CSGBuilder *csg, int operation, ITexture *texture,
                      float x, float y, float z, float pitch, float yaw, float roll,
                      float width, float height, float depth,
                      float uoffset, float voffset, float tex_rotation,
                      float uscale, float vscale)
  {
    if (csg)
      AddBox(csg, operation, texture, x, y, z, pitch, yaw, roll, width, height, depth,
             uoffset, voffset, tex_rotation, uscale, vscale);
  }

  void CALL AddCsgCylinder(CSGBuilder *csg, int operation, ITexture *texture,
                           float x, float y, float z, float pitch, float yaw, float roll,
                           float radius, float height, int segments,
                           float uoffset, float voffset, float tex_rotation,
                           float uscale, float vscale)
  {
    if (csg)
      AddCylinder(csg, operation, texture, segments, x, y, z, pitch, yaw, roll,
                  radius, height, uoffset, voffset, tex_rotation, uscale, vscale);
  }

  void CALL AddCsgCone(CSGBuilder *csg, int operation, ITexture *texture,
                       float x, float y, float z, float pitch, float yaw, float roll,
                       float radius, float height, int segments,
                       float uoffset, float voffset, float tex_rotation,
                       float uscale, float vscale)
  {
    if (csg)
      AddCone(csg, operation, texture, segments, x, y, z, pitch, yaw, roll,
              radius, height, uoffset, voffset, tex_rotation, uscale, vscale);
  }

  void CALL AddCsgWedge(CSGBuilder *csg, int operation, ITexture *texture,
                        float x, float y, float z, float pitch, float yaw, float roll,
                        float width, float height, float depth,
                        float uoffset, float voffset, float tex_rotation,
                        float uscale, float vscale)
  {
    if (csg)
      AddWedge(csg, operation, texture, x, y, z, pitch, yaw, roll, width, height, depth,
               uoffset, voffset, tex_rotation, uscale, vscale);
  }

  IMesh *CALL CsgMesh(CSGBuilder *csg)
  {
    SMesh *mesh = new SMesh();
    if (csg)
    {
      core::array<SMeshBuffer *> buffers;
      core::array<ITexture *> bufferTextures;
      const u32 VMAX = 65000;
      for (u32 pass = 0; pass < 2; ++pass)
      {
        const core::array<CSGFace> *set = (pass == 0) ? &csg->fills : &csg->carves;
        for (u32 i = 0; i < set->size(); ++i)
        {
          const CSGFace &f = (*set)[i];
          if (f.verts.empty())
            continue;
          const u32 vc = f.verts.size();
          // Reuse a buffer with the same texture that still has room, else make a new one.
          SMeshBuffer *buffer = NULL;
          for (u32 bi = 0; bi < buffers.size(); ++bi)
          {
            if (bufferTextures[bi] == f.texture && buffers[bi]->Vertices.size() + vc <= VMAX)
            {
              buffer = buffers[bi];
              break;
            }
          }
          if (!buffer)
          {
            buffer = new SMeshBuffer();
            mesh->addMeshBuffer(buffer);
            _FixMaterial(&buffer->getMaterial(), true);
            if (f.texture)
              buffer->getMaterial().setTexture(0, f.texture);
            buffers.push_back(buffer);
            bufferTextures.push_back(f.texture);
          }
          const u16 base = (u16)buffer->Vertices.size();
          vector3df vecs[2];
          TextureAxes(f, vecs);
          for (u32 k = 0; k < vc; ++k)
          {
            const vector3df &p = f.verts[k];
            S3DVertex vtx;
            vtx.Pos = p;
            vtx.Normal = f.normal;
            vtx.Color = SColor(255, 255, 255, 255);
            vtx.TCoords = vector2df(p.dotProduct(vecs[0]) + f.uoffset,
                                    p.dotProduct(vecs[1]) + f.voffset);
            buffer->Vertices.push_back(vtx);
          }
          for (u32 k = 1; k + 1 < vc; ++k)
          {
            buffer->Indices.push_back(base);
            buffer->Indices.push_back((u16)(base + k));
            buffer->Indices.push_back((u16)(base + k + 1));
          }
        }
      }
      for (u32 i = 0; i < buffers.size(); ++i)
      {
        buffers[i]->recalculateBoundingBox();
        buffers[i]->drop();
      }
      mesh->recalculateBoundingBox();
      mesh->setDirty();
    }
    return mesh;
  }

} // extern "C"