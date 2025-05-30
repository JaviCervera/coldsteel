#include "color.h"
#include "surface.h"

#define STD_VERTEX_SIZE 36
#define LIGHTMAP_VERTEX_SIZE 44
#define TANGENTS_VERTEX_SIZE 48

extern "C"
{

  EXPORT IMeshBuffer *CALL AddSurface(IMesh *mesh, Memblock *vertices, int numVertices, Memblock *indices, int numIndices, int type)
  {
    IMeshBuffer *buffer = NULL;
    switch (type)
    {
    case SURFACE_LIGHTMAP:
      buffer = new SMeshBufferLightMap();
      break;
    case SURFACE_TANGENTS:
      buffer = new SMeshBufferTangents();
      break;
    default:
      buffer = new SMeshBuffer();
    }
    SetVertices(buffer, vertices, numVertices);
    SetIndices(buffer, indices, numIndices);
    ((SMesh *)mesh)->addMeshBuffer(buffer);
    buffer->drop();
    return buffer;
  }

  EXPORT void CALL SetVertices(IMeshBuffer *surface, Memblock *vertices, int numVertices)
  {
    switch (surface->getVertexType())
    {
    case EVT_STANDARD:
    {
      SMeshBuffer *buffer = (SMeshBuffer *)surface;
      buffer->Vertices.set_used(numVertices);
      for (int i = 0; i < numVertices; ++i)
      {
        const float x = PeekFloat(vertices, i * STD_VERTEX_SIZE + 0);
        const float y = PeekFloat(vertices, i * STD_VERTEX_SIZE + 4);
        const float z = PeekFloat(vertices, i * STD_VERTEX_SIZE + 8);
        const float nx = PeekFloat(vertices, i * STD_VERTEX_SIZE + 12);
        const float ny = PeekFloat(vertices, i * STD_VERTEX_SIZE + 16);
        const float nz = PeekFloat(vertices, i * STD_VERTEX_SIZE + 20);
        const int color = PeekInt(vertices, i * STD_VERTEX_SIZE + 24);
        const float u = PeekFloat(vertices, i * STD_VERTEX_SIZE + 28);
        const float v = PeekFloat(vertices, i * STD_VERTEX_SIZE + 32);
        buffer->Vertices[i] = S3DVertex(x, y, z, nx, ny, nz, _Color(color), u, v);
      }
      break;
    }
    case EVT_2TCOORDS:
    {
      SMeshBufferLightMap *buffer = (SMeshBufferLightMap *)surface;
      buffer->Vertices.set_used(numVertices);
      for (int i = 0; i < numVertices; ++i)
      {
        const float x = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 0);
        const float y = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 4);
        const float z = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 8);
        const float nx = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 12);
        const float ny = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 16);
        const float nz = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 20);
        const int color = PeekInt(vertices, i * LIGHTMAP_VERTEX_SIZE + 24);
        const float u = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 28);
        const float v = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 32);
        const float u2 = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 36);
        const float v2 = PeekFloat(vertices, i * LIGHTMAP_VERTEX_SIZE + 40);
        buffer->Vertices[i] = S3DVertex2TCoords(x, y, z, nx, ny, nz, _Color(color), u, v, u2, v2);
      }
      break;
    }
    case EVT_TANGENTS:
    {
      SMeshBufferTangents *buffer = (SMeshBufferTangents *)surface;
      buffer->Vertices.set_used(numVertices);
      for (int i = 0; i < numVertices; ++i)
      {
        const float x = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 0);
        const float y = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 4);
        const float z = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 8);
        const float nx = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 12);
        const float ny = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 16);
        const float nz = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 20);
        const int color = PeekInt(vertices, i * TANGENTS_VERTEX_SIZE + 24);
        const float u = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 28);
        const float v = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 32);
        const float tx = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 36);
        const float ty = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 40);
        const float tz = PeekFloat(vertices, i * TANGENTS_VERTEX_SIZE + 44);
        const vector3df bitangent = vector3df(nx, ny, nx).crossProduct(vector3df(tx, ty, tz));
        buffer->Vertices[i] = S3DVertexTangents(x, y, z, nx, ny, nz, _Color(color), u, v, tx, ty, tz, bitangent.X, bitangent.Y, bitangent.Z);
      }
      break;
    }
    }
  }

  EXPORT int CALL NumVertices(IMeshBuffer *surface)
  {
    return surface->getVertexCount();
  }

  EXPORT float CALL VertexX(IMeshBuffer *surface, int index)
  {
    return surface->getPosition(index).X;
  }

  EXPORT float CALL VertexY(IMeshBuffer *surface, int index)
  {
    return surface->getPosition(index).Y;
  }

  EXPORT float CALL VertexZ(IMeshBuffer *surface, int index)
  {
    return surface->getPosition(index).Z;
  }

  EXPORT float CALL VertexNX(IMeshBuffer *surface, int index)
  {
    return surface->getNormal(index).X;
  }

  EXPORT float CALL VertexNY(IMeshBuffer *surface, int index)
  {
    return surface->getNormal(index).Y;
  }

  EXPORT float CALL VertexNZ(IMeshBuffer *surface, int index)
  {
    return surface->getNormal(index).Z;
  }

  EXPORT int CALL VertexColor(IMeshBuffer *surface, int index)
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

  EXPORT float CALL VertexU(IMeshBuffer *surface, int index, int set)
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

  EXPORT float CALL VertexV(IMeshBuffer *surface, int index, int set)
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

  EXPORT void CALL SetIndices(IMeshBuffer *surface, Memblock *indices, int numIndices)
  {
    switch (surface->getVertexType())
    {
    case EVT_STANDARD:
    {
      SMeshBuffer *buffer = (SMeshBuffer *)surface;
      buffer->Indices.set_used(numIndices);
      for (int i = 0; i < numIndices; ++i)
      {
        buffer->Indices[i] = PeekShort(indices, i * sizeof(u16));
      }
      break;
    }
    case EVT_2TCOORDS:
    {
      SMeshBufferLightMap *buffer = (SMeshBufferLightMap *)surface;
      buffer->Indices.set_used(numIndices);
      for (int i = 0; i < numIndices; ++i)
      {
        buffer->Indices[i] = PeekShort(indices, i * sizeof(u16));
      }
      break;
    }
    case EVT_TANGENTS:
    {
      SMeshBufferTangents *buffer = (SMeshBufferTangents *)surface;
      buffer->Indices.set_used(numIndices);
      for (int i = 0; i < numIndices; ++i)
      {
        buffer->Indices[i] = PeekShort(indices, i * sizeof(u16));
      }
      break;
    }
    }
  }

  EXPORT int CALL NumIndices(IMeshBuffer *surface)
  {
    return surface->getIndexCount();
  }

  EXPORT int CALL SurfaceIndex(IMeshBuffer *surface, int number)
  {
    return (surface->getIndexType() == EIT_16BIT)
               ? (surface->getIndices()[number] + 1)
               : (((u32 *)surface->getIndices())[number] + 1);
  }

  EXPORT SMaterial *CALL SurfaceMaterial(IMeshBuffer *surface)
  {
    return &surface->getMaterial();
  }

} // extern "C"
