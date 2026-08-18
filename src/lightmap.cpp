#include "lightmap.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "log.h"
#include <ITexture.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

namespace
{

  const f32 DEG2RAD = 0.017453292519943295f;
  const f32 LM_PAD = 1.0f;    // texel padding between triangles
  const f32 LM_BIAS_TEXELS = 2.0f; // shadow ray offset, in texels
  const f32 LM_EPS = 0.0001f; // degenerate test
  const f32 LM_PI = 3.141592653589793f;
  const f32 LM_INV_PI = 0.3183098861837907f;
  const u32 LM_NO_TRI = 0xFFFFFFFFu;
  const int LM_MAX_LEAF = 4;  // max triangles per BVH leaf

  struct LMLight
  {
    E_LIGHT_TYPE type;
    vector3df position;   // world
    vector3df direction;  // world, normalized
    SColorf diffuse;
    vector3df attenuation;
    f32 cosInner, cosOuter, falloff;
    bool castShadow;
  };

  struct LMTri
  {
    vector3df a, b, c; // world corners
    vector3df normal;  // world, normalized
    int plane;         // dominant axis of the normal
    int uMin, vMin, uMax, vMax; // texel rect (padded, inclusive)
    int atlasX, atlasY;         // position in the atlas
    SMeshBufferLightMap *dst;   // target buffer
    u32 firstVertex;            // first vertex of this triangle in dst
    vector3df centroid;         // world centroid (radiosity source)
    f32 area;                   // world area (radiosity source)
    SColorf diffuse;            // material diffuse color (reflectance base)
    video::ITexture *tex0;      // diffuse texture, may be NULL
    SColorf albedo;             // reflectance used for bounces
    core::array<SColorf> texels; // per-texel radiance (uMin..uMax x vMin..vMax)
  };

  struct LMNodePart
  {
    IMeshSceneNode *node;
    SMesh *newMesh;
    bool lightmapped;
  };

  struct LMNodeInfo
  {
    core::aabbox3df box;
    int n0, n1;       // child node indices, -1 for leaf
    int start, count; // leaf: triangle range in order
  };

  struct LMRef
  {
    u32 index;
    f32 centroid;
  };

  struct LMBvh
  {
    core::array<LMNodeInfo> nodes;
    core::array<u32> order;
    const core::array<LMTri> *tris;
  };

  inline f32 Axis(const vector3df &v, int a)
  {
    return a == 0 ? v.X : (a == 1 ? v.Y : v.Z);
  }

  inline void SetAxis(vector3df &v, int a, f32 x)
  {
    if (a == 0)
      v.X = x;
    else if (a == 1)
      v.Y = x;
    else
      v.Z = x;
  }

  inline void GetPlaneAxes(int plane, int &axisU, int &axisV, int &axisW)
  {
    if (plane == 0) { axisU = 1; axisV = 2; axisW = 0; } // normal ~X
    else if (plane == 1) { axisU = 0; axisV = 2; axisW = 1; } // normal ~Y
    else { axisU = 0; axisV = 1; axisW = 2; } // normal ~Z
  }

  inline void PlaneCoord(const vector3df &p, int axisU, int axisV, f32 &u, f32 &v)
  {
    u = Axis(p, axisU);
    v = Axis(p, axisV);
  }

  int CmpX(const void *pa, const void *pb)
  {
    const LMRef *a = (const LMRef *)pa;
    const LMRef *b = (const LMRef *)pb;
    return a->centroid < b->centroid ? -1 : (a->centroid > b->centroid ? 1 : 0);
  }

  int CmpY(const void *pa, const void *pb)
  {
    const LMRef *a = (const LMRef *)pa;
    const LMRef *b = (const LMRef *)pb;
    return a->centroid < b->centroid ? -1 : (a->centroid > b->centroid ? 1 : 0);
  }

  int CmpZ(const void *pa, const void *pb)
  {
    const LMRef *a = (const LMRef *)pa;
    const LMRef *b = (const LMRef *)pb;
    return a->centroid < b->centroid ? -1 : (a->centroid > b->centroid ? 1 : 0);
  }

