// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_OCTREE_SCENENODE_

#include "COctreeSceneNode.h"
#include "Octree.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "ICameraSceneNode.h"
#include "IMeshCache.h"
#include "IAnimatedMesh.h"
#include "IMaterialRenderer.h"
#include "os.h"
#ifdef _IRR_COMPILE_WITH_SHADOW_VOLUME_SCENENODE_
#include "CShadowVolumeSceneNode.h"
#else
#include "IShadowVolumeSceneNode.h"
#endif // _IRR_COMPILE_WITH_SHADOW_VOLUME_SCENENODE_
#include "EProfileIDs.h"
#include "IProfiler.h"

namespace irr
{
namespace scene
{


//! constructor
COctreeSceneNode::COctreeSceneNode(ISceneNode* parent, ISceneManager* mgr,
					 s32 id, s32 minimalPolysPerNode)
	: IOctreeSceneNode(parent, mgr, id), StdOctree(0), LightMapOctree(0),
	TangentsOctree(0), VertexType((video::E_VERTEX_TYPE)-1),
	MinimalPolysPerNode(minimalPolysPerNode), Mesh(0), Shadow(0),
	UseVBOs(EOV_NO_VBO), PolygonChecks(EOPC_BOX)
{
#ifdef _DEBUG
	setDebugName("COctreeSceneNode");
#endif

	IRR_PROFILE(
		static bool initProfile = false;
		if (!initProfile )
		{
			initProfile = true;
			getProfiler().add(EPID_OC_RENDER, L"render octnode", L"Irrlicht scene");
			getProfiler().add(EPID_OC_CALCPOLYS, L"calc octnode", L"Irrlicht scene");
		}
 	)
}


//! destructor
COctreeSceneNode::~COctreeSceneNode()
{
	if (Shadow)
		Shadow->drop();
	deleteTree();
}


void COctreeSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		// because this node supports rendering of mixed mode meshes consisting of
		// transparent and solid material at the same time, we need to go through all
		// materials, check of what type they are and register this node for the right
		// render pass according to that.

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		PassCount = 0;
		u32 transparentCount = 0;
		u32 solidCount = 0;

		// count transparent and solid materials in this scene node
		for (u32 i=0; i<Materials.size(); ++i)
		{
			if (driver->needsTransparentRenderPass(Materials[i]))
				++transparentCount;
			else
				++solidCount;

			if (solidCount && transparentCount)
				break;
		}

		// register according to material types counted

		if (solidCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);

		if (transparentCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

		ISceneNode::OnRegisterSceneNode();
	}
}

template <class VT>
void renderMeshBuffer(video::IVideoDriver* driver, EOCTREENODE_VBO useVBO, typename Octree<VT>::SMeshChunk& meshChunk, const typename Octree<VT>::SIndexData& indexData)
{
	switch ( useVBO )
	{
		case EOV_NO_VBO:
			driver->drawIndexedTriangleList(
				&meshChunk.Vertices[0],
				meshChunk.Vertices.size(),
				indexData.Indices, indexData.CurrentSize / 3);
				break;
		case EOV_USE_VBO:
			driver->drawMeshBuffer ( &meshChunk );
			break;
		case EOV_USE_VBO_WITH_VISIBITLY:
		{
			u16* oldPointer = meshChunk.Indices.pointer();
			const u32 oldSize = meshChunk.Indices.size();
			meshChunk.Indices.set_free_when_destroyed(false);
			meshChunk.Indices.set_pointer(indexData.Indices, indexData.CurrentSize, false, false);
			meshChunk.setDirty(scene::EBT_INDEX);
			driver->drawMeshBuffer ( &meshChunk );
			meshChunk.Indices.set_pointer(oldPointer, oldSize);
			meshChunk.setDirty(scene::EBT_INDEX);
			break;
		}
	}
}

//! renders the node.
void COctreeSceneNode::render()
{
	IRR_PROFILE(CProfileScope psRender(EPID_OC_RENDER);)
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!driver)
		return;

	ICameraSceneNode* camera = SceneManager->getActiveCamera();
	if (!camera)
		return;

	const bool isTransparentPass =
		SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;
	++PassCount;

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	if (Shadow)
		Shadow->updateShadowVolumes();

	SViewFrustum frust = *camera->getViewFrustum();

	//transform the frustum to the current absolute transformation
	if ( !AbsoluteTransformation.isIdentity() )
	{
		core::matrix4 invTrans(AbsoluteTransformation, core::matrix4::EM4CONST_INVERSE);
		frust.transform(invTrans);
	}

	const core::aabbox3d<float> &box = frust.getBoundingBox();

