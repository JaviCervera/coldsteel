#include "graphics.h"
#include "surface.h"

IMeshBuffer *AddSurface(IMesh *mesh)
{
  IMeshBuffer *buffer = new SMeshBufferLightMap();
  ((SMesh *)mesh)->addMeshBuffer(buffer);
  buffer->drop();
  return buffer;
}

int AddIndex(IMeshBuffer *surface, int index)
{
  int lastIndex = NumIndices(surface);
  surface->append(NULL, 0, (const u16 *)&index, 1);
  surface->getIndices()[lastIndex] = index;
  return lastIndex;
}

int NumIndices(IMeshBuffer *surface)
{
  return surface->getIndexCount();
}

int SurfaceIndex(IMeshBuffer *surface, int number)
{
  return (surface->getIndexType() == EIT_16BIT)
             ? surface->getIndices()[number]
             : ((u32 *)surface->getIndices())[number];
}

int AddVertex(IMeshBuffer *surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v)
{
  switch (surface->getVertexType())
  {
  case EVT_STANDARD:
  {
    S3DVertex vertex(x, y, z, nx, ny, nz, _Color(color), u, v);
    surface->append(&vertex, 1, NULL, 0);
    break;
  }
  case EVT_2TCOORDS:
  {
    S3DVertex2TCoords vertex(x, y, z, nx, ny, nz, _Color(color), u, v);
    surface->append(&vertex, 1, NULL, 0);
    break;
  }
  case EVT_TANGENTS:
  {
    S3DVertexTangents vertex(x, y, z, nx, ny, nz, _Color(color), u, v);
    surface->append(&vertex, 1, NULL, 0);
    break;
  }
  }
  return NumVertices(surface) - 1;
}

int NumVertices(IMeshBuffer *surface)
{
  return surface->getVertexCount();
}

float VertexX(IMeshBuffer *surface, int index)
{
  return surface->getPosition(index).X;
}

float VertexY(IMeshBuffer *surface, int index)
{
  return surface->getPosition(index).Y;
}

float VertexZ(IMeshBuffer *surface, int index)
{
  return surface->getPosition(index).Z;
}

float VertexNX(IMeshBuffer *surface, int index)
{
  return surface->getNormal(index).X;
}

float VertexNY(IMeshBuffer *surface, int index)
{
  return surface->getNormal(index).Y;
}

float VertexNZ(IMeshBuffer *surface, int index)
{
  return surface->getNormal(index).Z;
}

int VertexColor(IMeshBuffer *surface, int index)
{
  switch (surface->getVertexType())
  {
  case EVT_STANDARD:
    return _IntColor(((S3DVertex *)surface->getVertices())[index].Color);
  case EVT_2TCOORDS:
    return _IntColor(((S3DVertex2TCoords *)surface->getVertices())[index].Color);
  case EVT_TANGENTS:
    return _IntColor(((S3DVertexTangents *)surface->getVertices())[index].Color);
  }
  return 0;
}

void SetVertexTexCoords(IMeshBuffer *surface, int index, float u, float v, int set)
{
  if (set == 0 || (surface->getVertexType() != EVT_2TCOORDS))
  {
    surface->getTCoords(index) = vector2df(u, v);
  }
  else
  {
    ((S3DVertex2TCoords *)surface->getVertices())[index].TCoords2 = vector2df(u, v);
  }
}

float VertexU(IMeshBuffer *surface, int index, int set)
{
  if (set == 0 || (surface->getVertexType() != EVT_2TCOORDS))
  {
    return surface->getTCoords(index).X;
  }
  else
  {
    return ((S3DVertex2TCoords *)surface->getVertices())[index].TCoords2.X;
  }
}

float VertexV(IMeshBuffer *surface, int index, int set)
{
  if (set == 0 || (surface->getVertexType() != EVT_2TCOORDS))
  {
    return surface->getTCoords(index).Y;
  }
  else
  {
    return ((S3DVertex2TCoords *)surface->getVertices())[index].TCoords2.Y;
  }
}

SMaterial *SurfaceMaterial(IMeshBuffer *surface)
{
  return &surface->getMaterial();
}
