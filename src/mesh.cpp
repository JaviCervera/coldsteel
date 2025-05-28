#include "color.h"
#include "core.h"
#include "mesh.h"
#include "surface.h"

extern "C"
{

  IMesh *FixMeshSpecular(IMesh *mesh);

  EXPORT IMesh *CALL CreateConeMesh(int segments)
  {
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createConeMesh(0.5f, 1, segments));
  }

#ifdef LEGACY_IRRLICHT
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
#ifndef LEGACY_IRRLICHT
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1), ECMT_1BUF_24VTX_NP));
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
    vertices[20] = Vertex(-0.5f, -0.5f, 0.5f, 0, -1, 0, COLOR_WHITE, 0, 0);
    vertices[21] = Vertex(0.5f, -0.5f, 0.5f, 0, -1, 0, COLOR_WHITE, 1, 0);
    vertices[22] = Vertex(-0.5f, -0.5f, -0.5f, 0, -1, 0, COLOR_WHITE, 0, 1);
    vertices[23] = Vertex(0.5f, -0.5f, -0.5f, 0, -1, 0, COLOR_WHITE, 1, 1);
    array<u16> indices;
    indices.set_used(36);
    indices[0] = 1;
    indices[1] = 2;
    indices[2] = 3; // Front face
    indices[3] = 4;
    indices[4] = 3;
    indices[5] = 2;
    indices[6] = 5;
    indices[7] = 6;
    indices[8] = 7; // Back face
    indices[9] = 8;
    indices[10] = 7;
    indices[11] = 6;
    indices[12] = 9;
    indices[13] = 10;
    indices[14] = 11; // Left face
    indices[15] = 12;
    indices[16] = 11;
    indices[17] = 10;
    indices[18] = 13;
    indices[19] = 14;
    indices[20] = 15; // Right face
    indices[21] = 16;
    indices[22] = 15;
    indices[23] = 14;
    indices[24] = 17;
    indices[25] = 18;
    indices[26] = 19; // Top face
    indices[27] = 20;
    indices[28] = 19;
    indices[29] = 18;
    indices[30] = 21;
    indices[31] = 22;
    indices[32] = 23; // Bottom face
    indices[33] = 24;
    indices[34] = 23;
    indices[35] = 22;
    IMesh *cube = CreateMesh();
    IMeshBuffer *surf = AddSurface(cube, &vertices[0], vertices.size(), &indices[0], indices.size(), SURFACE_STANDARD);
    return FixMeshSpecular(cube);
#endif
  }

  EXPORT IMesh *CALL CreateCylinderMesh(int segments)
  {
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5f, 1, segments));
  }

  EXPORT IMesh *CALL CreateMesh()
  {
    return new SMesh();
  }

  EXPORT IMesh *CALL CreateQuadMesh()
  {
    IMesh *quad = FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1, 1)));
    RotateMesh(quad, -90, 0, 0);
    return quad;
  }

  EXPORT IMesh *CALL CreateSimpleCubeMesh()
  {
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1)));
  }

  EXPORT IMesh *CALL CreateSphereMesh(int segments)
  {
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments));
  }

  EXPORT IMesh *CALL LoadMesh(const char *filename)
  {
    return _Device()->getSceneManager()->getMesh(filename);
  }

  EXPORT IMesh *CALL LoadTerrainMesh(IImage *heightmap, IImage *pixmap, float width, float height, float depth)
  {
    if (heightmap && pixmap)
    {
      const dimension2df stretch = dimension2df(
          float(width) / heightmap->getDimension().Width,
          float(depth) / heightmap->getDimension().Height);
      return _Device()->getSceneManager()->addTerrainMesh("*", pixmap, heightmap, stretch, height);
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

  IMesh *FixMeshSpecular(IMesh *mesh)
  {
    for (u32 i = 0; i < mesh->getMeshBufferCount(); ++i)
    {
      mesh->getMeshBuffer(i)->getMaterial().SpecularColor = SColor(255, 0, 0, 0);
    }
    return mesh;
  }

} // extern "C"