	switch (VertexType)
	{
	case video::EVT_STANDARD:
		{
			IRR_PROFILE(getProfiler().start(EPID_OC_CALCPOLYS));
			switch ( PolygonChecks )
			{
				case EOPC_BOX:
					StdOctree->calculatePolys(box);
					break;
				case EOPC_FRUSTUM:
					StdOctree->calculatePolys(frust);
					break;
			}
			IRR_PROFILE(getProfiler().stop(EPID_OC_CALCPOLYS));

			const Octree<video::S3DVertex>::SIndexData* d = StdOctree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				if ( 0 == d[i].CurrentSize )
					continue;

				const bool transparent = driver->needsTransparentRenderPass(Materials[i]);

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
					renderMeshBuffer<video::S3DVertex>(driver, UseVBOs, StdMeshes[i], d[i]);
				}
			}
		}
		break;
	case video::EVT_2TCOORDS:
		{
			IRR_PROFILE(getProfiler().start(EPID_OC_CALCPOLYS));
			switch ( PolygonChecks )
			{
				case EOPC_BOX:
					LightMapOctree->calculatePolys(box);
					break;
				case EOPC_FRUSTUM:
					LightMapOctree->calculatePolys(frust);
					break;
			}
			IRR_PROFILE(getProfiler().stop(EPID_OC_CALCPOLYS));

			const Octree<video::S3DVertex2TCoords>::SIndexData* d = LightMapOctree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				if ( 0 == d[i].CurrentSize )
					continue;

				const video::IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);

					renderMeshBuffer<video::S3DVertex2TCoords>(driver, UseVBOs, LightMapMeshes[i], d[i]);
				}
			}
		}
		break;
	case video::EVT_TANGENTS:
		{
			IRR_PROFILE(getProfiler().start(EPID_OC_CALCPOLYS));
			switch ( PolygonChecks )
			{
				case EOPC_BOX:
					TangentsOctree->calculatePolys(box);
					break;
				case EOPC_FRUSTUM:
					TangentsOctree->calculatePolys(frust);
					break;
			}
			IRR_PROFILE(getProfiler().stop(EPID_OC_CALCPOLYS));

			const Octree<video::S3DVertexTangents>::SIndexData* d =  TangentsOctree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				if ( 0 == d[i].CurrentSize )
					continue;

				const video::IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
					renderMeshBuffer<video::S3DVertexTangents>(driver, UseVBOs, TangentsMeshes[i], d[i]);
				}
			}
		}
		break;
	}

	// for debug purposes only
	if (DebugDataVisible && !Materials.empty() && PassCount==1)
	{
		core::array< const core::aabbox3d<f32>* > boxes;
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		if ( DebugDataVisible & scene::EDS_BBOX_BUFFERS )
		{
			switch (VertexType)
			{
				case video::EVT_STANDARD:
					StdOctree->getBoundingBoxes(box, boxes);
					break;
				case video::EVT_2TCOORDS:
					LightMapOctree->getBoundingBoxes(box, boxes);
					break;
				case video::EVT_TANGENTS:
					TangentsOctree->getBoundingBoxes(box, boxes);
					break;
			}

			for (u32 b=0; b!=boxes.size(); ++b)
				driver->draw3DBox(*boxes[b]);
		}

		if ( DebugDataVisible & scene::EDS_BBOX )
			driver->draw3DBox(Box,video::SColor(0,255,0,0));
	}

}


//! Removes a child from this scene node.
//! Implemented here, to be able to remove the shadow properly, if there is one,
//! or to remove attached childs.
bool COctreeSceneNode::removeChild(ISceneNode* child)
{
	if (child && Shadow == child)
	{
		Shadow->drop();
		Shadow = 0;
	}

	return ISceneNode::removeChild(child);
}

void COctreeSceneNode::setUseVBO(EOCTREENODE_VBO useVBO)
{
	UseVBOs = useVBO;
	if ( Mesh )
		createTree(Mesh);
}

EOCTREENODE_VBO COctreeSceneNode::getUseVBO() const
{
	return UseVBOs;
}

void COctreeSceneNode::setPolygonChecks(EOCTREE_POLYGON_CHECKS checks)
{
	PolygonChecks = checks;
}

EOCTREE_POLYGON_CHECKS COctreeSceneNode::getPolygonChecks() const
{
	return PolygonChecks;
}