  int BvhBuildIndex(LMBvh &bvh, core::array<LMRef> &pool, int begin, int end)
  {
    const int count = end - begin;
    LMNodeInfo node;
    node.start = begin;
    node.count = count;
    node.n0 = -1;
    node.n1 = -1;
    {
      const LMTri &t0 = (*bvh.tris)[bvh.order[begin]];
      aabbox3df box(t0.a, t0.a);
      for (int i = begin; i < end; ++i)
      {
        const LMTri &t = (*bvh.tris)[bvh.order[i]];
        box.addInternalPoint(t.a);
        box.addInternalPoint(t.b);
        box.addInternalPoint(t.c);
      }
      node.box = box;
    }
    const int index = (int)bvh.nodes.size();
    bvh.nodes.push_back(node);
    if (count <= LM_MAX_LEAF)
      return index;
    const vector3df ext = node.box.getExtent();
    int axis = 0;
    if (ext.Y > ext.X)
      axis = 1;
    if (Axis(ext, 2) > Axis(ext, axis))
      axis = 2;
    pool.set_used(count);
    for (int i = 0; i < count; ++i)
    {
      const LMTri &t = (*bvh.tris)[bvh.order[begin + i]];
      const vector3df c = (t.a + t.b + t.c) / 3.f;
      pool[i].index = bvh.order[begin + i];
      pool[i].centroid = Axis(c, axis);
    }
    qsort(pool.pointer(), (size_t)count, sizeof(LMRef), axis == 0 ? CmpX : (axis == 1 ? CmpY : CmpZ));
    for (int i = 0; i < count; ++i)
      bvh.order[begin + i] = pool[i].index;
    const int mid = begin + count / 2;
    const int l = BvhBuildIndex(bvh, pool, begin, mid);
    const int r = BvhBuildIndex(bvh, pool, mid, end);
    bvh.nodes[index].n0 = l;
    bvh.nodes[index].n1 = r;
    return index;
  }

  bool BvhOccluded(const LMBvh &bvh, u32 selfTri, const core::line3df &line, u32 ignoreTri = LM_NO_TRI)
  {
    const vector3df vec = line.getVector(); // direction from origin toward the light
    int stack[256];
    int sp = 0;
    stack[sp++] = 0;
    while (sp)
    {
      const int idx = stack[--sp];
      const LMNodeInfo &node = bvh.nodes[idx];
      if (!node.box.intersectsWithLine(line))
        continue;
      if (node.n0 < 0)
      {
        for (int i = 0; i < node.count; ++i)
        {
          const u32 t = bvh.order[node.start + i];
          if (t == selfTri || t == ignoreTri)
            continue;
          const LMTri &tri = (*bvh.tris)[t];
          if (vec.dotProduct(tri.normal) <= 0.f)
            continue; // backfacing occluder
          vector3df out;
          const triangle3df tri3(tri.a, tri.b, tri.c);
          if (tri3.getIntersectionWithLimitedLine(line, out))
            return true;
        }
      }
      else
      {
        stack[sp++] = node.n0;
        stack[sp++] = node.n1;
      }
    }
    return false;
  }

  void CollectNodes(ISceneNode *root, core::array<LMNodePart> &parts, core::array<LMLight> &lights)
  {
    if (!root)
      root = _Device()->getSceneManager()->getRootSceneNode();
    const ISceneNodeList &children = root->getChildren();
    for (ISceneNodeList::ConstIterator it = children.begin(); it != children.end(); ++it)
    {
      ISceneNode *n = *it;
      const ESCENE_NODE_TYPE type = n->getType();
      if (type == ESNT_LIGHT)
      {
        ILightSceneNode *ln = (ILightSceneNode *)n;
        const SLight &ld = ln->getLightData();
        LMLight l;
        l.type = ln->getLightType();
        l.diffuse = ld.DiffuseColor;
        l.attenuation = ld.Attenuation;
        l.falloff = ld.Falloff;
        l.castShadow = ld.CastShadows;
        l.cosInner = cosf(ld.InnerCone * DEG2RAD);
        l.cosOuter = cosf(ld.OuterCone * DEG2RAD);
        l.position = n->getAbsolutePosition();
        vector3df dir(0.f, 0.f, 1.f);
        n->getAbsoluteTransformation().rotateVect(dir);
        l.direction = dir.normalize();
        lights.push_back(l);
      }
      else if (type == ESNT_MESH || type == ESNT_OCTREE)
      {
        IMeshSceneNode *mn = (IMeshSceneNode *)n;
        if (mn->getMesh())
        {
          LMNodePart part;
          part.node = mn;
          part.newMesh = new SMesh();
          part.lightmapped = false;
          parts.push_back(part);
        }
      }
      CollectNodes(n, parts, lights);
    }
  }

