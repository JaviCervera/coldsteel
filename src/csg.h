/**
 * @file
 * Constructive Solid Geometry (CSG). A CSG builder accumulates convex primitives (union and
 * difference) and converts the resulting solid into a single mesh, following the level-editor
 * mental model: start from solid space, carve rooms out of it with CSG_SUBTRACT, then fill them
 * with geometry using CSG_ADD. Operations are processed strictly in call order.
 */
#pragma once

#include "common.h"

#define CSG_ADD 0 /**< Union: place solid geometry with outward faces. */
#define CSG_SUBTRACT 1 /**< Difference: carve a cavity with inward faces. */

typedef struct CSGBuilder CSGBuilder; /* opaque CSG builder handle */

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a CSG builder that starts with a solid world (solid everywhere).
   *
   * @return The new CSG builder.
   */
  EXPORT CSGBuilder *CALL CreateCsg();

  /**
   * Frees a CSG builder.
   *
   * @param csg The CSG builder to free.
   */
  EXPORT void CALL FreeCsg(CSGBuilder *csg);

  /**
   * Resets the builder to an empty (solid everywhere) world, reusing the handle.
   *
   * @param csg The CSG builder to clear.
   */
  EXPORT void CALL ClearCsg(CSGBuilder *csg);

  /**
   * Adds a box primitive to the CSG solid.
   *
   * A first CSG_ADD renders nothing (the box is buried in the solid world); carve rooms first
   * and then fill them. Rotation is in degrees, matching RotateMesh.
   *
   * @param csg The CSG builder to modify.
   * @param operation The CSG operation to apply (CSG_ADD or CSG_SUBTRACT).
   * @param x The X position of the box center.
   * @param y The Y position of the box center.
   * @param z The Z position of the box center.
   * @param pitch The pitch rotation in degrees.
   * @param yaw The yaw rotation in degrees.
   * @param roll The roll rotation in degrees.
   * @param width The box width.
   * @param height The box height.
   * @param depth The box depth.
   */
  EXPORT void CALL AddCsgBox(CSGBuilder *csg, int operation, float x, float y, float z,
                             float pitch, float yaw, float roll, float width, float height, float depth);

  /**
   * Adds a cylinder primitive to the CSG solid.
   *
   * @param csg The CSG builder to modify.
   * @param operation The CSG operation to apply (CSG_ADD or CSG_SUBTRACT).
   * @param segments The number of radial segments.
   * @param x The X position of the cylinder center.
   * @param y The Y position of the cylinder center.
   * @param z The Z position of the cylinder center.
   * @param pitch The pitch rotation in degrees.
   * @param yaw The yaw rotation in degrees.
   * @param roll The roll rotation in degrees.
   * @param radius The cylinder radius.
   * @param height The cylinder height.
   */
  EXPORT void CALL AddCsgCylinder(CSGBuilder *csg, int operation, int segments,
                                  float x, float y, float z, float pitch, float yaw, float roll,
                                  float radius, float height);

  /**
   * Adds a cone primitive to the CSG solid.
   *
   * @param csg The CSG builder to modify.
   * @param operation The CSG operation to apply (CSG_ADD or CSG_SUBTRACT).
   * @param segments The number of radial segments.
   * @param x The X position of the cone center.
   * @param y The Y position of the cone center.
   * @param z The Z position of the cone center.
   * @param pitch The pitch rotation in degrees.
   * @param yaw The yaw rotation in degrees.
   * @param roll The roll rotation in degrees.
   * @param radius The cone base radius.
   * @param height The cone height.
   */
  EXPORT void CALL AddCsgCone(CSGBuilder *csg, int operation, int segments,
                              float x, float y, float z, float pitch, float yaw, float roll,
                              float radius, float height);

  /**
   * Adds a wedge (right-triangular prism) primitive to the CSG solid.
   *
   * The wedge is centered on its bounding box: it spans +/-width/2 (X), +/-height/2 (Y) and
   * +/-depth/2 (Z). The tall edge is at the back (z = -depth/2) and the thin edge at the front
   * (z = +depth/2); moving +Z climbs the slope. Placing the wedge on a floor needs a -height/2
   * Y offset.
   *
   * @param csg The CSG builder to modify.
   * @param operation The CSG operation to apply (CSG_ADD or CSG_SUBTRACT).
   * @param x The X position of the wedge center.
   * @param y The Y position of the wedge center.
   * @param z The Z position of the wedge center.
   * @param pitch The pitch rotation in degrees.
   * @param yaw The yaw rotation in degrees.
   * @param roll The roll rotation in degrees.
   * @param width The wedge width.
   * @param height The wedge height.
   * @param depth The wedge depth.
   */
  EXPORT void CALL AddCsgWedge(CSGBuilder *csg, int operation,
                               float x, float y, float z, float pitch, float yaw, float roll,
                               float width, float height, float depth);

  /**
   * Builds the final mesh from the CSG solid.
   *
   * The result is a concave mesh with per-triangle flat normals and a fixed material, similar to
   * FixMaterials. On an untouched builder, an empty mesh is returned.
   *
   * @param csg The CSG builder to convert.
   * @return A new mesh with the CSG result.
   */
  EXPORT IMesh *CALL CsgMesh(CSGBuilder *csg);

#ifdef __cplusplus
} /* extern "C" */
#endif