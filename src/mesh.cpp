#include "color.h"
#include "core.h"
#include "material.h"
#include "mesh.h"
#include "surface.h"

extern "C"
{

  static IMesh *FixMaterials(IMesh *mesh, bool fix_specular = true)
  {
    for (u32 i = 0; i < mesh->getMeshBufferCount(); ++i)
    {
      _FixMaterial(&mesh->getMeshBuffer(i)->getMaterial(), fix_specular);
    }
    return mesh;
  }

  EXPORT IMesh *CALL CreateConeMesh(int segments)
  {
    return FixMaterials(_Device()->getSceneManager()->getGeometryCreator()->createConeMesh(0.5f, 1, segments));
  }

#ifndef IRRLICHT_SVN
  struct Vertex
  {
    float x, y, z;    // Position
    float nx, ny, nz; // Normal
    int color;        // Color
    float u, v;       // Texture coordinates

    Vertex(float x, float y, float z, float nx, float ny, float nz, int color, float u, float v)
        : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), color(color), u(u), v(v) {}
  };
#endif

  EXPORT IMesh *CALL CreateCubeMesh()
  {
#ifdef IRRLICHT_SVN
    return FixMaterials(_Device()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1), ECMT_1BUF_24VTX_NP));
#else
    array<Vertex> vertices;
    vertices.set_used(24);
    vertices[0] = Vertex(-0.5f, 0.5f, -0.5f, 0, 0, -1, COLOR_WHITE, 0, 0);
    vertices[1] = Vertex(0.5f, 0.5f, -0.5f, 0, 0, -1, COLOR_WHITE, 1, 0);
    vertices[2] = Vertex(-0.5f, -0.5f, -0.5f, 0, 0, -1, COLOR_WHITE, 0, 1);
    vertices[3] = Vertex(0.5f, -0.5f, -0.5f, 0, 0, -1, COLOR_WHITE, 1, 1);
    vertices[4] = Vertex(0.5f, 0.5f, 0.5f, 0, 0, 1, COLOR_WHITE, 0, 0);
    vertices[5] = Vertex(-0.5f, 0.5f, 0.5f, 0, 0, 1, COLOR_WHITE, 1, 0);
    vertices[6] = Vertex(0.5f, -0.5f, 0.5f, 0, 0, 1, COLOR_WHITE, 0, 1);
    vertices[7] = Vertex(-0.5f, -0.5f, 0.5f, 0, 0, 1, COLOR_WHITE, 1, 1);
    vertices[8] = Vertex(-0.5f, 0.5f, 0.5f, -1, 0, 0, COLOR_WHITE, 0, 0);
    vertices[9] = Vertex(-0.5f, 0.5f, -0.5f, -1, 0, 0, COLOR_WHITE, 1, 0);
    vertices[10] = Vertex(-0.5f, -0.5f, 0.5f, -1, 0, 0, COLOR_WHITE, 0, 1);
    vertices[11] = Vertex(-0.5f, -0.5f, -0.5f, -1, 0, 0, COLOR_WHITE, 1, 1);
    vertices[12] = Vertex(0.5f, 0.5f, -0.5f, 1, 0, 0, COLOR_WHITE, 0, 0);
    vertices[13] = Vertex(0.5f, 0.5f, 0.5f, 1, 0, 0, COLOR_WHITE, 1, 0);
    vertices[14] = Vertex(0.5f, -0.5f, -0.5f, 1, 0, 0, COLOR_WHITE, 0, 1);
    vertices[15] = Vertex(0.5f, -0.5f, 0.5f, 1, 0, 0, COLOR_WHITE, 1, 1);
    vertices[16] = Vertex(-0.5f, 0.5f, 0.5f, 0, 1, 0, COLOR_WHITE, 0, 0);
    vertices[17] = Vertex(0.5f, 0.5f, 0.5f, 0, 1, 0, COLOR_WHITE, 1, 0);
    vertices[18] = Vertex(-0.5f, 0.5f, -0.5f, 0, 1, 0, COLOR_WHITE, 0, 1);
    vertices[19] = Vertex(0.5f, 0.5f, -0.5f, 0, 1, 0, COLOR_WHITE, 1, 1);
    vertices[20] = Vertex(-0.5f, -0.5f, -0.5f, 0, -1, 0, COLOR_WHITE, 0, 0);
    vertices[21] = Vertex(0.5f, -0.5f, -0.5f, 0, -1, 0, COLOR_WHITE, 1, 0);
    vertices[22] = Vertex(-0.5f, -0.5f, 0.5f, 0, -1, 0, COLOR_WHITE, 0, 1);
    vertices[23] = Vertex(0.5f, -0.5f, 0.5f, 0, -1, 0, COLOR_WHITE, 1, 1);
    array<u16> indices;
    indices.set_used(36);
    indices[0] = 0; // Front face
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;
    indices[4] = 2;
    indices[5] = 1;
    indices[6] = 4; // Back face
    indices[7] = 5;
    indices[8] = 6;
    indices[9] = 7;
    indices[10] = 6;
    indices[11] = 5;
    indices[12] = 8; // Left face
    indices[13] = 9;
    indices[14] = 10;
    indices[15] = 11;
    indices[16] = 10;
    indices[17] = 9;
    indices[18] = 12; // Right face
    indices[19] = 13;
    indices[20] = 14;
    indices[21] = 15;
    indices[22] = 14;
    indices[23] = 13;
    indices[24] = 16; // Top face
    indices[25] = 17;
    indices[26] = 18;
    indices[27] = 19;
    indices[28] = 18;
    indices[29] = 17;
    indices[30] = 20; // Bottom face
    indices[31] = 21;
    indices[32] = 22;
    indices[33] = 23;
    indices[34] = 22;
    indices[35] = 21;
    IMesh *cube = CreateMesh();
    IMeshBuffer *surf = AddSurface(cube, &vertices[0], vertices.size(), &indices[0], indices.size(), SURFACE_STANDARD);
    return FixMaterials(cube);
