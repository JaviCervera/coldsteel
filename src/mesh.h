#pragma once

#include "common.h"

IMesh *CreateCone(int segments);
IMesh *CreateCube();
IMesh *CreateCylinder(int segments);
IMesh *CreateMesh();
IMesh *CreateQuad();
IMesh *CreateSphere(int segments);
IMesh *LoadMesh(const char *filename);
void FreeMesh(IMesh *mesh);
void SetMeshFPS(IMesh *mesh, float fps);
float MeshFPS(IMesh *mesh);
int MeshFrames(IMesh *mesh);
IMesh *MeshForFrame(IMesh *mesh, int frame);
int NumSurfaces(IMesh *mesh);
IMeshBuffer *MeshSurface(IMesh *mesh, int index);
void UpdateMesh(IMesh *mesh);
void TranslateMesh(IMesh *mesh, float x, float y, float z);
void RotateMesh(IMesh *mesh, float pitch, float yaw, float roll);
void ScaleMesh(IMesh *mesh, float x, float y, float z);
void FlipMesh(IMesh *mesh);
void SetMeshColor(IMesh *mesh, int color);
void UpdateMeshNormals(IMesh *mesh);
float MeshWidth(IMesh *mesh);
float MeshHeight(IMesh *mesh);
float MeshDepth(IMesh *mesh);

bool_t _MeshAnimated(IMesh *mesh);
