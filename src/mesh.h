#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMesh* CALL asCreateCone(int segments);
EXPORT IMesh* CALL asCreateCube();
EXPORT IMesh* CALL asCreateCylinder(int segments);
EXPORT IMesh* CALL asCreateMesh();
EXPORT IMesh* CALL asCreateQuad();
EXPORT IMesh* CALL asCreateSphere(int segments);
EXPORT IMesh* CALL asLoadMesh(const char* filename);
EXPORT void CALL asFreeMesh(IMesh* mesh);
EXPORT void CALL asSetMeshFPS(IMesh* mesh, float fps);
EXPORT float CALL asMeshFPS(IMesh* mesh);
EXPORT int CALL asMeshFrames(IMesh* mesh);
EXPORT IMesh* CALL asMeshForFrame(IMesh* mesh, int frame);
EXPORT void CALL asTranslateMesh(IMesh* mesh, float x, float y, float z);
EXPORT void CALL asRotateMesh(IMesh* mesh, float pitch, float yaw, float roll);
EXPORT void CALL asScaleMesh(IMesh* mesh, float x, float y, float z);
EXPORT void CALL asFlipMesh(IMesh* mesh);
EXPORT void CALL asSetMeshColor(IMesh* mesh, int color);
EXPORT void CALL asUpdateMeshNormals(IMesh* mesh);
EXPORT float CALL asMeshWidth(IMesh* mesh);
EXPORT float CALL asMeshHeight(IMesh* mesh);
EXPORT float CALL asMeshDepth(IMesh* mesh);


bool_t _asMeshAnimated(IMesh* mesh);


#ifdef __cplusplus
} /* extern "C" */
#endif
