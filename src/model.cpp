#include "core.h"
#include "entity.h"
#include "mesh.h"
#include "model.h"

extern "C"
{

  EXPORT IMeshSceneNode *CALL CreateModel(IMesh *mesh)
  {
    if (_MeshAnimated(mesh))
    {
      return (IMeshSceneNode *)_Device()->getSceneManager()->addAnimatedMeshSceneNode((IAnimatedMesh *)mesh);
    }
    else
    {
      return _Device()->getSceneManager()->addMeshSceneNode(mesh);
    }
  }

  EXPORT IMeshSceneNode *CALL CreateOctreeModel(IMesh *mesh)
  {
    return _Device()->getSceneManager()->addOctreeSceneNode(mesh);
  }

  EXPORT IMeshSceneNode *CALL LoadModel(const char *filename)
  {
    IMesh *mesh = LoadMesh(filename);
    if (!mesh)
      return NULL;
    IMeshSceneNode *model = CreateModel(mesh);
    FreeMesh(mesh);
    return model;
  }

  EXPORT IMeshSceneNode *CALL LoadOctreeModel(const char *filename)
  {
    IMesh *mesh = LoadMesh(filename);
    if (!mesh)
      return NULL;
    IMeshSceneNode *model = CreateOctreeModel(mesh);
    FreeMesh(mesh);
    return model;
  }

  EXPORT IMesh *CALL ModelMesh(IMeshSceneNode *entity)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
    {
      return ((IAnimatedMeshSceneNode *)entity)->getMesh();
    }
    else
    {
      return entity->getMesh();
    }
  }

  EXPORT void CALL SetModelCastShadows(IMeshSceneNode *entity, bool_t enable)
  {
    int num = EntityNumChildren(entity);
    ISceneNode *shadowNode = NULL;
    for (int i = 0; i < num; ++i)
    {
      ISceneNode *child = EntityChild(entity, i);
      if (child->getType() == ESNT_SHADOW_VOLUME)
      {
        shadowNode = child;
        break;
      }
    }
    if (shadowNode)
      shadowNode->remove();
    if (enable)
    {
      if (entity->getType() == ESNT_ANIMATED_MESH)
      {
        ((IAnimatedMeshSceneNode *)entity)->addShadowVolumeSceneNode();
      }
      else
      {
        entity->addShadowVolumeSceneNode();
      }
    }
  }

  EXPORT bool_t CALL ModelCastShadows(IMeshSceneNode *entity)
  {
    int num = EntityNumChildren(entity);
    for (int i = 0; i < num; ++i)
    {
      if (EntityChild(entity, i)->getType() == ESNT_SHADOW_VOLUME)
      {
        return true;
      }
    }
    return false;
  }

  EXPORT void CALL SetModelLoop(IMeshSceneNode *entity, bool_t loop)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setLoopMode(loop);
  }

  EXPORT bool_t CALL ModelLoop(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getLoopMode()
               : false;
  }

  EXPORT void CALL SetModelFPS(IMeshSceneNode *entity, float fps)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setAnimationSpeed(fps);
  }

  EXPORT float CALL ModelFPS(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getAnimationSpeed()
               : 0;
  }

  EXPORT void CALL SetModelFrame(IMeshSceneNode *entity, float frame)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setCurrentFrame(frame);
  }

  EXPORT float CALL ModelFrame(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getFrameNr()
               : -1;
  }

  EXPORT void CALL SetModelFrames(IMeshSceneNode *entity, int first, int last)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setFrameLoop(first, last);
  }

  EXPORT int CALL ModelFirstFrame(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getStartFrame()
               : -1;
  }

  EXPORT int CALL ModelLastFrame(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getEndFrame()
               : -1;
  }

} // extern "C"
