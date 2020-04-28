#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMeshSceneNode* CALL asCreateMeshEntity(IMesh* mesh);
EXPORT IMeshSceneNode* CALL asCreateOctreeMeshEntity(IMesh* mesh);
EXPORT IMesh* CALL asMeshEntityMesh(IMeshSceneNode* entity);
EXPORT void CALL asSetMeshEntityCastShadows(IMeshSceneNode* entity, bool_t enable);
EXPORT bool_t CALL asMeshEntityCastShadows(IMeshSceneNode* entity);
EXPORT void CALL asSetMeshEntityLoop(IMeshSceneNode* entity, bool_t loop);
EXPORT bool_t CALL asMeshEntityLoop(IMeshSceneNode* entity);
EXPORT void CALL asSetMeshEntityFPS(IMeshSceneNode* entity, float fps);
EXPORT float CALL asMeshEntityFPS(IMeshSceneNode* entity);
EXPORT void CALL asSetMeshEntityFrame(IMeshSceneNode* entity, float frame);
EXPORT float CALL asMeshEntityFrame(IMeshSceneNode* entity);
EXPORT void CALL asSetMeshEntityFrames(IMeshSceneNode* entity, int first, int last);
EXPORT int CALL asMeshEntityFirstFrame(IMeshSceneNode* entity);
EXPORT int CALL asMeshEntityLastFrame(IMeshSceneNode* entity);


#ifdef __cplusplus
} /* extern "C" */
#endif