  void BakeNode(LMNodePart &part, f32 density, core::array<LMTri> &tris)
  {
    IMesh *src = part.node->getMesh();
    const core::matrix4 &abs = part.node->getAbsoluteTransformation();
    for (u32 i = 0; i < src->getMeshBufferCount(); ++i)
    {
      IMeshBuffer *mb = src->getMeshBuffer(i);
      const video::SMaterial &nodeMat = part.node->getMaterial(i);
      const bool bake =
          mb->getVertexType() == EVT_STANDARD && mb->getIndexType() == EIT_16BIT &&
          mb->getPrimitiveType() == EPT_TRIANGLES && mb->getIndexCount() >= 3 &&
          mb->getIndexCount() <= 65535 && mb->getVertexCount() >= 3 && !nodeMat.isTransparent();
      if (!bake)
      {
        part.newMesh->addMeshBuffer(mb); // keep a reference to the source buffer
        continue;
      }
      const S3DVertex *vtx = (const S3DVertex *)mb->getVertices();
      const u16 *idx = mb->getIndices();
      SMeshBufferLightMap *dst = new SMeshBufferLightMap();
      dst->getMaterial() = nodeMat;
      part.newMesh->addMeshBuffer(dst);
      dst->drop();
      const u32 triCount = mb->getIndexCount() / 3;
      for (u32 t = 0; t < triCount; ++t)
      {
        const u16 i0 = idx[t * 3 + 0];
        const u16 i1 = idx[t * 3 + 1];
        const u16 i2 = idx[t * 3 + 2];
        vector3df a = vtx[i0].Pos;
        vector3df b = vtx[i1].Pos;
        vector3df c = vtx[i2].Pos;
        abs.transformVect(a);
        abs.transformVect(b);
        abs.transformVect(c);
        vector3df n = (b - a).crossProduct(c - a);
        const f32 len = n.getLength();
        if (len < LM_EPS)
          continue; // degenerate triangle
        n /= len;
        int plane;
        const f32 ax = fabsf(n.X), ay = fabsf(n.Y), az = fabsf(n.Z);
        if (ax >= ay && ax >= az)
          plane = 0;
        else if (ay >= az)
          plane = 1;
        else
          plane = 2;
        int axisU, axisV, axisW;
        GetPlaneAxes(plane, axisU, axisV, axisW);
        const vector3df corners[3] = { a, b, c };
        f32 loU = 0.f, loV = 0.f, hiU = 0.f, hiV = 0.f;
        for (int k = 0; k < 3; ++k)
        {
          f32 u, v;
          PlaneCoord(corners[k], axisU, axisV, u, v);
          if (k == 0)
          {
            loU = hiU = u;
            loV = hiV = v;
          }
          else
          {
            if (u < loU) loU = u;
            if (u > hiU) hiU = u;
            if (v < loV) loV = v;
            if (v > hiV) hiV = v;
          }
        }
        int uMin = (int)floorf(loU * density) - (int)LM_PAD;
        int uMax = (int)floorf(hiU * density) + (int)LM_PAD;
        int vMin = (int)floorf(loV * density) - (int)LM_PAD;
        int vMax = (int)floorf(hiV * density) + (int)LM_PAD;
        if (uMax < uMin) uMax = uMin;
        if (vMax < vMin) vMax = vMin;
        const u32 first = (u32)dst->Vertices.size();
        for (int k = 0; k < 3; ++k)
        {
          const S3DVertex &sv = vtx[idx[t * 3 + k]];
          S3DVertex2TCoords nv;
          nv.Pos = sv.Pos;
          nv.Normal = sv.Normal;
          nv.Color = sv.Color;
          nv.TCoords = sv.TCoords;
          nv.TCoords2 = vector2df(0.f, 0.f);
          dst->Vertices.push_back(nv);
        }
        dst->Indices.push_back((u16)first);
        dst->Indices.push_back((u16)(first + 1));
        dst->Indices.push_back((u16)(first + 2));
        LMTri tri;
        tri.a = a;
        tri.b = b;
        tri.c = c;
        tri.normal = n;
        tri.plane = plane;
        tri.uMin = uMin;
        tri.vMin = vMin;
        tri.uMax = uMax;
        tri.vMax = vMax;
        tri.atlasX = 0;
        tri.atlasY = 0;
        tri.dst = dst;
        tri.firstVertex = first;
        tri.centroid = (a + b + c) / 3.f;
        tri.area = len * 0.5f;
        tri.diffuse = nodeMat.DiffuseColor;
        tri.tex0 = nodeMat.getTexture(0);
        tri.albedo = nodeMat.DiffuseColor;
        tris.push_back(tri);
        part.lightmapped = true;
      }
      dst->recalculateBoundingBox();
    }
    part.newMesh->recalculateBoundingBox();
  }

