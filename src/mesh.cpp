#include "core.h"
#include "graphics.h"
#include "mesh.h"
#include "surface.h"

IMesh *FixMeshSpecular(IMesh *mesh);

IMesh *CreateCone(int segments)
{
  return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createConeMesh(0.5f, 1, segments));
}

IMesh *CreateCube()
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

IMesh *CreateCylinder(int segments)
{
  return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5f, 1, segments));
}

IMesh *CreateMesh()
{
  return new SMesh();
}

IMesh *CreateQuad()
{
  IMesh *quad = FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1, 1)));
  RotateMesh(quad, -90, 0, 0);
  return quad;
}

IMesh *CreateSphere(int segments)
{
  return FixMeshSpecular(_Device()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments));
}

IMesh *LoadMesh(const char *filename)
{
  return _Device()->getSceneManager()->getMesh(filename);
}

void FreeMesh(IMesh *mesh)
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

void SetMeshFPS(IMesh *mesh, float fps)
{
  if (_MeshAnimated(mesh))
  {
    ((IAnimatedMesh *)mesh)->setAnimationSpeed(fps);
  }
}

float MeshFPS(IMesh *mesh)
{
  return (_MeshAnimated(mesh))
             ? ((IAnimatedMesh *)mesh)->getAnimationSpeed()
             : 0;
}

int MeshFrames(IMesh *mesh)
{
  return (_MeshAnimated(mesh))
             ? ((IAnimatedMesh *)mesh)->getFrameCount()
             : 1;
}

IMesh *MeshForFrame(IMesh *mesh, int frame)
{
  return (_MeshAnimated(mesh))
             ? ((IAnimatedMesh *)mesh)->getMesh(frame)
             : mesh;
}

int NumSurfaces(IMesh *mesh)
{
  return mesh->getMeshBufferCount();
}

IMeshBuffer *MeshSurface(IMesh *mesh, int index)
{
  return mesh->getMeshBuffer(index);
}

void UpdateMesh(IMesh *mesh)
{
  for (int i = 0; i < NumSurfaces(mesh); ++i)
  {
    MeshSurface(mesh, i)->recalculateBoundingBox();
  }
  ((SMesh *)mesh)->recalculateBoundingBox();
  mesh->setDirty();
}

void TranslateMesh(IMesh *mesh, float x, float y, float z)
{
  matrix4 m;
  m.setTranslation(vector3df(x, y, z));
  _Device()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}

void RotateMesh(IMesh *mesh, float pitch, float yaw, float roll)
{
  matrix4 m;
  m.setRotationDegrees(vector3df(pitch, yaw, roll));
  _Device()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}

void ScaleMesh(IMesh *mesh, float x, float y, float z)
{
  _Device()->getSceneManager()->getMeshManipulator()->scale(mesh, vector3df(x, y, z));
}

void FlipMesh(IMesh *mesh)
{
  _Device()->getSceneManager()->getMeshManipulator()->flipSurfaces(mesh);
}

void SetMeshColor(IMesh *mesh, int color)
{
  _Device()->getSceneManager()->getMeshManipulator()->setVertexColors(mesh, _Color(color));
}

void UpdateMeshNormals(IMesh *mesh)
{
  _Device()->getSceneManager()->getMeshManipulator()->recalculateNormals(mesh);
}

float MeshWidth(IMesh *mesh)
{
  return mesh->getBoundingBox().getExtent().X;
}

float MeshHeight(IMesh *mesh)
{
  return mesh->getBoundingBox().getExtent().Y;
}

float MeshDepth(IMesh *mesh)
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
