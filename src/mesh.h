/** @file */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  EXPORT IMesh *CALL CreateConeMesh(int segments);
  EXPORT IMesh *CALL CreateCubeMesh();
  EXPORT IMesh *CALL CreateCylinderMesh(int segments);
  EXPORT IMesh *CALL CreateMesh();
  EXPORT IMesh *CALL CreateQuadMesh();
  EXPORT IMesh *CALL CreateSimpleCubeMesh();
  EXPORT IMesh *CALL CreateSphereMesh(int segments);
  EXPORT IMesh *CALL LoadMesh(const char *filename);
  EXPORT IMesh *CALL LoadTerrainMesh(IImage *heightmap, IImage *pixmap, float width, float height, float depth);
  EXPORT void CALL FreeMesh(IMesh *mesh);
  EXPORT void CALL SetMeshFPS(IMesh *mesh, float fps);
  EXPORT float CALL MeshFPS(IMesh *mesh);
  EXPORT int CALL MeshFrames(IMesh *mesh);
  EXPORT IMesh *CALL MeshForFrame(IMesh *mesh, int frame);
  EXPORT int CALL NumSurfaces(IMesh *mesh);
  EXPORT IMeshBuffer *CALL MeshSurface(IMesh *mesh, int index);
  EXPORT void CALL UpdateMesh(IMesh *mesh);
  EXPORT void CALL TranslateMesh(IMesh *mesh, float x, float y, float z);
  EXPORT void CALL RotateMesh(IMesh *mesh, float pitch, float yaw, float roll);
  EXPORT void CALL ScaleMesh(IMesh *mesh, float x, float y, float z);
  EXPORT void CALL FlipMesh(IMesh *mesh);
  EXPORT void CALL SetMeshColor(IMesh *mesh, int color);
  EXPORT void CALL UpdateMeshNormals(IMesh *mesh);
  EXPORT float CALL MeshWidth(IMesh *mesh);
  EXPORT float CALL MeshHeight(IMesh *mesh);
  EXPORT float CALL MeshDepth(IMesh *mesh);

#ifndef SWIG
  bool_t _MeshAnimated(IMesh *mesh);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
