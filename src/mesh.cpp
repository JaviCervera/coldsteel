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

  EXPORT IMesh *CALL CreateCubeMesh()
  {
    return FixMaterials(_Device()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1), ECMT_1BUF_24VTX_NP));
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

  EXPORT bool_t CALL SaveMesh(IMesh *mesh, const char *filename)
  {
    if (mesh && filename)
    {
      IMeshWriter *writer = _Device()->getSceneManager()->createMeshWriter(EMWT_OBJ);
      if (writer)
      {
        IWriteFile *file = _Device()->getFileSystem()->createAndWriteFile(filename);
        if (file)
        {
          bool_t result = (bool_t)writer->writeMesh(file, mesh);
          file->drop();
          writer->drop();
          return result;
        }
        writer->drop();
      }
    }
    return FALSE;
  }

  bool_t _MeshAnimated(IMesh *mesh)
  {
    return _Device()->getSceneManager()->getMeshCache()->getMeshIndex(mesh) != -1;
  }

} // extern "C"
