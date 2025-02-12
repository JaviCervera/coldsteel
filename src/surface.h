#pragma once

#include "common.h"

IMeshBuffer *AddSurface(IMesh *mesh);
int AddIndex(IMeshBuffer *surface, int index);
int NumIndices(IMeshBuffer *surface);
int SurfaceIndex(IMeshBuffer *surface, int number);
int AddVertex(IMeshBuffer *surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v);
int NumVertices(IMeshBuffer *surface);
float VertexX(IMeshBuffer *surface, int index);
float VertexY(IMeshBuffer *surface, int index);
float VertexZ(IMeshBuffer *surface, int index);
float VertexNX(IMeshBuffer *surface, int index);
float VertexNY(IMeshBuffer *surface, int index);
float VertexNZ(IMeshBuffer *surface, int index);
int VertexColor(IMeshBuffer *surface, int index);
void SetVertexTexCoords(IMeshBuffer *surface, int index, float u, float v, int set);
float VertexU(IMeshBuffer *surface, int index, int set);
float VertexV(IMeshBuffer *surface, int index, int set);
SMaterial *SurfaceMaterial(IMeshBuffer *surface);
