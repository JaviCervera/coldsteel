#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMeshBuffer* CALL asAddSurface(IMesh* mesh);
EXPORT int CALL asAddIndex(IMeshBuffer* surface, int index);
EXPORT int CALL asNumIndices(IMeshBuffer* surface);
EXPORT int CALL asSurfaceIndex(IMeshBuffer* surface, int number);
EXPORT int CALL asAddVertex(IMeshBuffer* surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v);
EXPORT int CALL asNumVertices(IMeshBuffer* surface);
EXPORT float CALL asVertexX(IMeshBuffer* surface, int index);
EXPORT float CALL asVertexY(IMeshBuffer* surface, int index);
EXPORT float CALL asVertexZ(IMeshBuffer* surface, int index);
EXPORT float CALL asVertexNX(IMeshBuffer* surface, int index);
EXPORT float CALL asVertexNY(IMeshBuffer* surface, int index);
EXPORT float CALL asVertexNZ(IMeshBuffer* surface, int index);
EXPORT int CALL asVertexColor(IMeshBuffer* surface, int index);
EXPORT float CALL asVertexU(IMeshBuffer* surface, int index);
EXPORT float CALL asVertexV(IMeshBuffer* surface, int index);


#ifdef __cplusplus
} /* extern "C" */
#endif
