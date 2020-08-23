#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IMeshBuffer* CALL csAddSurface(IMesh* mesh);
EXPORT int CALL csAddIndex(IMeshBuffer* surface, int index);
EXPORT int CALL csNumIndices(IMeshBuffer* surface);
EXPORT int CALL csSurfaceIndex(IMeshBuffer* surface, int number);
EXPORT int CALL csAddVertex(IMeshBuffer* surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v);
EXPORT int CALL csNumVertices(IMeshBuffer* surface);
EXPORT float CALL csVertexX(IMeshBuffer* surface, int index);
EXPORT float CALL csVertexY(IMeshBuffer* surface, int index);
EXPORT float CALL csVertexZ(IMeshBuffer* surface, int index);
EXPORT float CALL csVertexNX(IMeshBuffer* surface, int index);
EXPORT float CALL csVertexNY(IMeshBuffer* surface, int index);
EXPORT float CALL csVertexNZ(IMeshBuffer* surface, int index);
EXPORT int CALL csVertexColor(IMeshBuffer* surface, int index);
EXPORT float CALL csVertexU(IMeshBuffer* surface, int index);
EXPORT float CALL csVertexV(IMeshBuffer* surface, int index);


#ifdef __cplusplus
} /* extern "C" */
#endif
