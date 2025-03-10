// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CGeometryCreator.h"
#include "SMesh.h"
#include "IMesh.h"
#include "IVideoDriver.h"
#include "os.h"

namespace irr
{
namespace scene
{

IMesh* CGeometryCreator::createCubeMesh(const core::vector3df& size, ECUBE_MESH_TYPE type) const
{
	SMesh* mesh = new SMesh;

	const video::SColor clr(255,255,255,255);

	if ( type == ECMT_1BUF_12VTX_NA )
	{
		SMeshBuffer* buffer = new SMeshBuffer();

		// Create indices (pos, neg describes normal direction of front-face)
		const u16 u[36] = {	0,2,1,   0,3,2,		// NEG_Z
							1,5,4,   1,2,5,		// POS_X
							4,6,7,   4,5,6,		// POS_Z
							7,3,0,   7,6,3,		// NEG_X
							9,5,2,   9,8,5,		// POS_Y
							0,11,10, 0,10,7};	// NEG_Y

		buffer->Indices.set_used(36);

		for (u32 i=0; i<36; ++i)
			buffer->Indices[i] = u[i];

		// Create vertices
		buffer->Vertices.reallocate(12);

		buffer->Vertices.push_back(video::S3DVertex(0,0,0, -1,-1,-1, clr, 0, 1));	// 0
		buffer->Vertices.push_back(video::S3DVertex(1,0,0,  1,-1,-1, clr, 1, 1));	// 1
		buffer->Vertices.push_back(video::S3DVertex(1,1,0,  1, 1,-1, clr, 1, 0));	// 2
		buffer->Vertices.push_back(video::S3DVertex(0,1,0, -1, 1,-1, clr, 0, 0));	// 3
		buffer->Vertices.push_back(video::S3DVertex(1,0,1,  1,-1, 1, clr, 0, 1));	// 4
		buffer->Vertices.push_back(video::S3DVertex(1,1,1,  1, 1, 1, clr, 0, 0));	// 5
		buffer->Vertices.push_back(video::S3DVertex(0,1,1, -1, 1, 1, clr, 1, 0));	// 6
		buffer->Vertices.push_back(video::S3DVertex(0,0,1, -1,-1, 1, clr, 1, 1));	// 7
		buffer->Vertices.push_back(video::S3DVertex(0,1,1, -1, 1, 1, clr, 0, 1));	// 8
		buffer->Vertices.push_back(video::S3DVertex(0,1,0, -1, 1,-1, clr, 1, 1));	// 9
		buffer->Vertices.push_back(video::S3DVertex(1,0,1,  1,-1, 1, clr, 1, 0));	// 10
		buffer->Vertices.push_back(video::S3DVertex(1,0,0,  1,-1,-1, clr, 0, 0));	// 11 

		// Recalculate bounding box and set cube size
		buffer->BoundingBox.reset(0,0,0);

		for (u32 i=0; i<12; ++i)
		{
			buffer->Vertices[i].Pos -= core::vector3df(0.5f, 0.5f, 0.5f);
			buffer->Vertices[i].Pos *= size;
			buffer->Vertices[i].Normal.normalize();
			buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
		}

		mesh->addMeshBuffer(buffer);
		buffer->drop();
	}
	else if ( type == ECMT_6BUF_4VTX_NP )
	{
		for ( int b=0; b<6; ++b )
		{
			SMeshBuffer* buffer = new SMeshBuffer();

			// Create indices
			const u16 u[6] = { 0,2,1,0,3,2};

			buffer->Indices.set_used(6);

			for ( int i=0; i<6; ++i )
				buffer->Indices[i] = u[i];

			// Create vertices
			buffer->Vertices.reallocate(4);

			switch ( b )
			{
				case 0:
					buffer->Vertices.push_back(video::S3DVertex(0,0,0,	0, 0,-1, clr, 0, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,0,0,  0, 0,-1, clr, 1, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,1,0,  0, 0,-1, clr, 1, 0));
					buffer->Vertices.push_back(video::S3DVertex(0,1,0,  0, 0,-1, clr, 0, 0));
				break;
				case 1:
					buffer->Vertices.push_back(video::S3DVertex(1,0,0,	1, 0, 0, clr, 0, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,0,1,  1, 0, 0, clr, 1, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,1,1,  1, 0, 0, clr, 1, 0));
					buffer->Vertices.push_back(video::S3DVertex(1,1,0,  1, 0, 0, clr, 0, 0));
				break;
				case 2:
					buffer->Vertices.push_back(video::S3DVertex(1,0,1,	0, 0, 1, clr, 0, 1));
					buffer->Vertices.push_back(video::S3DVertex(0,0,1,  0, 0, 1, clr, 1, 1));
					buffer->Vertices.push_back(video::S3DVertex(0,1,1,  0, 0, 1, clr, 1, 0));
					buffer->Vertices.push_back(video::S3DVertex(1,1,1,  0, 0, 1, clr, 0, 0));
				break;
				case 3:
					buffer->Vertices.push_back(video::S3DVertex(0,0,1,	-1, 0, 0, clr, 0, 1));
					buffer->Vertices.push_back(video::S3DVertex(0,0,0,  -1, 0, 0, clr, 1, 1));
					buffer->Vertices.push_back(video::S3DVertex(0,1,0,  -1, 0, 0, clr, 1, 0));
					buffer->Vertices.push_back(video::S3DVertex(0,1,1,  -1, 0, 0, clr, 0, 0));
				break;
				case 4:
					buffer->Vertices.push_back(video::S3DVertex(0,1,0,	0, 1, 0, clr, 0, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,1,0,  0, 1, 0, clr, 1, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,1,1,  0, 1, 0, clr, 1, 0));
					buffer->Vertices.push_back(video::S3DVertex(0,1,1,  0, 1, 0, clr, 0, 0));
				break;
				case 5:
					buffer->Vertices.push_back(video::S3DVertex(0,0,1,	0, -1, 0, clr, 0, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,0,1,  0, -1, 0, clr, 1, 1));
					buffer->Vertices.push_back(video::S3DVertex(1,0,0,  0, -1, 0, clr, 1, 0));
					buffer->Vertices.push_back(video::S3DVertex(0,0,0,  0, -1, 0, clr, 0, 0));
				break;
			}

			// Recalculate bounding box and set cube size
			for (u32 i=0; i<4; ++i)
			{
				buffer->Vertices[i].Pos -= core::vector3df(0.5f, 0.5f, 0.5f);
				buffer->Vertices[i].Pos *= size;
				if ( i == 0 )
					buffer->BoundingBox.reset(buffer->Vertices[i].Pos);
				else
					buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
			}

			mesh->addMeshBuffer(buffer);
			buffer->drop();
		}
	}
	else if ( type == ECMT_1BUF_24VTX_NP )
	{
		SMeshBuffer* buffer = new SMeshBuffer();

		// Create indices (pos, neg describes normal direction of front-face)
		const u16 u[36] = {	0,2,1,    0,3,2,		// NEG_Z
							4,7,6,    4,5,7,		// POS_X
							8,10,11,  8,9,10,		// POS_Z
							15,13,12, 15,14,13,		// NEG_X
							19,17,16, 19,18,17,		// POS_Y
							20,23,22, 20,22,21};	// NEG_Y

		buffer->Indices.set_used(36);

		for (u32 i=0; i<36; ++i)
			buffer->Indices[i] = u[i];

		// Create vertices
		buffer->Vertices.reallocate(24);

		buffer->Vertices.push_back(video::S3DVertex(0,0,0,  0, 0,-1, clr, 0, 1));	// 0
		buffer->Vertices.push_back(video::S3DVertex(1,0,0,  0, 0,-1, clr, 1, 1));	// 1
		buffer->Vertices.push_back(video::S3DVertex(1,1,0,  0, 0,-1, clr, 1, 0));	// 2
		buffer->Vertices.push_back(video::S3DVertex(0,1,0,  0, 0,-1, clr, 0, 0));	// 3

		buffer->Vertices.push_back(video::S3DVertex(1,0,0,  1, 0, 0, clr, 1, 1));	// 4 (1)
		buffer->Vertices.push_back(video::S3DVertex(1,1,0,  1, 0, 0, clr, 1, 0));	// 5 (2)
		buffer->Vertices.push_back(video::S3DVertex(1,0,1,  1, 0, 0, clr, 0, 1));	// 6 (4)
		buffer->Vertices.push_back(video::S3DVertex(1,1,1,  1, 0, 0, clr, 0, 0));	// 7 (5)

		buffer->Vertices.push_back(video::S3DVertex(1,0,1,  0, 0, 1, clr, 0, 1));	// 8 (4)
		buffer->Vertices.push_back(video::S3DVertex(1,1,1,  0, 0, 1, clr, 0, 0));	// 9 (5)
		buffer->Vertices.push_back(video::S3DVertex(0,1,1,  0, 0, 1, clr, 1, 0));	// 10 (6)
		buffer->Vertices.push_back(video::S3DVertex(0,0,1,  0, 0, 1, clr, 1, 1));	// 11 (7)

		buffer->Vertices.push_back(video::S3DVertex(0,0,0, -1, 0, 0, clr, 0, 1));	// 12 (0)
		buffer->Vertices.push_back(video::S3DVertex(0,1,0, -1, 0, 0, clr, 0, 0));	// 13 (3)
		buffer->Vertices.push_back(video::S3DVertex(0,1,1, -1, 0, 0, clr, 1, 0));	// 14 (6)
		buffer->Vertices.push_back(video::S3DVertex(0,0,1, -1, 0, 0, clr, 1, 1));	// 15 (7)

		buffer->Vertices.push_back(video::S3DVertex(1,1,0,  0, 1, 0, clr, 1, 0));	// 16 (2)
		buffer->Vertices.push_back(video::S3DVertex(1,1,1,  0, 1, 0, clr, 0, 0));	// 17 (5)
		buffer->Vertices.push_back(video::S3DVertex(0,1,1,  0, 1, 0, clr, 0, 1));	// 18 (8)
		buffer->Vertices.push_back(video::S3DVertex(0,1,0,  0, 1, 0, clr, 1, 1));	// 19 (9)

		buffer->Vertices.push_back(video::S3DVertex(0,0,0,  0,-1, 0, clr, 0, 1));	// 20 (0)
		buffer->Vertices.push_back(video::S3DVertex(0,0,1,  0,-1, 0, clr, 1, 1));	// 21 (7)
		buffer->Vertices.push_back(video::S3DVertex(1,0,1,  0,-1, 0, clr, 1, 0));	// 22 (10)
		buffer->Vertices.push_back(video::S3DVertex(1,0,0,  0,-1, 0, clr, 0, 0));	// 23 (11)

		// Recalculate bounding box and set cube size
		buffer->BoundingBox.reset(0,0,0);

		for (u32 i=0; i<24; ++i)
		{
			buffer->Vertices[i].Pos -= core::vector3df(0.5f, 0.5f, 0.5f);
			buffer->Vertices[i].Pos *= size;
			buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
		}

		mesh->addMeshBuffer(buffer);
		buffer->drop();
	}

	mesh->recalculateBoundingBox();
	return mesh;
}


// creates a hill plane
IMesh* CGeometryCreator::createHillPlaneMesh(
		const core::dimension2d<f32>& tileSize,
		const core::dimension2d<u32>& tc, video::SMaterial* material,
		f32 hillHeight, const core::dimension2d<f32>& ch,
		const core::dimension2d<f32>& textureRepeatCount) const
{
	core::dimension2d<u32> tileCount = tc;
	core::dimension2d<f32> countHills = ch;

	if (countHills.Width < 0.01f)
		countHills.Width = 1.f;
	if (countHills.Height < 0.01f)
		countHills.Height = 1.f;

	// center
	const core::position2d<f32> center((tileSize.Width * tileCount.Width) * 0.5f, (tileSize.Height * tileCount.Height) * 0.5f);

	// texture coord step
	const core::dimension2d<f32> tx(
			textureRepeatCount.Width / tileCount.Width,
			textureRepeatCount.Height / tileCount.Height);

	// add one more point in each direction for proper tile count
	++tileCount.Height;
	++tileCount.Width;

	SMeshBuffer* buffer = new SMeshBuffer();
	video::S3DVertex vtx;
	vtx.Color.set(255,255,255,255);

	// create vertices from left-front to right-back
	f32 sx=0.f, tsx=0.f;
	for (u32 x=0; x<tileCount.Width; ++x)
	{
		f32 sy=0.f, tsy=0.f;
		for (u32 y=0; y<tileCount.Height; ++y)
		{
			vtx.Pos.set(sx - center.X, 0, sy - center.Y);
			vtx.TCoords.set(tsx, 1.0f - tsy);

			if (core::isnotzero(hillHeight))
				vtx.Pos.Y = sinf(vtx.Pos.X * countHills.Width * core::PI / center.X) *
					cosf(vtx.Pos.Z * countHills.Height * core::PI / center.Y) *
					hillHeight;

			buffer->Vertices.push_back(vtx);
			sy += tileSize.Height;
			tsy += tx.Height;
		}
		sx += tileSize.Width;
		tsx += tx.Width;
	}

	// create indices

	for (u32 x=0; x<tileCount.Width-1; ++x)
	{
		for (u32 y=0; y<tileCount.Height-1; ++y)
		{
			const s32 current = x*tileCount.Height + y;

			buffer->Indices.push_back(current);
			buffer->Indices.push_back(current + 1);
			buffer->Indices.push_back(current + tileCount.Height);

			buffer->Indices.push_back(current + 1);
			buffer->Indices.push_back(current + 1 + tileCount.Height);
			buffer->Indices.push_back(current + tileCount.Height);
		}
	}

	// recalculate normals
	for (u32 i=0; i<buffer->Indices.size(); i+=3)
	{
		const core::vector3df normal = core::plane3d<f32>(
			buffer->Vertices[buffer->Indices[i+0]].Pos,
			buffer->Vertices[buffer->Indices[i+1]].Pos,
			buffer->Vertices[buffer->Indices[i+2]].Pos).Normal;

		buffer->Vertices[buffer->Indices[i+0]].Normal = normal;
		buffer->Vertices[buffer->Indices[i+1]].Normal = normal;
		buffer->Vertices[buffer->Indices[i+2]].Normal = normal;
	}

	if (material)
		buffer->Material = *material;

	buffer->recalculateBoundingBox();
	buffer->setHardwareMappingHint(EHM_STATIC);

	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	mesh->recalculateBoundingBox();
	buffer->drop();
	return mesh;
}

namespace
{

// Return the position on an exponential curve. Input from 0 to 1.
float geopos(float pos)
{
	pos = core::clamp<float>(pos, 0, 1);
	pos *= 5;

	const float out = powf(2.5f, pos - 5);

	return out;
}

}

//! Create a geoplane.
IMesh* CGeometryCreator::createGeoplaneMesh(f32 radius, u32 rows, u32 columns) const
{
	using namespace core;
	using namespace video;

	rows = clamp<u32>(rows, 3, 2048);
	columns = clamp<u32>(columns, 3, 2048);

	SMeshBuffer * const mb = new SMeshBuffer();
	S3DVertex v(0, 0, 0, 0, 1, 0, SColor(255, 255, 255, 255), 0, 0);
	const float anglestep = (2 * PI) / columns;

	mb->Vertices.reallocate((rows * columns) + 1);
	mb->Indices.reallocate((((rows - 2) * columns * 2) + columns) * 3);

	u32 i, j;
	mb->Vertices.push_back(v);
	for (j = 1; j < rows; j++)
	{
		const float len = radius * geopos((float) j/(rows-1));

		for (i = 0; i < columns; i++)
		{
			const float angle = anglestep * i;
			v.Pos = vector3df(len * sinf(angle), 0, len * cosf(angle));

			mb->Vertices.push_back(v);
		}
	}

	// Indices
	// First the inner fan
	for (i = 0; i < columns; i++)
	{
		mb->Indices.push_back(0);
		mb->Indices.push_back(1 + i);

		if (i == columns - 1)
			mb->Indices.push_back(1);
		else
			mb->Indices.push_back(2 + i);
	}

	// Then the surrounding quads
	for (j = 0; j < rows - 2; j++)
	{
		for (i = 0; i < columns; i++)
		{
			u32 start = ((j * columns) + i) + 1;
			u32 next = start + 1;
			u32 far = (((j + 1) * columns) + i) + 1;
			u32 farnext = far + 1;

			if (i == columns - 1)
			{
				next = ((j * columns)) + 1;
				farnext = (((j + 1) * columns)) + 1;
			}

			mb->Indices.push_back(start);
			mb->Indices.push_back(far);
			mb->Indices.push_back(next);

			mb->Indices.push_back(next);
			mb->Indices.push_back(far);
			mb->Indices.push_back(farnext);
		}
	}

	// Done
	SMesh * const mesh = new SMesh();
	mesh->addMeshBuffer(mb);
	mb->recalculateBoundingBox();
	mb->setHardwareMappingHint(EHM_STATIC);
	mesh->recalculateBoundingBox();
	mb->drop();

	return mesh;
}

IMesh* CGeometryCreator::createTerrainMesh(video::IImage* texture,
		video::IImage* heightmap, const core::dimension2d<f32>& stretchSize,
		f32 maxHeight, video::IVideoDriver* driver,
		const core::dimension2d<u32>& maxVtxBlockSize,
		bool debugBorders) const
{
	if (!texture || !heightmap)
		return 0;

	// debug border
	const s32 borderSkip = debugBorders ? 0 : 1;

	video::S3DVertex vtx;
	vtx.Color.set(255,255,255,255);

	SMesh* mesh = new SMesh();

	const u32 tm = os::Timer::getRealTime()/1000;
	const core::dimension2d<u32> hMapSize= heightmap->getDimension();
	const core::dimension2d<u32> tMapSize= texture->getDimension();
	const core::position2d<f32> thRel(static_cast<f32>(tMapSize.Width) / hMapSize.Width, static_cast<f32>(tMapSize.Height) / hMapSize.Height);
	maxHeight /= 255.0f; // height step per color value

	core::position2d<u32> processed(0,0);
	while (processed.Y<hMapSize.Height)
	{
		while(processed.X<hMapSize.Width)
		{
			core::dimension2d<u32> blockSize = maxVtxBlockSize;
			if (processed.X + blockSize.Width > hMapSize.Width)
				blockSize.Width = hMapSize.Width - processed.X;
			if (processed.Y + blockSize.Height > hMapSize.Height)
				blockSize.Height = hMapSize.Height - processed.Y;

			SMeshBuffer* buffer = new SMeshBuffer();
			buffer->setHardwareMappingHint(EHM_STATIC);
			buffer->Vertices.reallocate(blockSize.getArea());
			// add vertices of vertex block
			u32 y;
			core::vector2df pos(0.f, processed.Y*stretchSize.Height);
			const core::vector2df bs(1.f/blockSize.Width, 1.f/blockSize.Height);
			core::vector2df tc(0.f, 0.5f*bs.Y);
			for (y=0; y<blockSize.Height; ++y)
			{
				pos.X=processed.X*stretchSize.Width;
				tc.X=0.5f*bs.X;
				for (u32 x=0; x<blockSize.Width; ++x)
				{
					const f32 height = heightmap->getPixel(x+processed.X, y+processed.Y).getAverage() * maxHeight;

					vtx.Pos.set(pos.X, height, pos.Y);
					vtx.TCoords.set(tc);
					buffer->Vertices.push_back(vtx);
					pos.X += stretchSize.Width;
					tc.X += bs.X;
				}
				pos.Y += stretchSize.Height;
				tc.Y += bs.Y;
			}

			buffer->Indices.reallocate((blockSize.Height-1)*(blockSize.Width-1)*6);
			// add indices of vertex block
			s32 c1 = 0;
			for (y=0; y<blockSize.Height-1; ++y)
			{
				for (u32 x=0; x<blockSize.Width-1; ++x)
				{
					const s32 c = c1 + x;

					buffer->Indices.push_back(c);
					buffer->Indices.push_back(c + blockSize.Width);
					buffer->Indices.push_back(c + 1);

					buffer->Indices.push_back(c + 1);
					buffer->Indices.push_back(c + blockSize.Width);
					buffer->Indices.push_back(c + 1 + blockSize.Width);
				}
				c1 += blockSize.Width;
			}

			// recalculate normals
			for (u32 i=0; i<buffer->Indices.size(); i+=3)
			{
				const core::vector3df normal = core::plane3d<f32>(
					buffer->Vertices[buffer->Indices[i+0]].Pos,
					buffer->Vertices[buffer->Indices[i+1]].Pos,
					buffer->Vertices[buffer->Indices[i+2]].Pos).Normal;

				buffer->Vertices[buffer->Indices[i+0]].Normal = normal;
				buffer->Vertices[buffer->Indices[i+1]].Normal = normal;
				buffer->Vertices[buffer->Indices[i+2]].Normal = normal;
			}

			if (buffer->Vertices.size())
			{
				// create texture for this block
				video::IImage* img = driver->createImage(texture->getColorFormat(), core::dimension2d<u32>(core::floor32(blockSize.Width*thRel.X), core::floor32(blockSize.Height*thRel.Y)));
				texture->copyTo(img, core::position2di(0,0), core::recti(
					core::position2d<s32>(core::floor32(processed.X*thRel.X), core::floor32(processed.Y*thRel.Y)),
					core::dimension2d<u32>(core::floor32(blockSize.Width*thRel.X), core::floor32(blockSize.Height*thRel.Y))), 0);

				c8 textureName[64];
				sprintf(textureName, "terrain%u_%u", tm, mesh->getMeshBufferCount());

				buffer->Material.setTexture(0, driver->addTexture(textureName, img));

				if (buffer->Material.getTexture(0))
				{
					c8 tmp[255];
					sprintf(tmp, "Generated terrain texture (%ux%u): %s",
						buffer->Material.getTexture(0)->getSize().Width,
						buffer->Material.getTexture(0)->getSize().Height,
						textureName);
					os::Printer::log(tmp);
				}
				else
					os::Printer::log("Could not create terrain texture.", textureName, ELL_ERROR);

				img->drop();
			}

			buffer->recalculateBoundingBox();
			mesh->addMeshBuffer(buffer);
			buffer->drop();

			// keep on processing
			processed.X += maxVtxBlockSize.Width - borderSkip;
		}

		// keep on processing
		processed.X = 0;
		processed.Y += maxVtxBlockSize.Height - borderSkip;
	}

	mesh->recalculateBoundingBox();
	return mesh;
}


/*
	a cylinder, a cone and a cross
	point up on (0,1.f, 0.f )
*/
IMesh* CGeometryCreator::createArrowMesh(const u32 tessellationCylinder,
						const u32 tessellationCone,
						const f32 height,
						const f32 cylinderHeight,
						const f32 width0,
						const f32 width1,
						const video::SColor vtxColor0,
						const video::SColor vtxColor1) const
{
	SMesh* mesh = (SMesh*)createCylinderMesh(width0, cylinderHeight, tessellationCylinder, vtxColor0, false);

	IMesh* mesh2 = createConeMesh(width1, height-cylinderHeight, tessellationCone, vtxColor1, vtxColor0);
	for (u32 i=0; i<mesh2->getMeshBufferCount(); ++i)
	{
		IMeshBuffer* buffer = mesh2->getMeshBuffer(i);
		for (u32 j=0; j<buffer->getVertexCount(); ++j)
			buffer->getPosition(j).Y += cylinderHeight;
		buffer->setDirty(EBT_VERTEX);
		buffer->recalculateBoundingBox();
		mesh->addMeshBuffer(buffer);
	}
	mesh2->drop();
	mesh->setHardwareMappingHint(EHM_STATIC);

	mesh->recalculateBoundingBox();
	return mesh;
}


/* A sphere with proper normals and texture coords */
IMesh* CGeometryCreator::createSphereMesh(f32 radius, u32 polyCountX, u32 polyCountY) const
{
	// thanks to Alfaz93 who made his code available for Irrlicht on which
	// this one is based!

	// we are creating the sphere mesh here.

	if (polyCountX < 2)
		polyCountX = 2;
	if (polyCountY < 2)
		polyCountY = 2;
	while (polyCountX * polyCountY > 32767) // prevent u16 overflow
	{
		polyCountX /= 2;
		polyCountY /= 2;
	}

	const u32 polyCountXPitch = polyCountX+1; // get to same vertex on next level

	SMeshBuffer* buffer = new SMeshBuffer();

	buffer->Indices.reallocate((polyCountX * polyCountY) * 6);

	const video::SColor clr(255, 255,255,255);

	u32 level = 0;

	for (u32 p1 = 0; p1 < polyCountY-1; ++p1)
	{
		//main quads, top to bottom
		for (u32 p2 = 0; p2 < polyCountX - 1; ++p2)
		{
			const u32 curr = level + p2;
			buffer->Indices.push_back(curr + polyCountXPitch);
			buffer->Indices.push_back(curr);
			buffer->Indices.push_back(curr + 1);
			buffer->Indices.push_back(curr + polyCountXPitch);
			buffer->Indices.push_back(curr+1);
			buffer->Indices.push_back(curr + 1 + polyCountXPitch);
		}

		// the connectors from front to end
		buffer->Indices.push_back(level + polyCountX - 1 + polyCountXPitch);
		buffer->Indices.push_back(level + polyCountX - 1);
		buffer->Indices.push_back(level + polyCountX);

		buffer->Indices.push_back(level + polyCountX - 1 + polyCountXPitch);
		buffer->Indices.push_back(level + polyCountX);
		buffer->Indices.push_back(level + polyCountX + polyCountXPitch);
		level += polyCountXPitch;
	}

	const u32 polyCountSq = polyCountXPitch * polyCountY; // top point
	const u32 polyCountSq1 = polyCountSq + 1; // bottom point
	const u32 polyCountSqM1 = (polyCountY - 1) * polyCountXPitch; // last row's first vertex

	for (u32 p2 = 0; p2 < polyCountX - 1; ++p2)
	{
		// create triangles which are at the top of the sphere

		buffer->Indices.push_back(polyCountSq);
		buffer->Indices.push_back(p2 + 1);
		buffer->Indices.push_back(p2);

		// create triangles which are at the bottom of the sphere

		buffer->Indices.push_back(polyCountSqM1 + p2);
		buffer->Indices.push_back(polyCountSqM1 + p2 + 1);
		buffer->Indices.push_back(polyCountSq1);
	}

	// create final triangle which is at the top of the sphere

	buffer->Indices.push_back(polyCountSq);
	buffer->Indices.push_back(polyCountX);
	buffer->Indices.push_back(polyCountX-1);

	// create final triangle which is at the bottom of the sphere

	buffer->Indices.push_back(polyCountSqM1 + polyCountX - 1);
	buffer->Indices.push_back(polyCountSqM1);
	buffer->Indices.push_back(polyCountSq1);

	// calculate the angle which separates all points in a circle
	const f64 AngleX = 2 * core::PI / polyCountX;
	const f64 AngleY = core::PI / polyCountY;

	u32 i=0;

	// we don't start at 0.

	f64 ay = 0;//AngleY / 2;

	buffer->Vertices.set_used((polyCountXPitch * polyCountY) + 2);
	for (u32 y = 0; y < polyCountY; ++y)
	{
		ay += AngleY;
		const f64 sinay = sin(ay);
		f64 axz = 0;

		// calculate the necessary vertices without the doubled one
		for (u32 xz = 0;xz < polyCountX; ++xz)
		{
			// calculate points position

			const core::vector3df pos(static_cast<f32>(radius * cos(axz) * sinay),
						static_cast<f32>(radius * cos(ay)),
						static_cast<f32>(radius * sin(axz) * sinay));
			// for spheres the normal is the position
			core::vector3df normal(pos);
			normal.normalize();

			// calculate texture coordinates via sphere mapping
			// tu is the same on each level, so only calculate once
			f32 tu = 0.5f;
			if (y==0)
			{
				if (normal.Y != -1.0f && normal.Y != 1.0f)
					tu = static_cast<f32>(acos(core::clamp(normal.X/sinay, -1.0, 1.0)) * 0.5 *core::RECIPROCAL_PI64);
				if (normal.Z < 0.0f)
					tu=1-tu;
			}
			else
				tu = buffer->Vertices[i-polyCountXPitch].TCoords.X;
			buffer->Vertices[i] = video::S3DVertex(pos.X, pos.Y, pos.Z,
						normal.X, normal.Y, normal.Z,
						clr, tu,
						static_cast<f32>(ay*core::RECIPROCAL_PI64));
			++i;
			axz += AngleX;
		}
		// This is the doubled vertex on the initial position
		buffer->Vertices[i] = video::S3DVertex(buffer->Vertices[i-polyCountX]);
		buffer->Vertices[i].TCoords.X=1.0f;
		++i;
	}

	// the vertex at the top of the sphere
	buffer->Vertices[i] = video::S3DVertex(0.0f,radius,0.0f, 0.0f,1.0f,0.0f, clr, 0.5f, 0.0f);

	// the vertex at the bottom of the sphere
	++i;
	buffer->Vertices[i] = video::S3DVertex(0.0f,-radius,0.0f, 0.0f,-1.0f,0.0f, clr, 0.5f, 1.0f);

	// recalculate bounding box

	buffer->BoundingBox.reset(buffer->Vertices[i].Pos);
	buffer->BoundingBox.addInternalPoint(buffer->Vertices[i-1].Pos);
	buffer->BoundingBox.addInternalPoint(radius,0.0f,0.0f);
	buffer->BoundingBox.addInternalPoint(-radius,0.0f,0.0f);
	buffer->BoundingBox.addInternalPoint(0.0f,0.0f,radius);
	buffer->BoundingBox.addInternalPoint(0.0f,0.0f,-radius);

	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	buffer->drop();

	mesh->setHardwareMappingHint(EHM_STATIC);
	mesh->recalculateBoundingBox();
	return mesh;
}


/* A cylinder with proper normals and texture coords */
IMesh* CGeometryCreator::createCylinderMesh(f32 radius, f32 length,
			u32 tessellation, const video::SColor& color,
			bool closeTop, f32 oblique, u32 normalType) const
{
	SMeshBuffer* buffer = new SMeshBuffer();

	if ( tessellation < 2 )
		tessellation = 2;

	const f32 recTessellation = core::reciprocal((f32)tessellation);
	const f32 angleStep = (core::PI * 2.f ) * recTessellation;

	video::S3DVertex v;
	v.Color = color;
	buffer->Vertices.reallocate(tessellation*2+2+(closeTop?2:1));
	buffer->Indices.reallocate(tessellation*(closeTop?12:9));
	f32 tcx = 0.f;
	for (u32 i = 0; i <= tessellation; ++i )
	{
		const f32 angle = angleStep * i;
		v.Pos.X = radius * cosf(angle);
		v.Pos.Y = 0.f;
		v.Pos.Z = radius * sinf(angle);
		switch (normalType)
		{
			case 0: v.Normal = v.Pos; break;
			case 1: v.Normal = v.Pos; break;
		}
		v.Normal.normalize();
		v.TCoords.X=tcx;
		v.TCoords.Y=0.f;
		buffer->Vertices.push_back(v);

		v.Pos.X += oblique;
		v.Pos.Y = length;
		switch (normalType)
		{
			case 0: v.Normal = v.Pos; break;
			case 1: v.Normal = core::vector3df(v.Pos.X-oblique, 0, v.Pos.Z); break;
		}
		v.Normal.normalize();
		v.TCoords.Y=1.f;
		buffer->Vertices.push_back(v);
		tcx += recTessellation;
	}

	const u32 nonWrappedSize = tessellation*2;
	for (u32 i=0; i != nonWrappedSize; i += 2)
	{
		buffer->Indices.push_back(i + 2);
		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(i + 1);

		buffer->Indices.push_back(i + 2);
		buffer->Indices.push_back(i + 1);
		buffer->Indices.push_back(i + 3);
	}

	// close down
	v.Pos.X = 0.f;
	v.Pos.Y = 0.f;
	v.Pos.Z = 0.f;
	v.Normal.X = 0.f;
	v.Normal.Y = -1.f;
	v.Normal.Z = 0.f;
	v.TCoords.X = 0.5f;
	v.TCoords.Y = 1.f;
	buffer->Vertices.push_back(v);

	u32 index = buffer->Vertices.size() - 1;

	for (u32 i = 0; i != nonWrappedSize; i += 2 )
	{
		buffer->Indices.push_back(index);
		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(i + 2);
	}

	if (closeTop)
	{
		// close top
		v.Pos.X = oblique;
		v.Pos.Y = length;
		v.Pos.Z = 0.f;
		v.Normal.X = 0.f;
		v.Normal.Y = 1.f;
		v.Normal.Z = 0.f;
		v.TCoords.X = 0.5f;
		v.TCoords.Y = 0.f;
		buffer->Vertices.push_back(v);

		index = buffer->Vertices.size() - 1;

		for (u32 i = 0; i != nonWrappedSize; i += 2 )
		{
			buffer->Indices.push_back(i + 1);
			buffer->Indices.push_back(index);
			buffer->Indices.push_back(i + 3);
		}
	}

	buffer->recalculateBoundingBox();
	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	mesh->setHardwareMappingHint(EHM_STATIC);
	mesh->recalculateBoundingBox();
	buffer->drop();
	return mesh;
}


/* A cone with proper normals and texture coords */
IMesh* CGeometryCreator::createConeMesh(f32 radius, f32 length, u32 tessellation,
					const video::SColor& colorTop,
					const video::SColor& colorBottom,
					f32 oblique) const
{
	SMeshBuffer* buffer = new SMeshBuffer();

	if ( tessellation < 2 )
		tessellation = 2;

	const f32 angleStep = (core::PI * 2.f ) / tessellation;

	video::S3DVertex v;
	v.Pos.Y = 0.f;

	v.Color = colorTop;
	for (u32 i = 0; i != tessellation; ++i )
	{
		const f32 angle = angleStep * f32(i);
		const f32 cosAngle = cosf(angle);
		const f32 sinAngle = sinf(angle);

		v.TCoords.X = (1.f+cosAngle)*0.5f;
		v.TCoords.Y = (1.f+sinAngle)*0.5f;
		v.Pos.X = radius * cosAngle;
		v.Pos.Z = radius * sinAngle;
		v.Normal = v.Pos;
		v.Normal.normalize();
		buffer->Vertices.push_back(v);
	}

	const u32 nonWrappedSize = buffer->Vertices.size() - 1;

	// close top
	v.Pos.X = oblique;
	v.Pos.Y = length;
	v.Pos.Z = 0.f;
	v.Normal.X = 0.f;
	v.Normal.Y = 1.f;
	v.Normal.Z = 0.f;
	v.TCoords.X = 0.5f;
	v.TCoords.Y = 0.5f;
	buffer->Vertices.push_back(v);

	u32 index = buffer->Vertices.size() - 1;

	for (u32 i = 0; i != nonWrappedSize; ++i)
	{
		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(index);
		buffer->Indices.push_back(i + 1);
	}

	buffer->Indices.push_back(nonWrappedSize);
	buffer->Indices.push_back(index);
	buffer->Indices.push_back(0);

	// close down
	v.Color = colorBottom;
	v.Pos.X = 0.f;
	v.Pos.Y = 0.f;
	v.Pos.Z = 0.f;
	v.Normal.X = 0.f;
	v.Normal.Y = -1.f;
	v.Normal.Z = 0.f;
	v.TCoords.X = 0.5f;
	v.TCoords.Y = 0.5f;
	buffer->Vertices.push_back(v);

	index = buffer->Vertices.size() - 1;

	for (u32 i = 0; i != nonWrappedSize; i += 1 )
	{
		buffer->Indices.push_back(index);
		buffer->Indices.push_back(i + 0);
		buffer->Indices.push_back(i + 1);
	}

	buffer->Indices.push_back(index);
	buffer->Indices.push_back(nonWrappedSize);
	buffer->Indices.push_back(0);

	buffer->recalculateBoundingBox();
	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	buffer->drop();

	mesh->setHardwareMappingHint(EHM_STATIC);
	mesh->recalculateBoundingBox();
	return mesh;
}

irr::scene::IMesh* CGeometryCreator::createTorusMesh(irr::f32 majorRadius, irr::f32 minorRadius, irr::u32 majorSegments, irr::u32 minorSegments, f32 angleStart, f32 angleEnd, int capEnds) const
{
	if ( majorRadius == 0.f || minorRadius == 0.f )
		return 0;

	if ( majorSegments < 3 )
		majorSegments = 3;
	if ( minorSegments < 3 )
		minorSegments = 3;

	// Note: first/last vertices of major and minor lines are on same position, but not shared to allow for independent uv's.

	// prevent 16-bit vertex buffer overflow
	const u32 numCapVertices = (capEnds & 1 ? 1 : 0) + (capEnds & 2 ? 1 : 0);
	u32 numVertices = (majorSegments+1)*(minorSegments+1)+numCapVertices;
	while (numVertices > 65536)
	{
		if ( majorSegments > 2*minorSegments )
			majorSegments /= 2;
		else if ( minorSegments > 2*majorSegments )
			minorSegments /= 2;
		else
		{
			majorSegments /= 2;
			minorSegments /= 2;
		}
		numVertices = (majorSegments+1)*(minorSegments+1)+numCapVertices;
	}

	const u32 majorLines = majorSegments+1;
	const u32 minorLines = minorSegments+1;

	const video::SColor color(255,255,255,255);
	SMeshBuffer* buffer = new SMeshBuffer();
	buffer->Indices.reallocate(majorSegments*minorSegments*6);
	buffer->Vertices.reallocate(numVertices);

	if ( angleStart > angleEnd )
		core::swap(angleStart, angleEnd);
	const f32 radStart = angleStart * core::DEGTORAD;
	const f32 radEnd = angleEnd * core::DEGTORAD;
	const f32 radMajorLen = radEnd-radStart;
	const f32 radStepMajor = radMajorLen / majorSegments;
	const f32 TWO_PI = 2.f*core::PI;
	const f32 radStepMinor = TWO_PI / minorSegments;

	// vertices
	for ( irr::u32 major = 0; major < majorLines; ++major)
	{
		const f32 radMajor = radStart + major*radStepMajor;
		const f32 cosMajor = cosf(radMajor);
		const f32 sinMajor = sinf(radMajor);

		// points of major circle
		const core::vector3df pm(majorRadius*cosMajor, 0.f, majorRadius * sinMajor);

		for ( irr::u32 minor = 0; minor < minorLines; ++minor)
		{
			const f32 radMinor = minor*radStepMinor;
			const f32 cosMinor = cosf(radMinor);

			const core::vector3df n(cosMinor * cosMajor, sinf(radMinor), cosMinor * sinMajor);
			const core::vector2df uv(radMajor/TWO_PI, radMinor/TWO_PI);
			buffer->Vertices.push_back( video::S3DVertex(pm+n*minorRadius, n, color, uv) );
		}
	}

	// indices
	for ( irr::u32 major = 0; major < majorSegments; ++major)
	{
		for ( irr::u32 minor = 0; minor < minorSegments; ++minor)
		{
			const irr::u16 i = major*minorLines+minor;
			buffer->Indices.push_back(i+1);
			buffer->Indices.push_back(i+minorLines);
			buffer->Indices.push_back(i);

			buffer->Indices.push_back(i+1);
			buffer->Indices.push_back(i+minorLines+1);
			buffer->Indices.push_back(i+minorLines);
		}
	}

	// add start caps
	if ( capEnds & 1 )
	{
		const core::vector3df p(cosf(radStart), 0.f, sinf(radStart));
		const core::vector3df n( p.crossProduct(core::vector3df(0,-1,0)) );
		const core::vector2df uv(radStart/TWO_PI, 0.5f);
		buffer->Vertices.push_back( video::S3DVertex(p*majorRadius, n, color, uv) );

		const irr::u16 i=buffer->Vertices.size()-1;
		for ( irr::u32 minor = 0; minor < minorSegments; ++minor)
		{
			buffer->Indices.push_back(minor+1);
			buffer->Indices.push_back(minor);
			buffer->Indices.push_back(i);
		}
	}

	// add end caps
	if ( capEnds & 2 )
	{
		const core::vector3df p(cosf(radEnd), 0.f, sinf(radEnd));
		const core::vector3df n( p.crossProduct(core::vector3df(0,1,0)) );
		const core::vector2df uv(radEnd/TWO_PI, 0.5f);
		buffer->Vertices.push_back( video::S3DVertex(p*majorRadius, n, color, uv) );

		const irr::u16 i=buffer->Vertices.size()-1;
		const irr::u16 k=i-numCapVertices;
		for ( irr::u32 minor = 0; minor < minorSegments; ++minor)
		{
			buffer->Indices.push_back(k-minor-1);
			buffer->Indices.push_back(k-minor);
			buffer->Indices.push_back(i);
		}
	}

	// recalculate bounding box
	buffer->BoundingBox.MaxEdge.X = core::abs_(majorRadius)+core::abs_(minorRadius);
	buffer->BoundingBox.MaxEdge.Z = buffer->BoundingBox.MaxEdge.X;
	buffer->BoundingBox.MaxEdge.Y = core::abs_(minorRadius);
	buffer->BoundingBox.MinEdge = buffer->BoundingBox.MaxEdge*-1.f;

	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	buffer->drop();

	mesh->setHardwareMappingHint(EHM_STATIC);
	mesh->recalculateBoundingBox();
	return mesh;
}

void CGeometryCreator::addToBuffer(const video::S3DVertex& v, SMeshBuffer* Buffer) const
{
	const s32 tnidx = Buffer->Vertices.linear_reverse_search(v);
	const bool alreadyIn = (tnidx != -1);
	u16 nidx = (u16)tnidx;
	if (!alreadyIn)
	{
		nidx = (u16)Buffer->Vertices.size();
		Buffer->Indices.push_back(nidx);
		Buffer->Vertices.push_back(v);
	}
	else
		Buffer->Indices.push_back(nidx);
}


IMesh* CGeometryCreator::createVolumeLightMesh(
		const u32 subdivideU, const u32 subdivideV,
		const video::SColor footColor, const video::SColor tailColor,
		const f32 lpDistance, const core::vector3df& lightDim) const
{
	SMeshBuffer* Buffer = new SMeshBuffer();
	Buffer->setHardwareMappingHint(EHM_STATIC);

	const core::vector3df lightPoint(0, -(lpDistance*lightDim.Y), 0);
	const f32 ax = lightDim.X * 0.5f; // X Axis
	const f32 az = lightDim.Z * 0.5f; // Z Axis

	Buffer->Vertices.clear();
	Buffer->Vertices.reallocate(6+12*(subdivideU+subdivideV));
	Buffer->Indices.clear();
	Buffer->Indices.reallocate(6+12*(subdivideU+subdivideV));
	//draw the bottom foot.. the glowing region
	addToBuffer(video::S3DVertex(-ax, 0, az,  0,0,0, footColor, 0, 1),Buffer);
	addToBuffer(video::S3DVertex( ax, 0, az,  0,0,0, footColor, 1, 1),Buffer);
	addToBuffer(video::S3DVertex( ax, 0,-az,  0,0,0, footColor, 1, 0),Buffer);

	addToBuffer(video::S3DVertex( ax, 0,-az,  0,0,0, footColor, 1, 0),Buffer);
	addToBuffer(video::S3DVertex(-ax, 0,-az,  0,0,0, footColor, 0, 0),Buffer);
	addToBuffer(video::S3DVertex(-ax, 0, az,  0,0,0, footColor, 0, 1),Buffer);

	f32 tu = 0.f;
	const f32 tuStep = 1.f/subdivideU;
	f32 bx = -ax;
	const f32 bxStep = lightDim.X * tuStep;
	// Slices in X/U space
	for (u32 i = 0; i <= subdivideU; ++i)
	{
		// These are the two endpoints for a slice at the foot
		core::vector3df end1(bx, 0.0f, -az);
		core::vector3df end2(bx, 0.0f, az);

		end1 -= lightPoint;		// get a vector from point to lightsource
		end1.normalize();		// normalize vector
		end1 *= lightDim.Y;	// multiply it out by shootlength

		end1.X += bx;			// Add the original point location to the vector
		end1.Z -= az;

		// Do it again for the other point.
		end2 -= lightPoint;
		end2.normalize();
		end2 *= lightDim.Y;

		end2.X += bx;
		end2.Z += az;

		addToBuffer(video::S3DVertex(bx , 0,  az,  0,0,0, footColor, tu, 1),Buffer);
		addToBuffer(video::S3DVertex(bx , 0, -az,  0,0,0, footColor, tu, 0),Buffer);
		addToBuffer(video::S3DVertex(end2.X , end2.Y, end2.Z,  0,0,0, tailColor, tu, 1),Buffer);

		addToBuffer(video::S3DVertex(bx , 0, -az,  0,0,0, footColor, tu, 0),Buffer);
		addToBuffer(video::S3DVertex(end1.X , end1.Y, end1.Z,  0,0,0, tailColor, tu, 0),Buffer);
		addToBuffer(video::S3DVertex(end2.X , end2.Y, end2.Z,  0,0,0, tailColor, tu, 1),Buffer);

		//back side
		addToBuffer(video::S3DVertex(-end2.X , end2.Y, -end2.Z,  0,0,0, tailColor, tu, 1),Buffer);
		addToBuffer(video::S3DVertex(-bx , 0,  -az,  0,0,0, footColor, tu, 1),Buffer);
		addToBuffer(video::S3DVertex(-bx , 0, az,  0,0,0, footColor, tu, 0),Buffer);

		addToBuffer(video::S3DVertex(-bx , 0, az,  0,0,0, footColor, tu, 0),Buffer);
		addToBuffer(video::S3DVertex(-end1.X , end1.Y, -end1.Z,  0,0,0, tailColor, tu, 0),Buffer);
		addToBuffer(video::S3DVertex(-end2.X , end2.Y, -end2.Z,  0,0,0, tailColor, tu, 1),Buffer);
		tu += tuStep;
		bx += bxStep;
	}

	f32 tv = 0.f;
	const f32 tvStep = 1.f/subdivideV;
	f32 bz = -az;
	const f32 bzStep = lightDim.Z * tvStep;
	// Slices in Z/V space
	for(u32 i = 0; i <= subdivideV; ++i)
	{
		// These are the two endpoints for a slice at the foot
		core::vector3df end1(-ax, 0.0f, bz);
		core::vector3df end2(ax, 0.0f, bz);

		end1 -= lightPoint;		// get a vector from point to lightsource
		end1.normalize();		// normalize vector
		end1 *= lightDim.Y;	// multiply it out by shootlength

		end1.X -= ax;			// Add the original point location to the vector
		end1.Z += bz;

		// Do it again for the other point.
		end2 -= lightPoint;
		end2.normalize();
		end2 *= lightDim.Y;

		end2.X += ax;
		end2.Z += bz;

		addToBuffer(video::S3DVertex(-ax , 0, bz,  0,0,0, footColor, 0, tv),Buffer);
		addToBuffer(video::S3DVertex(ax , 0,  bz,  0,0,0, footColor, 1, tv),Buffer);
		addToBuffer(video::S3DVertex(end2.X , end2.Y, end2.Z,  0,0,0, tailColor, 1, tv),Buffer);

		addToBuffer(video::S3DVertex(end2.X , end2.Y, end2.Z,  0,0,0, tailColor, 1, tv),Buffer);
		addToBuffer(video::S3DVertex(end1.X , end1.Y, end1.Z,  0,0,0, tailColor, 0, tv),Buffer);
		addToBuffer(video::S3DVertex(-ax , 0, bz,  0,0,0, footColor, 0, tv),Buffer);

		//back side
		addToBuffer(video::S3DVertex(ax , 0, -bz,  0,0,0, footColor, 0, tv),Buffer);
		addToBuffer(video::S3DVertex(-ax , 0,  -bz,  0,0,0, footColor, 1, tv),Buffer);
		addToBuffer(video::S3DVertex(-end2.X , end2.Y, -end2.Z,  0,0,0, tailColor, 1, tv),Buffer);

		addToBuffer(video::S3DVertex(-end2.X , end2.Y, -end2.Z,  0,0,0, tailColor, 1, tv),Buffer);
		addToBuffer(video::S3DVertex(-end1.X , end1.Y, -end1.Z,  0,0,0, tailColor, 0, tv),Buffer);
		addToBuffer(video::S3DVertex(ax , 0, -bz,  0,0,0, footColor, 0, tv),Buffer);
		tv += tvStep;
		bz += bzStep;
	}

	Buffer->recalculateBoundingBox();

	Buffer->Material.MaterialType = video::EMT_ONETEXTURE_BLEND;
	Buffer->Material.MaterialTypeParam = pack_textureBlendFunc( video::EBF_SRC_COLOR, video::EBF_SRC_ALPHA, video::EMFN_MODULATE_1X );

	Buffer->Material.Lighting = false;
	Buffer->Material.ZWriteEnable = video::EZW_OFF;

	Buffer->setDirty(EBT_VERTEX_AND_INDEX);

	Buffer->recalculateBoundingBox();
	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(Buffer);
	Buffer->drop();

	mesh->recalculateBoundingBox();
	return mesh;
}


} // end namespace scene
} // end namespace irr