  bool PackTris(core::array<LMTri> &tris, int maxAtlasSize, u32 &atlasSize)
  {
    const u32 n = tris.size();
    u64 totalArea = 0;
    for (u32 i = 0; i < n; ++i)
    {
      const u32 w = (u32)(tris[i].uMax - tris[i].uMin + 1);
      const u32 h = (u32)(tris[i].vMax - tris[i].vMin + 1);
      totalArea += (u64)w * (u64)h;
    }
    if (totalArea == 0)
      return false;
    u32 size = 8;
    {
      f64 need = sqrt((f64)totalArea * 1.5);
      while ((f64)size < need)
        size <<= 1;
    }
    for (;;)
    {
      if ((int)size > maxAtlasSize || size > 65535)
        return false;
      stbrp_context ctx;
      stbrp_node *nodes = new stbrp_node[size];
      stbrp_rect *rects = new stbrp_rect[n];
      for (u32 i = 0; i < n; ++i)
      {
        const u32 w = (u32)(tris[i].uMax - tris[i].uMin + 1);
        const u32 h = (u32)(tris[i].vMax - tris[i].vMin + 1);
        if (w > 65534 || h > 65534)
        {
          delete[] rects;
          delete[] nodes;
          return false;
        }
        rects[i].id = (int)i;
        rects[i].w = (stbrp_coord)w;
        rects[i].h = (stbrp_coord)h;
        rects[i].x = 0;
        rects[i].y = 0;
        rects[i].was_packed = 0;
      }
      stbrp_init_target(&ctx, size, size, nodes, (int)size);
      stbrp_setup_allow_out_of_mem(&ctx, 1);
      stbrp_pack_rects(&ctx, rects, (int)n);
      bool all = true;
      for (u32 i = 0; i < n; ++i)
      {
        if (!rects[i].was_packed)
        {
          all = false;
          break;
        }
      }
      if (all)
      {
        for (u32 i = 0; i < n; ++i)
        {
          tris[i].atlasX = rects[i].x;
          tris[i].atlasY = rects[i].y;
        }
        delete[] rects;
        delete[] nodes;
        atlasSize = size;
        return true;
      }
      delete[] rects;
      delete[] nodes;
      size <<= 1;
    }
  }

