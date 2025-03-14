#include "graphics.h"
#include "surface.h"

extern "C"
{

  EXPORT IMeshBuffer *CALL AddSurface(IMesh *mesh)
  {
    IMeshBuffer *buffer = new SMeshBufferLightMap();
    ((SMesh *)mesh)->addMeshBuffer(buffer);
    buffer->drop();
    return buffer;
  }

  EXPORT int CALL AddIndex(IMeshBuffer *surface, int vertexIndex)
  {
    --vertexIndex;
    int lastIndex = NumIndices(surface);
    surface->append(NULL, 0, (const u16 *)&vertexIndex, 1);
    surface->getIndices()[lastIndex] = vertexIndex;
    return lastIndex + 1;
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

} // extern "C"
