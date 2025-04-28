#include "color.h"
#include "surface.h"

static int _surfaceCreationFlags = 0;

extern "C"
{

  EXPORT IMeshBuffer *CALL AddSurface(IMesh *mesh)
  {
    IMeshBuffer *buffer = (_surfaceCreationFlags & SURFACE_2TEXCOORDS)
      ? (IMeshBuffer *)(new SMeshBufferLightMap())
      : (IMeshBuffer *)(new SMeshBuffer());
    ((SMesh *)mesh)->addMeshBuffer(buffer);
    buffer->drop();
    return buffer;
  }

  EXPORT int CALL AddIndex(IMeshBuffer *surface, int vertexIndex)
  {
    --vertexIndex;
    switch (surface->getVertexType())
    {
    case EVT_STANDARD:
      ((SMeshBuffer*)surface)->Indices.push_back(vertexIndex);
      break;
    case EVT_2TCOORDS:
      ((SMeshBufferLightMap*)surface)->Indices.push_back(vertexIndex);
      break;
    case EVT_TANGENTS:
      ((SMeshBufferTangents*)surface)->Indices.push_back(vertexIndex);
      break;
    }
    return NumIndices(surface);
  }

  EXPORT int CALL NumIndices(IMeshBuffer *surface)
  {
    return surface->getIndexCount();
  }

  EXPORT int CALL SurfaceIndex(IMeshBuffer *surface, int number)
  {
    return (surface->getIndexType() == EIT_16BIT)
               ? (surface->getIndices()[number - 1] + 1)
               : (((u32 *)surface->getIndices())[number - 1] + 1);
  }

  EXPORT int CALL AddVertex(IMeshBuffer *surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v)
  {
    switch (surface->getVertexType())
    {
    case EVT_STANDARD:
      ((SMeshBuffer*)surface)->Vertices.push_back(S3DVertex(x, y, z, nx, ny, nz, _Color(color), u, v));
      break;
    case EVT_2TCOORDS:
      ((SMeshBuffer*)surface)->Vertices.push_back(S3DVertex2TCoords(x, y, z, nx, ny, nz, _Color(color), u, v));
      break;
    case EVT_TANGENTS:
      ((SMeshBuffer*)surface)->Vertices.push_back(S3DVertexTangents(x, y, z, nx, ny, nz, _Color(color), u, v));
      break;
    }
    return NumVertices(surface);
  }

  EXPORT int CALL NumVertices(IMeshBuffer *surface)
  {
    return surface->getVertexCount();
  }

  EXPORT float CALL VertexX(IMeshBuffer *surface, int index)
  {
    return surface->getPosition(index - 1).X;
  }

  EXPORT float CALL VertexY(IMeshBuffer *surface, int index)
  {
    return surface->getPosition(index - 1).Y;
  }

  EXPORT float CALL VertexZ(IMeshBuffer *surface, int index)
  {
    return surface->getPosition(index - 1).Z;
  }

  EXPORT float CALL VertexNX(IMeshBuffer *surface, int index)
  {
    return surface->getNormal(index - 1).X;
  }

  EXPORT float CALL VertexNY(IMeshBuffer *surface, int index)
  {
    return surface->getNormal(index - 1).Y;
  }

  EXPORT float CALL VertexNZ(IMeshBuffer *surface, int index)
  {
    return surface->getNormal(index - 1).Z;
  }

  EXPORT int CALL VertexColor(IMeshBuffer *surface, int index)
  {
    switch (surface->getVertexType())
    {
    case EVT_STANDARD:
      return _IntColor(((S3DVertex *)surface->getVertices())[index - 1].Color);
    case EVT_2TCOORDS:
      return _IntColor(((S3DVertex2TCoords *)surface->getVertices())[index - 1].Color);
    case EVT_TANGENTS:
      return _IntColor(((S3DVertexTangents *)surface->getVertices())[index - 1].Color);
    }
    return 0;
  }

  EXPORT void CALL SetVertexTexCoords(IMeshBuffer *surface, int index, float u, float v, int set)
  {
    if (set == 0 || (surface->getVertexType() != EVT_2TCOORDS))
    {
      surface->getTCoords(index - 1) = vector2df(u, v);
    }
    else
    {
      ((S3DVertex2TCoords *)surface->getVertices())[index - 1].TCoords2 = vector2df(u, v);
    }
  }

  EXPORT float CALL VertexU(IMeshBuffer *surface, int index, int set)
  {
    if (set == 0 || (surface->getVertexType() != EVT_2TCOORDS))
    {
      return surface->getTCoords(index - 1).X;
    }
    else
    {
      return ((S3DVertex2TCoords *)surface->getVertices())[index - 1].TCoords2.X;
    }
  }

  EXPORT float CALL VertexV(IMeshBuffer *surface, int index, int set)
  {
    if (set == 0 || (surface->getVertexType() != EVT_2TCOORDS))
    {
      return surface->getTCoords(index - 1).Y;
    }
    else
    {
      return ((S3DVertex2TCoords *)surface->getVertices())[index - 1].TCoords2.Y;
    }
  }

  EXPORT SMaterial *CALL SurfaceMaterial(IMeshBuffer *surface)
  {
    return &surface->getMaterial();
  }

  EXPORT void *CALL SetSurfaceCreationFlags(int flags)
  {
    _surfaceCreationFlags = flags;
  }

} // extern "C"
