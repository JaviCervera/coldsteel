/** @file */
#pragma once

#include "common.h"
#include "memblock.h"

#define SURFACE_STANDARD 0
#define SURFACE_LIGHTMAP 1
#define SURFACE_TANGENTS 2

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT IMeshBuffer *CALL AddSurface(IMesh *mesh, Memblock *vertices, int numVertices, Memblock *indices, int numIndices, int type);
  EXPORT void CALL SetVertices(IMeshBuffer *surface, Memblock *vertices, int numVertices);
  EXPORT int CALL NumVertices(IMeshBuffer *surface);
  EXPORT float CALL VertexX(IMeshBuffer *surface, int index);
  EXPORT float CALL VertexY(IMeshBuffer *surface, int index);
  EXPORT float CALL VertexZ(IMeshBuffer *surface, int index);
  EXPORT float CALL VertexNX(IMeshBuffer *surface, int index);
  EXPORT float CALL VertexNY(IMeshBuffer *surface, int index);
  EXPORT float CALL VertexNZ(IMeshBuffer *surface, int index);
  EXPORT int CALL VertexColor(IMeshBuffer *surface, int index);
  EXPORT float CALL VertexU(IMeshBuffer *surface, int index, int set);
  EXPORT float CALL VertexV(IMeshBuffer *surface, int index, int set);
  EXPORT void CALL SetIndices(IMeshBuffer *surface, Memblock *indices, int numIndices);
  EXPORT int CALL NumIndices(IMeshBuffer *surface);
  EXPORT int CALL SurfaceIndex(IMeshBuffer *surface, int number);
  EXPORT SMaterial *CALL SurfaceMaterial(IMeshBuffer *surface);

#ifdef __cplusplus
} /* extern "C" */
#endif
