#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ISceneNode* CALL csCreateEntity();
EXPORT void CALL csFreeEntity(ISceneNode* entity);
EXPORT int CALL csEntityType(ISceneNode* entity);
EXPORT void CALL csSetEntityName(ISceneNode* entity, const char* name);
EXPORT const char* CALL csEntityName(ISceneNode* entity);
EXPORT void CALL csSetEntityVisible(ISceneNode* entity, bool_t visible);
EXPORT bool_t CALL csEntityVisible(ISceneNode* entity);
EXPORT void CALL csSetEntityParent(ISceneNode* entity, ISceneNode* parent);
EXPORT ISceneNode* CALL csEntityParent(ISceneNode* entity);
EXPORT int CALL csEntityNumChildren(ISceneNode* entity);
EXPORT ISceneNode* CALL csEntityChild(ISceneNode* entity, int index);
EXPORT void CALL csSetEntityPosition(ISceneNode* entity, float x, float y, float z);
EXPORT void CALL csMoveEntity(ISceneNode* entity, float x, float y, float z);
EXPORT ISceneNode* CALL csSlideEntity(ISceneNode* entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group);
EXPORT float CALL csEntityX(ISceneNode* entity);
EXPORT float CALL csEntityY(ISceneNode* entity);
EXPORT float CALL csEntityZ(ISceneNode* entity);
EXPORT float CALL csEntityLocalX(ISceneNode* entity);
EXPORT float CALL csEntityLocalY(ISceneNode* entity);
EXPORT float CALL csEntityLocalZ(ISceneNode* entity);
EXPORT void CALL csSetEntityRotation(ISceneNode* entity, float pitch, float yaw, float roll);
EXPORT void CALL csTurnEntity(ISceneNode* entity, float pitch, float yaw, float roll);
EXPORT void CALL csPointEntity(ISceneNode* entity, float x, float y, float z);
EXPORT float CALL csEntityPitch(ISceneNode* entity);
EXPORT float CALL csEntityYaw(ISceneNode* entity);
EXPORT float CALL csEntityRoll(ISceneNode* entity);
EXPORT void CALL csSetEntityScale(ISceneNode* entity, float x, float y, float z);
EXPORT float CALL csEntityScaleX(ISceneNode* entity);
EXPORT float CALL csEntityScaleY(ISceneNode* entity);
EXPORT float CALL csEntityScaleZ(ISceneNode* entity);
EXPORT float CALL csEntityWidth(ISceneNode* entity);
EXPORT float CALL csEntityHeight(ISceneNode* entity);
EXPORT float CALL csEntityDepth(ISceneNode* entity);
EXPORT int CALL csEntityNumMaterials(ISceneNode* entity);
EXPORT SMaterial* CALL csEntityMaterial(ISceneNode* entity, int index);
EXPORT void CALL csSetEntityCollision(ISceneNode* entity, int type, int group);
EXPORT bool_t CALL csEntityCollision(ISceneNode* entity);
EXPORT int CALL csEntityGroup(ISceneNode* entity);


#ifdef __cplusplus
} /* extern "C" */
#endif
