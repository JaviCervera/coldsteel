#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMesh* CALL csCreateCone(int segments);
EXPORT IMesh* CALL csCreateCube();
EXPORT IMesh* CALL csCreateCylinder(int segments);
EXPORT IMesh* CALL csCreateMesh();
EXPORT IMesh* CALL csCreateQuad();
EXPORT IMesh* CALL csCreateSphere(int segments);
EXPORT IMesh* CALL csLoadMesh(const char* filename);
EXPORT void CALL csFreeMesh(IMesh* mesh);
EXPORT void CALL csSetMeshFPS(IMesh* mesh, float fps);
EXPORT float CALL csMeshFPS(IMesh* mesh);
EXPORT int CALL csMeshFrames(IMesh* mesh);
EXPORT IMesh* CALL csMeshForFrame(IMesh* mesh, int frame);
EXPORT int CALL csNumSurfaces(IMesh* mesh);
EXPORT IMeshBuffer* CALL csMeshSurface(IMesh* mesh, int index);
EXPORT void CALL csUpdateMesh(IMesh* mesh);
EXPORT void CALL csTranslateMesh(IMesh* mesh, float x, float y, float z);
EXPORT void CALL csRotateMesh(IMesh* mesh, float pitch, float yaw, float roll);
EXPORT void CALL csScaleMesh(IMesh* mesh, float x, float y, float z);
EXPORT void CALL csFlipMesh(IMesh* mesh);
EXPORT void CALL csSetMeshColor(IMesh* mesh, int color);
EXPORT void CALL csUpdateMeshNormals(IMesh* mesh);
EXPORT float CALL csMeshWidth(IMesh* mesh);
EXPORT float CALL csMeshHeight(IMesh* mesh);
EXPORT float CALL csMeshDepth(IMesh* mesh);


bool_t _csMeshAnimated(IMesh* mesh);


#ifdef __cplusplus
} /* extern "C" */
#endif
