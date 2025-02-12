#pragma once

#include "common.h"

IMeshSceneNode *CreateMeshEntity(IMesh *mesh);
IMeshSceneNode *CreateOctreeMeshEntity(IMesh *mesh);
IMesh *MeshEntityMesh(IMeshSceneNode *entity);
void SetMeshEntityCastShadows(IMeshSceneNode *entity, bool_t enable);
bool_t MeshEntityCastShadows(IMeshSceneNode *entity);
void SetMeshEntityLoop(IMeshSceneNode *entity, bool_t loop);
bool_t MeshEntityLoop(IMeshSceneNode *entity);
void SetMeshEntityFPS(IMeshSceneNode *entity, float fps);
float MeshEntityFPS(IMeshSceneNode *entity);
void SetMeshEntityFrame(IMeshSceneNode *entity, float frame);
float MeshEntityFrame(IMeshSceneNode *entity);
void SetMeshEntityFrames(IMeshSceneNode *entity, int first, int last);
int MeshEntityFirstFrame(IMeshSceneNode *entity);
int MeshEntityLastFrame(IMeshSceneNode *entity);
