#include "core.h"
#include "entity.h"
#include "mesh.h"
#include "mesh_entity.h"

extern "C"
{

  EXPORT IMeshSceneNode *CALL CreateMeshEntity(IMesh *mesh)
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

  EXPORT IMeshSceneNode *CALL CreateOctreeMeshEntity(IMesh *mesh)
  {
    return _Device()->getSceneManager()->addOctreeSceneNode(mesh);
  }

  EXPORT IMesh *CALL MeshEntityMesh(IMeshSceneNode *entity)
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

  EXPORT void CALL SetMeshEntityCastShadows(IMeshSceneNode *entity, bool_t enable)
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

  EXPORT bool_t CALL MeshEntityCastShadows(IMeshSceneNode *entity)
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

  EXPORT void CALL SetMeshEntityLoop(IMeshSceneNode *entity, bool_t loop)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setLoopMode(loop);
  }

  EXPORT bool_t CALL MeshEntityLoop(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getLoopMode()
               : false;
  }

  EXPORT void CALL SetMeshEntityFPS(IMeshSceneNode *entity, float fps)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setAnimationSpeed(fps);
  }

  EXPORT float CALL MeshEntityFPS(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getAnimationSpeed()
               : 0;
  }

  EXPORT void CALL SetMeshEntityFrame(IMeshSceneNode *entity, float frame)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setCurrentFrame(frame);
  }

  EXPORT float CALL MeshEntityFrame(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getFrameNr()
               : -1;
  }

  EXPORT void CALL SetMeshEntityFrames(IMeshSceneNode *entity, int first, int last)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
      ((IAnimatedMeshSceneNode *)entity)->setFrameLoop(first, last);
  }

  EXPORT int CALL MeshEntityFirstFrame(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getStartFrame()
               : -1;
  }

  EXPORT int CALL MeshEntityLastFrame(IMeshSceneNode *entity)
  {
    return (entity->getType() == ESNT_ANIMATED_MESH)
               ? ((IAnimatedMeshSceneNode *)entity)->getEndFrame()
               : -1;
  }

} // extern "C"