//! Creates shadow volume scene node as child of this node
//! and returns a pointer to it.
IShadowVolumeSceneNode* COctreeSceneNode::addShadowVolumeSceneNode(
		const IMesh* shadowMesh, s32 id, bool zfailmethod, f32 infinity)
{
#ifdef _IRR_COMPILE_WITH_SHADOW_VOLUME_SCENENODE_ 
	if (!SceneManager->getVideoDriver()->queryFeature(video::EVDF_STENCIL_BUFFER))
		return 0;

	if (!shadowMesh)
		shadowMesh = Mesh; // if null is given, use the mesh of node

	if (Shadow)
		Shadow->drop();

	Shadow = new CShadowVolumeSceneNode(shadowMesh, this, SceneManager, id,  zfailmethod, infinity);
	return Shadow;
#else
	return 0;
#endif
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& COctreeSceneNode::getBoundingBox() const
{
	return Box;
}


//! creates the tree
/* This method has a lot of duplication and overhead. Moreover, the tangents mesh conversion does not really work. I think we need a a proper mesh implementation for octrees, which handle all vertex types internally. Converting all structures to just one vertex type is always problematic.
Thanks to Auria for fixing major parts of this method. */
bool COctreeSceneNode::createTree(IMesh* mesh)
{
	if (!mesh)
		return false;

	MeshName = SceneManager->getMeshCache()->getMeshName(mesh);

    mesh->grab();
	deleteTree();

	Mesh = mesh;

	const u32 beginTime = os::Timer::getRealTime();

	u32 nodeCount = 0;
	u32 polyCount = 0;
	u32 i;

	Box = mesh->getBoundingBox();

	if (mesh->getMeshBufferCount())
	{
		// check for "largest" buffer types
		// Also dropping buffers/materials for empty buffer
		// (which looks like a horrible idea. If a user wanted that material without mesh he should still get it...
		//	but not going to change that now. Only documenting it after figuring out what happens here.
		//  It works at least as Materials are reset in deleteTree).
		VertexType = video::EVT_STANDARD;
		u32 meshReserve = 0;
		for (i=0; i<mesh->getMeshBufferCount(); ++i)
		{
			const IMeshBuffer* b = mesh->getMeshBuffer(i);
			if (b->getVertexCount() && b->getIndexCount())
			{
				++meshReserve;
				if (b->getVertexType() == video::EVT_2TCOORDS)
					VertexType = video::EVT_2TCOORDS;
				else if (b->getVertexType() == video::EVT_TANGENTS)
					VertexType = video::EVT_TANGENTS;
			}
		}
		Materials.reallocate(Materials.size()+meshReserve);

		switch(VertexType)
		{
		case video::EVT_STANDARD:
			{
				StdMeshes.reallocate(StdMeshes.size() + meshReserve);
				for (i=0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount())
					{
						Materials.push_back(b->getMaterial());

						StdMeshes.push_back(Octree<video::S3DVertex>::SMeshChunk());
						Octree<video::S3DVertex>::SMeshChunk &nchunk = StdMeshes.getLast();
						nchunk.MaterialId = Materials.size() - 1;

						u32 v;
						nchunk.Vertices.reallocate(b->getVertexCount());
						switch (b->getVertexType())
						{
						case video::EVT_STANDARD:
							for (v=0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((video::S3DVertex*)b->getVertices())[v]);
							break;
						case video::EVT_2TCOORDS:
							for (v=0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((video::S3DVertex2TCoords*)b->getVertices())[v]);
							break;
						case video::EVT_TANGENTS:
							for (v=0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((video::S3DVertexTangents*)b->getVertices())[v]);
							break;
						}

						polyCount += b->getIndexCount();

						nchunk.Indices.reallocate(b->getIndexCount());
						for (v=0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				StdOctree = new Octree<video::S3DVertex>(StdMeshes, MinimalPolysPerNode);
				nodeCount = StdOctree->getNodeCount();
			}
			break;
		case video::EVT_2TCOORDS:
			{
				LightMapMeshes.reallocate(LightMapMeshes.size() + meshReserve);

				for ( i=0; i < mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount())
					{
						Materials.push_back(b->getMaterial());
						LightMapMeshes.push_back(Octree<video::S3DVertex2TCoords>::SMeshChunk());
						Octree<video::S3DVertex2TCoords>::SMeshChunk& nchunk = LightMapMeshes.getLast();
						nchunk.MaterialId = Materials.size() - 1;

						if (UseVBOs == EOV_USE_VBO_WITH_VISIBITLY)
						{
							nchunk.setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX);
							nchunk.setHardwareMappingHint(scene::EHM_DYNAMIC, scene::EBT_INDEX);
						}
						else
							nchunk.setHardwareMappingHint(scene::EHM_STATIC);

						u32 v;
						nchunk.Vertices.reallocate(b->getVertexCount());
						switch (b->getVertexType())
						{
						case video::EVT_STANDARD:
							for (v=0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((video::S3DVertex*)b->getVertices())[v]);
							break;
						case video::EVT_2TCOORDS:
							for (v=0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((video::S3DVertex2TCoords*)b->getVertices())[v]);
							break;
						case video::EVT_TANGENTS:
							for (v=0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((video::S3DVertexTangents*)b->getVertices())[v]);
							break;
						}

						polyCount += b->getIndexCount();
						nchunk.Indices.reallocate(b->getIndexCount());
						for (v=0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				LightMapOctree = new Octree<video::S3DVertex2TCoords>(LightMapMeshes, MinimalPolysPerNode);
				nodeCount = LightMapOctree->getNodeCount();
			}
			break;
		case video::EVT_TANGENTS:
			{
				TangentsMeshes.reallocate(TangentsMeshes.size() + meshReserve);

				for (u32 i=0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount())
					{
						Materials.push_back(b->getMaterial());
						TangentsMeshes.push_back(Octree<video::S3DVertexTangents>::SMeshChunk());
						Octree<video::S3DVertexTangents>::SMeshChunk& nchunk = TangentsMeshes.getLast();
						nchunk.MaterialId = Materials.size() - 1;

						u32 v;
						nchunk.Vertices.reallocate(b->getVertexCount());
						switch (b->getVertexType())
						{
						case video::EVT_STANDARD:
							for (v=0; v<b->getVertexCount(); ++v)
							{
								const video::S3DVertex& tmpV = ((video::S3DVertex*)b->getVertices())[v];
								nchunk.Vertices.push_back(video::S3DVertexTangents(tmpV.Pos, tmpV.Color, tmpV.TCoords));
							}
							break;
						case video::EVT_2TCOORDS:
							for (v=0; v<b->getVertexCount(); ++v)
							{
								const video::S3DVertex2TCoords& tmpV = ((video::S3DVertex2TCoords*)b->getVertices())[v];
								nchunk.Vertices.push_back(video::S3DVertexTangents(tmpV.Pos, tmpV.Color, tmpV.TCoords));
							}
							break;
						case video::EVT_TANGENTS:
							for (v=0; v<b->getVertexCount(); ++v)
								nchunk.Vertices.push_back(((video::S3DVertexTangents*)b->getVertices())[v]);
							break;
						}

						polyCount += b->getIndexCount();
						nchunk.Indices.reallocate(b->getIndexCount());
						for (v=0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				TangentsOctree = new Octree<video::S3DVertexTangents>(TangentsMeshes, MinimalPolysPerNode);
				nodeCount = TangentsOctree->getNodeCount();
			}
			break;
		}
	}

	const u32 endTime = os::Timer::getRealTime();
	c8 tmp[255];
	sprintf(tmp, "Needed %ums to create Octree SceneNode.(%u nodes, %u polys)",
		endTime - beginTime, nodeCount, polyCount/3);
	os::Printer::log(tmp, ELL_INFORMATION);

	return true;
}


//! returns the material based on the zero based index i. 
video::SMaterial& COctreeSceneNode::getMaterial(u32 i)
{
	if ( i >= Materials.size() )
		return ISceneNode::getMaterial(i);

	return Materials[i];
}


//! returns amount of materials used by this scene node.
u32 COctreeSceneNode::getMaterialCount() const
{
	return Materials.size();
}


//! Writes attributes of the scene node.
void COctreeSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addInt("MinimalPolysPerNode", MinimalPolysPerNode);
	out->addString("Mesh", MeshName.c_str());
}


//! Reads attributes of the scene node.
void COctreeSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	const s32 oldMinimal = MinimalPolysPerNode;

	MinimalPolysPerNode = in->getAttributeAsInt("MinimalPolysPerNode");
	io::path newMeshStr = in->getAttributeAsString("Mesh");

	IMesh* newMesh = 0;

	if (newMeshStr == "")
		newMeshStr = MeshName;

	IAnimatedMesh* newAnimatedMesh = SceneManager->getMesh(newMeshStr.c_str());

	if (newAnimatedMesh)
		newMesh = newAnimatedMesh->getMesh(0);

	if (newMesh && ((MeshName != newMeshStr) || (MinimalPolysPerNode != oldMinimal)))
	{
		// recalculate tree
		createTree(newMesh);
	}

	ISceneNode::deserializeAttributes(in, options);
}


void COctreeSceneNode::deleteTree()
{
	delete StdOctree;
	StdOctree = 0;
	StdMeshes.clear();

	delete LightMapOctree;
	LightMapOctree = 0;
	LightMapMeshes.clear();

	delete TangentsOctree;
	TangentsOctree = 0;
	TangentsMeshes.clear();

	Materials.clear();

	if(Mesh)
		Mesh->drop();
}

void COctreeSceneNode::setMesh(IMesh* mesh, bool copyMeshMaterials)
{
	// TODO: copyMeshMaterials ignored for now, createTree has another note about materials, read that first
	createTree(mesh);
}

IMesh* COctreeSceneNode::getMesh(void)
{
	return Mesh;
}

void COctreeSceneNode::setReadOnlyMaterials(bool readonly)
{
	// Do nothing
}

bool COctreeSceneNode::isReadOnlyMaterials() const
{
	return false;
}


} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OCTREE_SCENENODE_
