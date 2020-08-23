#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMeshSceneNode* CALL csCreateMeshEntity(IMesh* mesh);
EXPORT IMeshSceneNode* CALL csCreateOctreeMeshEntity(IMesh* mesh);
EXPORT IMesh* CALL csMeshEntityMesh(IMeshSceneNode* entity);
EXPORT void CALL csSetMeshEntityCastShadows(IMeshSceneNode* entity, bool_t enable);
EXPORT bool_t CALL csMeshEntityCastShadows(IMeshSceneNode* entity);
EXPORT void CALL csSetMeshEntityLoop(IMeshSceneNode* entity, bool_t loop);
EXPORT bool_t CALL csMeshEntityLoop(IMeshSceneNode* entity);
EXPORT void CALL csSetMeshEntityFPS(IMeshSceneNode* entity, float fps);
EXPORT float CALL csMeshEntityFPS(IMeshSceneNode* entity);
EXPORT void CALL csSetMeshEntityFrame(IMeshSceneNode* entity, float frame);
EXPORT float CALL csMeshEntityFrame(IMeshSceneNode* entity);
EXPORT void CALL csSetMeshEntityFrames(IMeshSceneNode* entity, int first, int last);
EXPORT int CALL csMeshEntityFirstFrame(IMeshSceneNode* entity);
EXPORT int CALL csMeshEntityLastFrame(IMeshSceneNode* entity);


#ifdef __cplusplus
} /* extern "C" */
#endif
