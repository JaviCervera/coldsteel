#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMeshSceneNode* CALL CreateMeshEntity(IMesh* mesh);
EXPORT IMeshSceneNode* CALL CreateOctreeMeshEntity(IMesh* mesh);
EXPORT IMesh* CALL MeshEntityMesh(IMeshSceneNode* entity);
EXPORT void CALL SetMeshEntityCastShadows(IMeshSceneNode* entity, bool_t enable);
EXPORT bool_t CALL MeshEntityCastShadows(IMeshSceneNode* entity);
EXPORT void CALL SetMeshEntityLoop(IMeshSceneNode* entity, bool_t loop);
EXPORT bool_t CALL MeshEntityLoop(IMeshSceneNode* entity);
EXPORT void CALL SetMeshEntityFPS(IMeshSceneNode* entity, float fps);
EXPORT float CALL MeshEntityFPS(IMeshSceneNode* entity);
EXPORT void CALL SetMeshEntityFrame(IMeshSceneNode* entity, float frame);
EXPORT float CALL MeshEntityFrame(IMeshSceneNode* entity);
EXPORT void CALL SetMeshEntityFrames(IMeshSceneNode* entity, int first, int last);
EXPORT int CALL MeshEntityFirstFrame(IMeshSceneNode* entity);
EXPORT int CALL MeshEntityLastFrame(IMeshSceneNode* entity);


#ifdef __cplusplus
} /* extern "C" */
#endif