#endif
  }

  EXPORT IMesh *CALL CreateCylinderMesh(int segments)
  {
    return FixMaterials(_Device()->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5f, 1, segments));
  }

  EXPORT IMesh *CALL CreateMesh()
  {
    return new SMesh();
  }

  EXPORT IMesh *CALL CreateQuadMesh()
  {
    IMesh *quad = FixMaterials(_Device()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1, 1)));
    RotateMesh(quad, -90, 0, 0);
    return quad;
  }

  EXPORT IMesh *CALL CreateSimpleCubeMesh()
  {
    return FixMaterials(_Device()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1)));
  }

  EXPORT IMesh *CALL CreateSphereMesh(int segments)
  {
    return FixMaterials(_Device()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments));
  }

  EXPORT IMesh *CALL LoadMesh(const char *filename)
  {
    return FixMaterials(_Device()->getSceneManager()->getMesh(filename), false);
  }

  EXPORT IMesh *CALL LoadTerrainMesh(IImage *heightmap, IImage *pixmap, float width, float height, float depth)
  {
    if (heightmap && pixmap)
    {
      const dimension2df stretch = dimension2df(
          float(width) / heightmap->getDimension().Width,
          float(depth) / heightmap->getDimension().Height);
      return FixMaterials(_Device()->getSceneManager()->addTerrainMesh("*", pixmap, heightmap, stretch, height));
    }
    return NULL;
  }

  EXPORT void CALL FreeMesh(IMesh *mesh)
  {
    if (_MeshAnimated(mesh))
    {
      // Animated meshes are the only cached ones
      _Device()->getSceneManager()->getMeshCache()->removeMesh(mesh);
    }
    else
    {
      mesh->drop();
    }
  }

  EXPORT void CALL SetMeshFPS(IMesh *mesh, float fps)
  {
    if (_MeshAnimated(mesh))
    {
      ((IAnimatedMesh *)mesh)->setAnimationSpeed(fps);
    }
  }

  EXPORT float CALL MeshFPS(IMesh *mesh)
  {
    return (_MeshAnimated(mesh))
               ? ((IAnimatedMesh *)mesh)->getAnimationSpeed()
               : 0;
  }

  EXPORT int CALL MeshFrames(IMesh *mesh)
  {
    return (_MeshAnimated(mesh))
               ? ((IAnimatedMesh *)mesh)->getFrameCount()
               : 1;
  }

  EXPORT IMesh *CALL MeshForFrame(IMesh *mesh, int frame)
  {
    return (_MeshAnimated(mesh))
               ? ((IAnimatedMesh *)mesh)->getMesh(frame)
               : mesh;
  }

  EXPORT int CALL NumSurfaces(IMesh *mesh)
  {
    return mesh->getMeshBufferCount();
  }

  EXPORT IMeshBuffer *CALL MeshSurface(IMesh *mesh, int index)
  {
    return mesh->getMeshBuffer(index - 1);
  }

  EXPORT void CALL UpdateMesh(IMesh *mesh)
  {
    for (int i = 1; i <= NumSurfaces(mesh); ++i)
    {
      MeshSurface(mesh, i)->recalculateBoundingBox();
    }
    ((SMesh *)mesh)->recalculateBoundingBox();
    mesh->setDirty();
  }

  EXPORT void CALL TranslateMesh(IMesh *mesh, float x, float y, float z)
  {
    matrix4 m;
    m.setTranslation(vector3df(x, y, z));
    _Device()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
  }

  EXPORT void CALL RotateMesh(IMesh *mesh, float pitch, float yaw, float roll)
  {
    matrix4 m;
    m.setRotationDegrees(vector3df(pitch, yaw, roll));
    _Device()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
  }

  EXPORT void CALL ScaleMesh(IMesh *mesh, float x, float y, float z)
  {
    _Device()->getSceneManager()->getMeshManipulator()->scale(mesh, vector3df(x, y, z));
  }

  EXPORT void CALL FlipMesh(IMesh *mesh)
  {
    _Device()->getSceneManager()->getMeshManipulator()->flipSurfaces(mesh);
  }

  EXPORT void CALL SetMeshColor(IMesh *mesh, int color)
  {
    _Device()->getSceneManager()->getMeshManipulator()->setVertexColors(mesh, _Color(color));
  }

  EXPORT void CALL UpdateMeshNormals(IMesh *mesh)
  {
    _Device()->getSceneManager()->getMeshManipulator()->recalculateNormals(mesh);
  }

  EXPORT float CALL MeshWidth(IMesh *mesh)
  {
    return mesh->getBoundingBox().getExtent().X;
  }

  EXPORT float CALL MeshHeight(IMesh *mesh)
  {
    return mesh->getBoundingBox().getExtent().Y;
  }

  EXPORT float CALL MeshDepth(IMesh *mesh)
  {
    return mesh->getBoundingBox().getExtent().Z;
  }

  bool_t _MeshAnimated(IMesh *mesh)
  {
    return _Device()->getSceneManager()->getMeshCache()->getMeshIndex(mesh) != -1;
  }

} // extern "C"
