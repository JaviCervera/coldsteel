#pragma once

#include "common.h"

ISceneNode *CreateEntity();
void FreeEntity(ISceneNode *entity);
int EntityType(ISceneNode *entity);
void SetEntityName(ISceneNode *entity, const char *name);
const char *EntityName(ISceneNode *entity);
void SetEntityVisible(ISceneNode *entity, bool_t visible);
bool_t EntityVisible(ISceneNode *entity);
void SetEntityParent(ISceneNode *entity, ISceneNode *parent);
ISceneNode *EntityParent(ISceneNode *entity);
int EntityNumChildren(ISceneNode *entity);
ISceneNode *EntityChild(ISceneNode *entity, int index);
void SetEntityPosition(ISceneNode *entity, float x, float y, float z);
void MoveEntity(ISceneNode *entity, float x, float y, float z);
ISceneNode *SlideEntity(ISceneNode *entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group);
float EntityX(ISceneNode *entity);
float EntityY(ISceneNode *entity);
float EntityZ(ISceneNode *entity);
float EntityLocalX(ISceneNode *entity);
float EntityLocalY(ISceneNode *entity);
float EntityLocalZ(ISceneNode *entity);
void SetEntityRotation(ISceneNode *entity, float pitch, float yaw, float roll);
void TurnEntity(ISceneNode *entity, float pitch, float yaw, float roll);
void PointEntity(ISceneNode *entity, float x, float y, float z);
float EntityPitch(ISceneNode *entity);
float EntityYaw(ISceneNode *entity);
float EntityRoll(ISceneNode *entity);
void SetEntityScale(ISceneNode *entity, float x, float y, float z);
float EntityScaleX(ISceneNode *entity);
float EntityScaleY(ISceneNode *entity);
float EntityScaleZ(ISceneNode *entity);
float EntityWidth(ISceneNode *entity);
float EntityHeight(ISceneNode *entity);
float EntityDepth(ISceneNode *entity);
int EntityNumMaterials(ISceneNode *entity);
SMaterial *EntityMaterial(ISceneNode *entity, int index);
void SetEntityCollision(ISceneNode *entity, int type, int group);
bool_t EntityCollision(ISceneNode *entity);
int EntityGroup(ISceneNode *entity);
float EntityDistance(ISceneNode *entity, float x, float y, float z);
float EntityDistanceSquare(ISceneNode *entity, float x, float y, float z);