  SColorf ShadePoint(const vector3df &p, const vector3df &n, const core::array<LMLight> &lights,
                     const LMBvh &bvh, u32 selfTri, f32 dirMaxDist, f32 bias)
  {
    SColorf c;
    c.r = c.g = c.b = 0.f;
    for (u32 li = 0; li < lights.size(); ++li)
    {
      const LMLight &l = lights[li];
      vector3df rayDir;
      f32 rayDist;
      f32 ndl;
      if (l.type == ELT_DIRECTIONAL)
      {
        rayDir = -l.direction;
        rayDist = dirMaxDist;
        ndl = n.dotProduct(rayDir);
      }
      else
      {
        rayDir = l.position - p;
        rayDist = rayDir.getLength();
        if (rayDist < LM_EPS)
          continue;
        rayDir /= rayDist;
        ndl = n.dotProduct(rayDir);
      }
      if (ndl <= 0.f)
        continue;
      f32 factor = ndl;
      if (l.type == ELT_POINT || l.type == ELT_SPOT)
      {
        const f32 denom = l.attenuation.X + l.attenuation.Y * rayDist + l.attenuation.Z * rayDist * rayDist;
        if (denom > LM_EPS)
          factor = ndl / denom;
      }
      if (l.type == ELT_SPOT)
      {
        const f32 cosSpot = (p - l.position).dotProduct(l.direction) / rayDist;
        if (cosSpot < l.cosOuter)
          continue;
        f32 t = 1.f;
        if (l.cosInner > l.cosOuter)
        {
          t = (cosSpot - l.cosOuter) / (l.cosInner - l.cosOuter);
          if (t > 1.f)
            t = 1.f;
          if (t < 0.f)
            t = 0.f;
          if (l.falloff != 1.f)
            t = powf(t, l.falloff);
        }
        factor *= t;
      }
      if (l.castShadow)
      {
        const vector3df origin = p + n * bias;
        const vector3df end = origin + rayDir * rayDist;
        if (BvhOccluded(bvh, selfTri, core::line3df(origin, end)))
          continue;
      }
      c.r += l.diffuse.r * factor;
      c.g += l.diffuse.g * factor;
      c.b += l.diffuse.b * factor;
    }
    return c;
  }

  SColorf AverageTexels(const LMTri &tri)
  {
    SColorf s;
    s.r = s.g = s.b = 0.f;
    const u32 n = tri.texels.size();
    if (n)
    {
      for (u32 i = 0; i < n; ++i)
      {
        s.r += tri.texels[i].r;
        s.g += tri.texels[i].g;
        s.b += tri.texels[i].b;
      }
      s.r /= (f32)n;
      s.g /= (f32)n;
      s.b /= (f32)n;
    }
    return s;
  }

  vector3df TexelPoint(const LMTri &tri, int tx, int ty, f32 density)
  {
    int axisU, axisV, axisW;
    GetPlaneAxes(tri.plane, axisU, axisV, axisW);
    const f32 uw = ((f32)tx + 0.5f) / density;
    const f32 vw = ((f32)ty + 0.5f) / density;
    const f32 nd = tri.normal.dotProduct(tri.a);
    vector3df p;
    SetAxis(p, axisU, uw);
    SetAxis(p, axisV, vw);
    SetAxis(p, axisW,
            (nd - Axis(tri.normal, axisU) * uw - Axis(tri.normal, axisV) * vw) / Axis(tri.normal, axisW));
    return p;
  }

  void AddClamp(SColorf &cell, const SColorf &delta)
  {
    cell.r += delta.r;
    cell.g += delta.g;
    cell.b += delta.b;
    if (cell.r > 1.f) cell.r = 1.f;
    if (cell.g > 1.f) cell.g = 1.f;
    if (cell.b > 1.f) cell.b = 1.f;
    if (cell.r < 0.f) cell.r = 0.f;
    if (cell.g < 0.f) cell.g = 0.f;
    if (cell.b < 0.f) cell.b = 0.f;
  }

