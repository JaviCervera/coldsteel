/** @file */
#pragma once

#include "common.h"

// Collision types
#define COLLISION_NONE 0
#define COLLISION_BOX 1
#define COLLISION_MESH 2
#define COLLISION_OCTREE 3

// Entity types
#define ENTITY_EMPTY 0
#define ENTITY_SPRITE 1
#define ENTITY_CAMERA 2
#define ENTITY_EMITTER 3
#define ENTITY_LIGHT 4
#define ENTITY_MESH 5
#define ENTITY_MESH_OCTREE 6
#define ENTITY_TERRAIN 7
#define ENTITY_WATER 8

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT ISceneNode *CALL CreateEntity();
  EXPORT void CALL FreeEntity(ISceneNode *entity);
  EXPORT int CALL EntityType(ISceneNode *entity);
  EXPORT void CALL SetEntityName(ISceneNode *entity, const char *name);
  EXPORT const char *CALL EntityName(ISceneNode *entity);
  EXPORT void CALL SetEntityVisible(ISceneNode *entity, bool_t visible);
  EXPORT bool_t CALL EntityVisible(ISceneNode *entity);
  EXPORT void CALL SetEntityParent(ISceneNode *entity, ISceneNode *parent);
  EXPORT ISceneNode *CALL EntityParent(ISceneNode *entity);
  EXPORT int CALL EntityNumChildren(ISceneNode *entity);
  EXPORT ISceneNode *CALL EntityChild(ISceneNode *entity, int index);
  EXPORT void CALL SetEntityPosition(ISceneNode *entity, float x, float y, float z);
  EXPORT void CALL MoveEntity(ISceneNode *entity, float x, float y, float z);
  EXPORT ISceneNode *CALL SlideEntity(ISceneNode *entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group);
  EXPORT void CALL TranslateEntity(ISceneNode *entity, float x, float y, float z);
  EXPORT float CALL EntityX(ISceneNode *entity);
  EXPORT float CALL EntityY(ISceneNode *entity);
  EXPORT float CALL EntityZ(ISceneNode *entity);
  EXPORT float CALL EntityLocalX(ISceneNode *entity);
  EXPORT float CALL EntityLocalY(ISceneNode *entity);
  EXPORT float CALL EntityLocalZ(ISceneNode *entity);
  EXPORT void CALL SetEntityRotation(ISceneNode *entity, float pitch, float yaw, float roll);
  EXPORT void CALL TurnEntity(ISceneNode *entity, float pitch, float yaw, float roll);
  EXPORT void CALL PointEntity(ISceneNode *entity, float x, float y, float z);
  EXPORT float CALL EntityPitch(ISceneNode *entity);
  EXPORT float CALL EntityYaw(ISceneNode *entity);
  EXPORT float CALL EntityRoll(ISceneNode *entity);
  EXPORT void CALL SetEntityScale(ISceneNode *entity, float x, float y, float z);
  EXPORT float CALL EntityScaleX(ISceneNode *entity);
  EXPORT float CALL EntityScaleY(ISceneNode *entity);
  EXPORT float CALL EntityScaleZ(ISceneNode *entity);
  EXPORT float CALL EntityWidth(ISceneNode *entity);
  EXPORT float CALL EntityHeight(ISceneNode *entity);
  EXPORT float CALL EntityDepth(ISceneNode *entity);
  EXPORT int CALL EntityNumMaterials(ISceneNode *entity);
  EXPORT SMaterial *CALL EntityMaterial(ISceneNode *entity, int index);
  EXPORT void CALL SetEntityCollision(ISceneNode *entity, int type, int group);
  EXPORT bool_t CALL EntityCollision(ISceneNode *entity);
  EXPORT int CALL EntityGroup(ISceneNode *entity);
  EXPORT float CALL EntityDistance(ISceneNode *entity, float x, float y, float z);
  EXPORT float CALL EntitySquareDistance(ISceneNode *entity, float x, float y, float z);

#ifdef __cplusplus
} /* extern "C" */
#endif
