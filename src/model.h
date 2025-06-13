/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT IMeshSceneNode *CALL CreateConeModel(int segments);
  EXPORT IMeshSceneNode *CALL CreateCubeModel();
  EXPORT IMeshSceneNode *CALL CreateCylinderModel(int segments);
  EXPORT IMeshSceneNode *CALL CreateModel(IMesh *mesh);
  EXPORT IMeshSceneNode *CALL CreateOctreeModel(IMesh *mesh);
  EXPORT IMeshSceneNode *CALL CreateQuadModel();
  EXPORT IMeshSceneNode *CALL CreateSphereModel(int segments);
  EXPORT IMeshSceneNode *CALL LoadModel(const char *filename);
  EXPORT IMeshSceneNode *CALL LoadOctreeModel(const char *filename);
  EXPORT IMesh *CALL ModelMesh(IMeshSceneNode *entity);
  EXPORT void CALL SetModelCastShadows(IMeshSceneNode *entity, bool_t enable);
  EXPORT bool_t CALL ModelCastShadows(IMeshSceneNode *entity);
  EXPORT void CALL SetModelLoop(IMeshSceneNode *entity, bool_t loop);
  EXPORT bool_t CALL ModelLoop(IMeshSceneNode *entity);
  EXPORT void CALL SetModelFPS(IMeshSceneNode *entity, float fps);
  EXPORT float CALL ModelFPS(IMeshSceneNode *entity);
  EXPORT void CALL SetModelFrame(IMeshSceneNode *entity, float frame);
  EXPORT float CALL ModelFrame(IMeshSceneNode *entity);
  EXPORT void CALL SetModelFrames(IMeshSceneNode *entity, int first, int last);
  EXPORT int CALL ModelFirstFrame(IMeshSceneNode *entity);
  EXPORT int CALL ModelLastFrame(IMeshSceneNode *entity);

#ifdef __cplusplus
} /* extern "C" */
#endif
