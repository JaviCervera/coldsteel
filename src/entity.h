#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT ISceneNode* CALL asCreateEntity();
EXPORT void CALL asDeleteEntity(ISceneNode* entity);
EXPORT int CALL asEntityType(ISceneNode* entity);
EXPORT void CALL asSetEntityName(ISceneNode* entity, const char* name);
EXPORT const char* CALL asEntityName(ISceneNode* entity);
EXPORT void CALL asSetEntityVisible(ISceneNode* entity, bool_t visible);
EXPORT bool_t CALL asEntityVisible(ISceneNode* entity);
EXPORT void CALL asSetEntityParent(ISceneNode* entity, ISceneNode* parent);
EXPORT ISceneNode* CALL asEntityParent(ISceneNode* entity);
EXPORT int CALL asEntityNumChildren(ISceneNode* entity);
EXPORT ISceneNode* CALL asEntityChild(ISceneNode* entity, int index);
EXPORT void CALL asSetEntityPosition(ISceneNode* entity, float x, float y, float z);
EXPORT void CALL asMoveEntity(ISceneNode* entity, float x, float y, float z);
EXPORT ISceneNode* CALL asSlideEntity(ISceneNode* entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group);
EXPORT float CALL asEntityX(ISceneNode* entity);
EXPORT float CALL asEntityY(ISceneNode* entity);
EXPORT float CALL asEntityZ(ISceneNode* entity);
EXPORT float CALL asEntityLocalX(ISceneNode* entity);
EXPORT float CALL asEntityLocalY(ISceneNode* entity);
EXPORT float CALL asEntityLocalZ(ISceneNode* entity);
EXPORT void CALL asSetEntityRotation(ISceneNode* entity, float pitch, float yaw, float roll);
EXPORT void CALL asTurnEntity(ISceneNode* entity, float pitch, float yaw, float roll);
EXPORT void CALL asPointEntity(ISceneNode* entity, float x, float y, float z);
EXPORT float CALL asEntityPitch(ISceneNode* entity);
EXPORT float CALL asEntityYaw(ISceneNode* entity);
EXPORT float CALL asEntityRoll(ISceneNode* entity);
EXPORT void CALL asSetEntityScale(ISceneNode* entity, float x, float y, float z);
EXPORT float CALL asEntityScaleX(ISceneNode* entity);
EXPORT float CALL asEntityScaleY(ISceneNode* entity);
EXPORT float CALL asEntityScaleZ(ISceneNode* entity);
EXPORT float CALL asEntityWidth(ISceneNode* entity);
EXPORT float CALL asEntityHeight(ISceneNode* entity);
EXPORT float CALL asEntityDepth(ISceneNode* entity);
EXPORT int CALL asEntityNumMaterials(ISceneNode* entity);
EXPORT SMaterial* CALL asEntityMaterial(ISceneNode* entity, int index);
EXPORT void CALL asSetEntityCollision(ISceneNode* entity, int type, int group);
EXPORT bool_t CALL asEntityCollision(ISceneNode* entity);
EXPORT int CALL asEntityGroup(ISceneNode* entity);


#ifdef __cplusplus
} /* extern "C" */
#endif
