#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMeshBuffer* CALL AddSurface(IMesh* mesh);
EXPORT int CALL AddIndex(IMeshBuffer* surface, int index);
EXPORT int CALL NumIndices(IMeshBuffer* surface);
EXPORT int CALL SurfaceIndex(IMeshBuffer* surface, int number);
EXPORT int CALL AddVertex(IMeshBuffer* surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v);
EXPORT int CALL NumVertices(IMeshBuffer* surface);
EXPORT float CALL VertexX(IMeshBuffer* surface, int index);
EXPORT float CALL VertexY(IMeshBuffer* surface, int index);
EXPORT float CALL VertexZ(IMeshBuffer* surface, int index);
EXPORT float CALL VertexNX(IMeshBuffer* surface, int index);
EXPORT float CALL VertexNY(IMeshBuffer* surface, int index);
EXPORT float CALL VertexNZ(IMeshBuffer* surface, int index);
EXPORT int CALL VertexColor(IMeshBuffer* surface, int index);
EXPORT void CALL SetVertexTexCoords(IMeshBuffer* surface, int index, float u, float v, int set);
EXPORT float CALL VertexU(IMeshBuffer* surface, int index, int set);
EXPORT float CALL VertexV(IMeshBuffer* surface, int index, int set);
EXPORT SMaterial* CALL SurfaceMaterial(IMeshBuffer* surface);


#ifdef __cplusplus
} /* extern "C" */
#endif