  SColorf SampleTextureAverage(ITexture *tex)
  {
    SColorf avg;
    avg.r = avg.g = avg.b = 1.f;
    if (!tex)
      return avg;
    void *data = tex->lock(ETLM_READ_ONLY);
    if (!data)
      return avg;
    const ECOLOR_FORMAT fmt = tex->getColorFormat();
    const u32 w = tex->getSize().Width;
    const u32 h = tex->getSize().Height;
    const u32 pitch = tex->getPitch();
    u64 sr = 0, sg = 0, sb = 0;
    u64 count = 0;
    for (u32 y = 0; y < h; ++y)
    {
      const u8 *row = (const u8 *)data + (u32)y * pitch;
      switch (fmt)
      {
        case ECF_A8R8G8B8:
          for (u32 x = 0; x < w; ++x)
          {
            const u8 *p = row + (u32)x * 4;
            sr += p[2];
            sg += p[1];
            sb += p[0];
          }
          count += w;
          break;
        case ECF_R8G8B8:
          for (u32 x = 0; x < w; ++x)
          {
            const u8 *p = row + (u32)x * 3;
            sr += p[0];
            sg += p[1];
            sb += p[2];
          }
          count += w;
          break;
        case ECF_R5G6B5:
          for (u32 x = 0; x < w; ++x)
          {
            const u16 v = *(const u16 *)(row + (u32)x * 2);
            sr += (v >> 11) & 31;
            sg += (v >> 5) & 63;
            sb += v & 31;
          }
          count += w;
          break;
        case ECF_A1R5G5B5:
          for (u32 x = 0; x < w; ++x)
          {
            const u16 v = *(const u16 *)(row + (u32)x * 2);
            sr += (v >> 10) & 31;
            sg += (v >> 5) & 31;
            sb += v & 31;
          }
          count += w;
          break;
        default:
          tex->unlock();
          avg.r = avg.g = avg.b = 1.f;
          return avg;
      }
    }
    tex->unlock();
    if (count)
    {
      if (fmt == ECF_R5G6B5)
      {
        avg.r = (f32)(sr / count) / 31.f;
        avg.g = (f32)(sg / count) / 63.f;
        avg.b = (f32)(sb / count) / 31.f;
      }
      else if (fmt == ECF_A1R5G5B5)
      {
        avg.r = (f32)(sr / count) / 31.f;
        avg.g = (f32)(sg / count) / 31.f;
        avg.b = (f32)(sb / count) / 31.f;
      }
      else
      {
        avg.r = (f32)(sr / count) / 255.f;
        avg.g = (f32)(sg / count) / 255.f;
        avg.b = (f32)(sb / count) / 255.f;
      }
    }
    return avg;
  }

  struct LMTextureAvg
  {
    ITexture *tex;
    SColorf avg;
  };

  void ComputeAlbedos(core::array<LMTri> &tris, bool useTextureAlbedo)
  {
    core::array<LMTextureAvg> cache;
    for (u32 i = 0; i < tris.size(); ++i)
    {
      LMTri &tri = tris[i];
      SColorf alb = tri.diffuse;
      if (useTextureAlbedo && tri.tex0)
      {
        SColorf tavg;
        bool found = false;
        for (u32 k = 0; k < cache.size(); ++k)
        {
          if (cache[k].tex == tri.tex0)
          {
            tavg = cache[k].avg;
            found = true;
            break;
          }
        }
        if (!found)
        {
          tavg = SampleTextureAverage(tri.tex0);
          LMTextureAvg e;
          e.tex = tri.tex0;
          e.avg = tavg;
          cache.push_back(e);
        }
        alb.r *= tavg.r;
        alb.g *= tavg.g;
        alb.b *= tavg.b;
      }
      tri.albedo = alb;
    }
  }

  SColorf GatherBounce(const vector3df &p, const vector3df &n, u32 selfTri,
                       const core::array<LMTri> &tris, const core::array<SColorf> &avg,
                       const LMBvh &bvh, f32 bias)
  {
    SColorf out;
    out.r = out.g = out.b = 0.f;
    for (u32 j = 0; j < tris.size(); ++j)
    {
      if (j == selfTri)
        continue;
      const LMTri &src = tris[j];
      // radiosity leaving the source = albedo * accumulated irradiance
      const f32 B = src.albedo.r * avg[j].r, Bg = src.albedo.g * avg[j].g, Bb = src.albedo.b * avg[j].b;
      if (B <= 0.f && Bg <= 0.f && Bb <= 0.f)
        continue;
      const vector3df d = src.centroid - p;
      f32 d2 = d.getLengthSQ();
      const f32 minD2 = src.area * LM_INV_PI; // clamp to avoid the singularity for touching patches
      if (d2 < minD2)
        d2 = minD2;
      const f32 dist = sqrtf(d2);
      const vector3df dir = d / dist;
      const f32 cosi = n.dotProduct(dir);
      if (cosi <= 0.f)
        continue;
      const f32 cosj = src.normal.dotProduct(-dir);
      if (cosj <= 0.f)
        continue;
      const f32 ff = cosi * cosj * src.area / (LM_PI * d2);
      if (ff <= 0.f)
        continue;
      const vector3df origin = p + n * bias;
      if (BvhOccluded(bvh, selfTri, core::line3df(origin, src.centroid), j))
        continue;
      out.r += B * ff;
      out.g += Bg * ff;
      out.b += Bb * ff;
    }
    return out;
  }

