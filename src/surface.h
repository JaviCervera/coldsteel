/**
 * @file
 * A Surface (also called a MeshBuffer) is a single renderable piece of a mesh, containing vertex
 * and index data. Use SURFACE_* constants when creating surfaces.
 */
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

  /**
   * Adds a new surface to a mesh using vertex and index memblocks.
   *
   * @param mesh The mesh that will receive the new surface.
   * @param vertices The memblock containing vertex data.
   * @param numVertices The number of vertices in the memblock.
   * @param indices The memblock containing index data.
   * @param numIndices The number of indices in the memblock.
   * @param type The surface type to create (SURFACE_*).
   * @return The new surface.
   */
  EXPORT IMeshBuffer *CALL AddSurface(IMesh *mesh, Memblock *vertices, int numVertices, Memblock *indices, int numIndices, int type);

  /**
   * Updates the vertex data of a surface from a memblock.
   *
   * @param surface The surface to update.
   * @param vertices The memblock containing vertex data.
   * @param numVertices The number of vertices in the memblock.
   */
  EXPORT void CALL SetVertices(IMeshBuffer *surface, Memblock *vertices, int numVertices);

  /**
   * Returns the number of vertices in a surface.
   *
   * @param surface The surface to query.
   * @return The number of vertices in the surface.
   */
  EXPORT int CALL NumVertices(IMeshBuffer *surface);

  /**
   * Returns the X position of a vertex.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @return The X position of the vertex.
   */
  EXPORT float CALL VertexX(IMeshBuffer *surface, int index);

  /**
   * Returns the Y position of a vertex.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @return The Y position of the vertex.
   */
  EXPORT float CALL VertexY(IMeshBuffer *surface, int index);

  /**
   * Returns the Z position of a vertex.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @return The Z position of the vertex.
   */
  EXPORT float CALL VertexZ(IMeshBuffer *surface, int index);

  /**
   * Returns the X component of a vertex normal.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @return The X component of the vertex normal.
   */
  EXPORT float CALL VertexNX(IMeshBuffer *surface, int index);

  /**
   * Returns the Y component of a vertex normal.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @return The Y component of the vertex normal.
   */
  EXPORT float CALL VertexNY(IMeshBuffer *surface, int index);

  /**
   * Returns the Z component of a vertex normal.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @return The Z component of the vertex normal.
   */
  EXPORT float CALL VertexNZ(IMeshBuffer *surface, int index);

  /**
   * Returns the color of a vertex.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @return The color of the vertex.
   */
  EXPORT int CALL VertexColor(IMeshBuffer *surface, int index);

  /**
   * Returns the U texture coordinate for a vertex.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @param set The texture coordinate set to read (0 or 1).
   * @return The U texture coordinate.
   */
  EXPORT float CALL VertexU(IMeshBuffer *surface, int index, int set);

  /**
   * Returns the V texture coordinate for a vertex.
   *
   * @param surface The surface to query.
   * @param index The vertex index.
   * @param set The texture coordinate set to read (0 or 1).
   * @return The V texture coordinate.
   */
  EXPORT float CALL VertexV(IMeshBuffer *surface, int index, int set);

  /**
   * Updates the index data of a surface from a memblock.
   *
   * @param surface The surface to update.
   * @param indices The memblock containing index data.
   * @param numIndices The number of indices in the memblock.
   */
  EXPORT void CALL SetIndices(IMeshBuffer *surface, Memblock *indices, int numIndices);

  /**
   * Returns the number of indices in a surface.
   *
   * @param surface The surface to query.
   * @return The number of indices in the surface.
   */
  EXPORT int CALL NumIndices(IMeshBuffer *surface);

  /**
   * Returns the index value at the given position.
   *
   * @param surface The surface to query.
   * @param number The index position to read.
   * @return The index value at the requested position.
   */
  EXPORT int CALL SurfaceIndex(IMeshBuffer *surface, int number);

  /**
   * Returns the material of a surface.
   *
   * @param surface The surface to query.
   * @return The material of the surface.
   */
  EXPORT SMaterial *CALL SurfaceMaterial(IMeshBuffer *surface);

#ifdef __cplusplus
} /* extern "C" */
#endif
