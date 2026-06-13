/**
 * @file
 * A Mesh is a collection of surfaces that can be loaded from a file or created procedurally.
 * Meshes are used to create Models.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a cone mesh.
   *
   * @param segments The number of radial segments.
   * @return A new cone mesh.
   */
  EXPORT IMesh *CALL CreateConeMesh(int segments);

  /**
   * Creates a cube mesh.
   *
   * @return A new cube mesh.
   */
  EXPORT IMesh *CALL CreateCubeMesh();

  /**
   * Creates a cylinder mesh.
   *
   * @param segments The number of radial segments.
   * @return A new cylinder mesh.
   */
  EXPORT IMesh *CALL CreateCylinderMesh(int segments);

  /**
   * Creates an empty mesh.
   *
   * @return A new empty mesh.
   */
  EXPORT IMesh *CALL CreateMesh();

  /**
   * Creates a unit quad mesh.
   *
   * @return A new quad mesh.
   */
  EXPORT IMesh *CALL CreateQuadMesh();

  /**
   * Creates a simple cube mesh without texture coordinates.
   *
   * @return A new simple cube mesh.
   */
  EXPORT IMesh *CALL CreateSimpleCubeMesh();

  /**
   * Creates a sphere mesh.
   *
   * @param segments The number of segments.
   * @return A new sphere mesh.
   */
  EXPORT IMesh *CALL CreateSphereMesh(int segments);

  /**
   * Loads a mesh from a file.
   *
   * @param filename The mesh file to load.
   * @return The loaded mesh.
   */
  EXPORT IMesh *CALL LoadMesh(const char *filename);

  /**
   * Creates a terrain mesh from image data.
   *
   * @param heightmap The heightmap image.
   * @param pixmap The color pixmap image.
   * @param width The terrain width in world units.
   * @param height The terrain height in world units.
   * @param depth The terrain depth in world units.
   * @return A new terrain mesh.
   */
  EXPORT IMesh *CALL LoadTerrainMesh(IImage *heightmap, IImage *pixmap, float width, float height, float depth);

  /**
   * Frees a mesh.
   *
   * @param mesh The mesh to free.
   */
  EXPORT void CALL FreeMesh(IMesh *mesh);

  /**
   * Sets the animation playback speed.
   *
   * @param mesh The mesh whose animation speed will be set.
   * @param fps The playback speed in frames per second.
   */
  EXPORT void CALL SetMeshFPS(IMesh *mesh, float fps);

  /**
   * Returns the animation playback speed.
   *
   * @param mesh The mesh to query.
   * @return The playback speed in frames per second.
   */
  EXPORT float CALL MeshFPS(IMesh *mesh);

  /**
   * Returns the total number of animation frames.
   *
   * @param mesh The mesh to query.
   * @return The total number of animation frames.
   */
  EXPORT int CALL MeshFrames(IMesh *mesh);

  /**
   * Returns the mesh for a specific animation frame.
   *
   * @param mesh The animated mesh to query.
   * @param frame The animation frame index.
   * @return The mesh for the requested frame.
   */
  EXPORT IMesh *CALL MeshForFrame(IMesh *mesh, int frame);

  /**
   * Returns the number of surfaces in the mesh.
   *
   * @param mesh The mesh to query.
   * @return The number of surfaces in the mesh.
   */
  EXPORT int CALL NumSurfaces(IMesh *mesh);

  /**
   * Returns the surface at the given index.
   *
   * @param mesh The mesh to query.
   * @param index The surface index.
   * @return The surface at the given index.
   */
  EXPORT IMeshBuffer *CALL MeshSurface(IMesh *mesh, int index);

  /**
   * Recalculates mesh bounding boxes after geometry changes.
   *
   * @param mesh The mesh to update.
   */
  EXPORT void CALL UpdateMesh(IMesh *mesh);

  /**
   * Translates the mesh geometry.
   *
   * @param mesh The mesh to transform.
   * @param x The X offset.
   * @param y The Y offset.
   * @param z The Z offset.
   */
  EXPORT void CALL TranslateMesh(IMesh *mesh, float x, float y, float z);

  /**
   * Rotates the mesh geometry.
   *
   * @param mesh The mesh to transform.
   * @param pitch The pitch angle in degrees.
   * @param yaw The yaw angle in degrees.
   * @param roll The roll angle in degrees.
   */
  EXPORT void CALL RotateMesh(IMesh *mesh, float pitch, float yaw, float roll);

  /**
   * Scales the mesh geometry.
   *
   * @param mesh The mesh to transform.
   * @param x The X scale factor.
   * @param y The Y scale factor.
   * @param z The Z scale factor.
   */
  EXPORT void CALL ScaleMesh(IMesh *mesh, float x, float y, float z);

  /**
   * Flips all surface normals of the mesh.
   *
   * @param mesh The mesh to modify.
   */
  EXPORT void CALL FlipMesh(IMesh *mesh);

  /**
   * Sets the vertex color of all vertices in the mesh.
   *
   * @param mesh The mesh to modify.
   * @param color The color to apply.
   */
  EXPORT void CALL SetMeshColor(IMesh *mesh, int color);

  /**
   * Recalculates all vertex normals.
   *
   * @param mesh The mesh to update.
   */
  EXPORT void CALL UpdateMeshNormals(IMesh *mesh);

  /**
   * Returns the bounding box width.
   *
   * @param mesh The mesh to query.
   * @return The mesh width.
   */
  EXPORT float CALL MeshWidth(IMesh *mesh);

  /**
   * Returns the bounding box height.
   *
   * @param mesh The mesh to query.
   * @return The mesh height.
   */
  EXPORT float CALL MeshHeight(IMesh *mesh);

  /**
   * Returns the bounding box depth.
   *
   * @param mesh The mesh to query.
   * @return The mesh depth.
   */
  EXPORT float CALL MeshDepth(IMesh *mesh);

  /**
   * Saves a mesh to an OBJ file.
   *
   * @param mesh The mesh to save.
   * @param filename The output OBJ file path.
   */
  EXPORT bool_t CALL SaveMesh(IMesh *mesh, const char *filename);

#ifndef SWIG
  bool_t _MeshAnimated(IMesh *mesh);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