  void DropParts(core::array<LMNodePart> &parts)
  {
    for (u32 i = 0; i < parts.size(); ++i)
      parts[i].newMesh->drop();
  }

} // namespace

extern "C"
{

  EXPORT IImage *CALL BakeLightmap(ISceneNode *root, float texelDensity, int maxAtlasSize,
                                    int bounces, bool useTextureAlbedo)
  {
    if (texelDensity <= 0.f)
      texelDensity = 8.f;
    if (maxAtlasSize <= 0)
      maxAtlasSize = 2048;
    if (bounces < 0)
      bounces = 0;

    core::array<LMNodePart> parts;
    core::array<LMLight> lights;
    CollectNodes(root, parts, lights);
    if (parts.empty())
    {
      _Device()->getLogger()->log("BakeLightmap: no static mesh nodes found.", ELL_WARNING);
      return NULL;
    }

    core::array<LMTri> tris;
    for (u32 i = 0; i < parts.size(); ++i)
      BakeNode(parts[i], texelDensity, tris);
    if (tris.empty())
    {
      DropParts(parts);
      _Device()->getLogger()->log("BakeLightmap: no lightmapped geometry found.", ELL_WARNING);
      return NULL;
    }

    aabbox3df sceneBox;
    {
      const LMTri &t0 = tris[0];
      sceneBox = aabbox3df(t0.a, t0.a);
    }
    for (u32 i = 0; i < tris.size(); ++i)
    {
      sceneBox.addInternalPoint(tris[i].a);
      sceneBox.addInternalPoint(tris[i].b);
      sceneBox.addInternalPoint(tris[i].c);
    }
    const f32 dirMaxDist = sceneBox.getExtent().getLength() * 2.f + 10.f;
    const f32 bias = LM_BIAS_TEXELS / texelDensity;

    u32 atlasSize = 0;
    if (!PackTris(tris, maxAtlasSize, atlasSize))
    {
      DropParts(parts);
      _Device()->getLogger()->log(
          "BakeLightmap: atlas overflow, increase maxAtlasSize or lower texelDensity.", ELL_WARNING);
      return NULL;
    }

    LMBvh bvh;
    bvh.tris = &tris;
    bvh.order.set_used(tris.size());
    for (u32 i = 0; i < tris.size(); ++i)
      bvh.order[i] = i;
    core::array<LMRef> pool;
    pool.set_used(tris.size());
    BvhBuildIndex(bvh, pool, 0, (int)tris.size());

    const ECOLOR_FORMAT format =
        _Device()->getVideoDriver()->getTextureCreationFlag(ETCF_ALWAYS_32_BIT)
            ? ECF_A8R8G8B8
            : ECF_A1R5G5B5;
    IImage *pixmap = _Device()->getVideoDriver()->createImage(format, dimension2du(atlasSize, atlasSize));

    const SColorf ambient = _Device()->getSceneManager()->getAmbientLight();
    const f32 atlasF = (f32)atlasSize;

    ComputeAlbedos(tris, useTextureAlbedo != 0);
    for (u32 ti = 0; ti < tris.size(); ++ti)
    {
      const LMTri &tri = tris[ti];
      const u32 w = (u32)(tri.uMax - tri.uMin + 1);
      const u32 h = (u32)(tri.vMax - tri.vMin + 1);
      tris[ti].texels.set_used(w * h);
    }
    core::array<SColorf> avg;
    avg.set_used(tris.size());

    for (u32 ti = 0; ti < tris.size(); ++ti)
    {
      LMTri &tri = tris[ti];
      const vector3df corners[3] = { tri.a, tri.b, tri.c };
      for (int k = 0; k < 3; ++k)
      {
        int axisU, axisV, axisW;
        GetPlaneAxes(tri.plane, axisU, axisV, axisW);
        f32 u, v;
        PlaneCoord(corners[k], axisU, axisV, u, v);
        S3DVertex2TCoords &vv = tri.dst->Vertices[tri.firstVertex + k];
        vv.TCoords2.X = (u * texelDensity - (f32)tri.uMin + (f32)tri.atlasX) / atlasF;
        vv.TCoords2.Y = (v * texelDensity - (f32)tri.vMin + (f32)tri.atlasY) / atlasF;
      }
      const u32 w = (u32)(tri.uMax - tri.uMin + 1);
      for (int ty = tri.vMin; ty <= tri.vMax; ++ty)
      {
        for (int tx = tri.uMin; tx <= tri.uMax; ++tx)
        {
          SColorf col =
              ShadePoint(TexelPoint(tri, tx, ty, texelDensity), tri.normal, lights, bvh, ti, dirMaxDist, bias);
          AddClamp(col, ambient);
          tri.texels[(u32)((ty - tri.vMin) * (int)w + (tx - tri.uMin))] = col;
        }
      }
      avg[ti] = AverageTexels(tri);
    }

    // Progressive radiosity: reflect each surface's radiance onto every other surface.
    for (int pass = 0; pass < bounces; ++pass)
    {
      for (u32 ti = 0; ti < tris.size(); ++ti)
      {
        LMTri &tri = tris[ti];
        const u32 w = (u32)(tri.uMax - tri.uMin + 1);
        for (int ty = tri.vMin; ty <= tri.vMax; ++ty)
        {
          for (int tx = tri.uMin; tx <= tri.uMax; ++tx)
          {
            SColorf &cell = tri.texels[(u32)((ty - tri.vMin) * (int)w + (tx - tri.uMin))];
            AddClamp(cell,
                     GatherBounce(TexelPoint(tri, tx, ty, texelDensity), tri.normal, ti, tris, avg, bvh, bias));
          }
        }
        avg[ti] = AverageTexels(tri);
      }
    }

    for (u32 ti = 0; ti < tris.size(); ++ti)
    {
      const LMTri &tri = tris[ti];
      const u32 w = (u32)(tri.uMax - tri.uMin + 1);
      for (int ty = tri.vMin; ty <= tri.vMax; ++ty)
      {
        for (int tx = tri.uMin; tx <= tri.uMax; ++tx)
        {
          const SColorf &col = tri.texels[(u32)((ty - tri.vMin) * (int)w + (tx - tri.uMin))];
          pixmap->setPixel(tri.atlasX + (tx - tri.uMin), tri.atlasY + (ty - tri.vMin),
                           SColor(255, (u32)(col.r * 255.f), (u32)(col.g * 255.f), (u32)(col.b * 255.f)));
        }
      }
    }

    static int lmCounter = 0;
    char name[64];
    sprintf(name, "__lightmap_%d", lmCounter++);
    ITexture *tex = _Device()->getVideoDriver()->addTexture(name, pixmap);
    if (tex)
    {
      for (u32 i = 0; i < parts.size(); ++i)
      {
        LMNodePart &part = parts[i];
        if (!part.lightmapped)
          continue;
        SMesh *m = part.newMesh;
        for (u32 j = 0; j < m->getMeshBufferCount(); ++j)
        {
          IMeshBuffer *mb = m->getMeshBuffer(j);
          if (mb->getVertexType() == EVT_2TCOORDS)
          {
            mb->getMaterial().MaterialType = EMT_LIGHTMAP_LIGHTING;
            mb->getMaterial().setTexture(1, tex);
          }
        }
        m->recalculateBoundingBox();
        part.node->setMesh(m, true);
      }
    }
    else
    {
      _Device()->getLogger()->log("BakeLightmap: could not create lightmap texture.", ELL_WARNING);
    }
    DropParts(parts);
    return pixmap;
  }

} // extern "C"