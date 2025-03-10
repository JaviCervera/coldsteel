// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef IRR_C_MESH_MANIPULATOR_H_INCLUDED
#define IRR_C_MESH_MANIPULATOR_H_INCLUDED

#include "IMeshManipulator.h"

namespace irr
{
namespace scene
{

//! An interface for easy manipulation of meshes.
/** Scale, set alpha value, flip surfaces, and so on. This exists for fixing
problems with wrong imported or exported meshes quickly after loading. It is
not intended for doing mesh modifications and/or animations during runtime.
*/
class CMeshManipulator : public IMeshManipulator
{
public:
	//! Flips the direction of surfaces.
	/** Changes backfacing triangles to frontfacing triangles and vice versa.
	\param mesh: Mesh on which the operation is performed. */
	virtual void flipSurfaces(scene::IMesh* mesh) const IRR_OVERRIDE;

	//! Recalculates all normals of the mesh.
	/** \param mesh: Mesh on which the operation is performed.
	\param smooth: Whether to use smoothed normals. */
	virtual void recalculateNormals(scene::IMesh* mesh, bool smooth = false, bool angleWeighted = false) const IRR_OVERRIDE;

	//! Recalculates all normals of the mesh buffer.
	/** \param buffer: Mesh buffer on which the operation is performed.
	\param smooth: Whether to use smoothed normals. */
	virtual void recalculateNormals(IMeshBuffer* buffer, bool smooth = false, bool angleWeighted = false) const IRR_OVERRIDE;

	//! Clones a static IMesh into a modifiable SMesh.
	virtual SMesh* createMeshCopy(const scene::IMesh* mesh) const IRR_OVERRIDE;

	//! Creates a planar texture mapping on the mesh
	/** \param mesh: Mesh on which the operation is performed.
	\param resolution: resolution of the planar mapping. This is the value
	specifying which is the relation between world space and
	texture coordinate space. */
	virtual void makePlanarTextureMapping(scene::IMesh* mesh, f32 resolution=0.001f) const IRR_OVERRIDE;

	//! Creates a planar texture mapping on the meshbuffer
	virtual void makePlanarTextureMapping(scene::IMeshBuffer* meshbuffer, f32 resolution=0.001f) const IRR_OVERRIDE;

	//! Creates a planar texture mapping on the meshbuffer
	void makePlanarTextureMapping(scene::IMeshBuffer* buffer, f32 resolutionS, f32 resolutionT, u8 axis, const core::vector3df& offset) const IRR_OVERRIDE;

	//! Creates a planar texture mapping on the mesh
	void makePlanarTextureMapping(scene::IMesh* mesh, f32 resolutionS, f32 resolutionT, u8 axis, const core::vector3df& offset) const IRR_OVERRIDE;

	//! Recalculates tangents, requires a tangent mesh buffer
	virtual void recalculateTangents(IMeshBuffer* buffer, bool recalculateNormals=false, bool smooth=false, bool angleWeighted=false) const IRR_OVERRIDE;

	//! Recalculates tangents, requires a tangent mesh
	virtual void recalculateTangents(IMesh* mesh, bool recalculateNormals=false, bool smooth=false, bool angleWeighted=false) const IRR_OVERRIDE;

	//! Creates a copy of the mesh, which will only consist of S3DVertexTangents vertices.
	virtual IMesh* createMeshWithTangents(const IMesh* mesh, bool recalculateNormals=false, bool smooth=false, bool angleWeighted=false, bool recalculateTangents=true) const IRR_OVERRIDE;

	//! Creates a copy of the mesh, which will only consist of S3D2TCoords vertices.
	virtual IMesh* createMeshWith2TCoords(const IMesh* mesh) const IRR_OVERRIDE;

	//! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
	virtual IMesh* createMeshWith1TCoords(const IMesh* mesh) const IRR_OVERRIDE;

	//! Creates a copy of the mesh, which will only consist of unique triangles, i.e. no vertices are shared.
	virtual IMesh* createMeshUniquePrimitives(const IMesh* mesh) const IRR_OVERRIDE;

	//! Creates a copy of the mesh, which will have all duplicated vertices removed, i.e. maximal amount of vertices are shared via indexing.
	virtual IMesh* createMeshWelded(const IMesh *mesh, f32 tolerance=core::ROUNDING_ERROR_f32) const IRR_OVERRIDE;

	//! Returns amount of polygons in mesh.
	virtual s32 getPolyCount(const scene::IMesh* mesh) const IRR_OVERRIDE;

	//! Returns amount of polygons in mesh.
	virtual s32 getPolyCount(scene::IAnimatedMesh* mesh) const IRR_OVERRIDE;

	//! create a new AnimatedMesh and adds the mesh to it
	virtual IAnimatedMesh* createAnimatedMesh(scene::IMesh* mesh,scene::E_ANIMATED_MESH_TYPE type) const IRR_OVERRIDE;

	//! create a mesh optimized for the vertex cache
	virtual IMesh* createForsythOptimizedMesh(const scene::IMesh *mesh) const IRR_OVERRIDE;

	//! Optimizes the mesh using an algorithm tuned for heightmaps
	virtual void heightmapOptimizeMesh(IMesh * const m, const f32 tolerance = core::ROUNDING_ERROR_f32) const IRR_OVERRIDE;

	//! Optimizes the mesh using an algorithm tuned for heightmaps
	virtual void heightmapOptimizeMesh(IMeshBuffer * const m, const f32 tolerance = core::ROUNDING_ERROR_f32) const IRR_OVERRIDE;
};

} // end namespace scene
} // end namespace irr

#endif
