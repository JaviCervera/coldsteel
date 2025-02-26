#include "core.h"
#include "graphics.h"
#include "mesh.h"
#include "surface.h"

extern "C"
{

  IMesh *FixMeshSpecular(IMesh *mesh);

  EXPORT IMesh *CALL CreateCone(int segments)
  {
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createConeMesh(0.5f, 1, segments));
  }

  EXPORT IMesh *CALL CreateCube()
  {
    const int white = RGB(255, 255, 255);
    IMesh *cube = CreateMesh();
    IMeshBuffer *surf = AddSurface(cube);

    // Front face
    AddVertex(surf, -0.5f, 0.5f, -0.5f, 0, 0, -1, white, 0, 0);
    AddVertex(surf, 0.5f, 0.5f, -0.5f, 0, 0, -1, white, 1, 0);
    AddVertex(surf, -0.5f, -0.5f, -0.5f, 0, 0, -1, white, 0, 1);
    AddVertex(surf, 0.5f, -0.5f, -0.5f, 0, 0, -1, white, 1, 1);
    AddIndex(surf, 0);
    AddIndex(surf, 1);
    AddIndex(surf, 2);
    AddIndex(surf, 3);
    AddIndex(surf, 2);
    AddIndex(surf, 1);

    // Back face
    AddVertex(surf, 0.5f, 0.5f, 0.5f, 0, 0, 1, white, 0, 0);
    AddVertex(surf, -0.5f, 0.5f, 0.5f, 0, 0, 1, white, 1, 0);
    AddVertex(surf, 0.5f, -0.5f, 0.5f, 0, 0, 1, white, 0, 1);
    AddVertex(surf, -0.5f, -0.5f, 0.5f, 0, 0, 1, white, 1, 1);
    AddIndex(surf, 4);
    AddIndex(surf, 5);
    AddIndex(surf, 6);
    AddIndex(surf, 7);
    AddIndex(surf, 6);
    AddIndex(surf, 5);

    // Left face
    AddVertex(surf, -0.5f, 0.5f, 0.5f, -1, 0, 0, white, 0, 0);
    AddVertex(surf, -0.5f, 0.5f, -0.5f, -1, 0, 0, white, 1, 0);
    AddVertex(surf, -0.5f, -0.5f, 0.5f, -1, 0, 0, white, 0, 1);
    AddVertex(surf, -0.5f, -0.5f, -0.5f, -1, 0, 0, white, 1, 1);
    AddIndex(surf, 8);
    AddIndex(surf, 9);
    AddIndex(surf, 10);
    AddIndex(surf, 11);
    AddIndex(surf, 10);
    AddIndex(surf, 9);

    // Right face
    AddVertex(surf, 0.5f, 0.5f, -0.5f, 1, 0, 0, white, 0, 0);
    AddVertex(surf, 0.5f, 0.5f, 0.5f, 1, 0, 0, white, 1, 0);
    AddVertex(surf, 0.5f, -0.5f, -0.5f, 1, 0, 0, white, 0, 1);
    AddVertex(surf, 0.5f, -0.5f, 0.5f, 1, 0, 0, white, 1, 1);
    AddIndex(surf, 12);
    AddIndex(surf, 13);
    AddIndex(surf, 14);
    AddIndex(surf, 15);
    AddIndex(surf, 14);
    AddIndex(surf, 13);

    // Top face
    AddVertex(surf, -0.5f, 0.5f, 0.5f, 0, 1, 0, white, 0, 0);
    AddVertex(surf, 0.5f, 0.5f, 0.5f, 0, 1, 0, white, 1, 0);
    AddVertex(surf, -0.5f, 0.5f, -0.5f, 0, 1, 0, white, 0, 1);
    AddVertex(surf, 0.5f, 0.5f, -0.5f, 0, 1, 0, white, 1, 1);
    AddIndex(surf, 16);
    AddIndex(surf, 17);
    AddIndex(surf, 18);
    AddIndex(surf, 19);
    AddIndex(surf, 18);
    AddIndex(surf, 17);

    // Bottom face
    AddVertex(surf, -0.5f, -0.5f, -0.5f, 0, -1, 0, white, 0, 0);
    AddVertex(surf, 0.5f, -0.5f, -0.5f, 0, -1, 0, white, 1, 0);
    AddVertex(surf, -0.5f, -0.5f, 0.5f, 0, -1, 0, white, 0, 1);
    AddVertex(surf, 0.5f, -0.5f, 0.5f, 0, -1, 0, white, 1, 1);
    AddIndex(surf, 20);
    AddIndex(surf, 21);
    AddIndex(surf, 22);
    AddIndex(surf, 23);
    AddIndex(surf, 22);
    AddIndex(surf, 21);

    // Update mesh
    UpdateMesh(cube);

    return FixMeshSpecular(cube);
  }

  EXPORT IMesh *CALL CreateCylinder(int segments)
  {
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5f, 1, segments));
  }

  EXPORT IMesh *CALL CreateMesh()
  {
    return new SMesh();
  }

  EXPORT IMesh *CALL CreateQuad()
  {
    IMesh *quad = FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1, 1)));
    RotateMesh(quad, -90, 0, 0);
    return quad;
  }

  EXPORT IMesh *CALL CreateSphere(int segments)
  {
    return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments));
  }

  EXPORT IMesh *CALL LoadMesh(const char *filename)
  {
    return _Device()->getSceneManager()->getMesh(filename);
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
    return mesh->getMeshBuffer(index);
  }

  EXPORT void CALL UpdateMesh(IMesh *mesh)
  {
    for (int i = 0; i < NumSurfaces(mesh); ++i)
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
