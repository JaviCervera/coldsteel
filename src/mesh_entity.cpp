#include "core.h"
#include "entity.h"
#include "mesh.h"
#include "mesh_entity.h"

IMeshSceneNode *CreateMeshEntity(IMesh *mesh)
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

IMeshSceneNode *CreateOctreeMeshEntity(IMesh *mesh)
{
  return _Device()->getSceneManager()->addOctreeSceneNode(mesh);
}

IMesh *MeshEntityMesh(IMeshSceneNode *entity)
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

void SetMeshEntityCastShadows(IMeshSceneNode *entity, bool_t enable)
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

bool_t MeshEntityCastShadows(IMeshSceneNode *entity)
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

void SetMeshEntityLoop(IMeshSceneNode *entity, bool_t loop)
{
  if (entity->getType() == ESNT_ANIMATED_MESH)
    ((IAnimatedMeshSceneNode *)entity)->setLoopMode(loop);
}

bool_t MeshEntityLoop(IMeshSceneNode *entity)
{
  return (entity->getType() == ESNT_ANIMATED_MESH)
             ? ((IAnimatedMeshSceneNode *)entity)->getLoopMode()
             : false;
}

void SetMeshEntityFPS(IMeshSceneNode *entity, float fps)
{
  if (entity->getType() == ESNT_ANIMATED_MESH)
    ((IAnimatedMeshSceneNode *)entity)->setAnimationSpeed(fps);
}

float MeshEntityFPS(IMeshSceneNode *entity)
{
  return (entity->getType() == ESNT_ANIMATED_MESH)
             ? ((IAnimatedMeshSceneNode *)entity)->getAnimationSpeed()
             : 0;
}

void SetMeshEntityFrame(IMeshSceneNode *entity, float frame)
{
  if (entity->getType() == ESNT_ANIMATED_MESH)
    ((IAnimatedMeshSceneNode *)entity)->setCurrentFrame(frame);
}

float MeshEntityFrame(IMeshSceneNode *entity)
{
  return (entity->getType() == ESNT_ANIMATED_MESH)
             ? ((IAnimatedMeshSceneNode *)entity)->getFrameNr()
             : -1;
}

void SetMeshEntityFrames(IMeshSceneNode *entity, int first, int last)
{
  if (entity->getType() == ESNT_ANIMATED_MESH)
    ((IAnimatedMeshSceneNode *)entity)->setFrameLoop(first, last);
}

int MeshEntityFirstFrame(IMeshSceneNode *entity)
{
  return (entity->getType() == ESNT_ANIMATED_MESH)
             ? ((IAnimatedMeshSceneNode *)entity)->getStartFrame()
             : -1;
}

int MeshEntityLastFrame(IMeshSceneNode *entity)
{
  return (entity->getType() == ESNT_ANIMATED_MESH)
             ? ((IAnimatedMeshSceneNode *)entity)->getEndFrame()
             : -1;
}
