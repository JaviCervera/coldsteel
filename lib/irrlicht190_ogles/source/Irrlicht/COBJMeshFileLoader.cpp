// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_OBJ_LOADER_

#include "COBJMeshFileLoader.h"
#include "CMeshTextureLoader.h"
#include "IMeshManipulator.h"
#include "IVideoDriver.h"
#include "SMesh.h"
#include "SAnimatedMesh.h"
#include "IReadFile.h"
#include "IAttributes.h"
#include "fast_atof.h"
#include "coreutil.h"
#include "os.h"

namespace irr
{
namespace scene
{

#ifdef _DEBUG
#define _IRR_DEBUG_OBJ_LOADER_
#endif

static const u32 WORD_BUFFER_LENGTH = 512;

//! Constructor
COBJMeshFileLoader::COBJMeshFileLoader(scene::ISceneManager* smgr, io::IFileSystem* fs)
: SceneManager(smgr), FileSystem(fs)
{
	#ifdef _DEBUG
	setDebugName("COBJMeshFileLoader");
	#endif

	if (FileSystem)
		FileSystem->grab();

	TextureLoader = new CMeshTextureLoader( FileSystem, SceneManager->getVideoDriver() );
}


//! destructor
COBJMeshFileLoader::~COBJMeshFileLoader()
{
	if (FileSystem)
		FileSystem->drop();
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool COBJMeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "obj" );
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* COBJMeshFileLoader::createMesh(io::IReadFile* file)
{
	if (!file)
		return 0;
	size_t filesize = file->getSize();
	if (filesize == 0 || filesize == (size_t)-1L)
		return 0;

	const io::path fullName = file->getFileName();
	const io::path relPath = FileSystem->getFileDir(fullName)+"/";

	c8* buf = new c8[filesize+1]; // plus null-terminator (some string functions used in parsing)
	filesize = file->read((void*)buf, filesize);
	if ( filesize == 0 )
	{
		delete[] buf;
		return 0;
	}
	buf[filesize] = 0;

	if ( getMeshTextureLoader() )
		getMeshTextureLoader()->setMeshFile(file);

	const c8* const bufEnd = buf+filesize;

	const u32 WORD_BUFFER_LENGTH = 512;

	core::array<core::vector3df, core::irrAllocatorFast<core::vector3df> > vertexBuffer(1000);
	core::array<core::vector3df, core::irrAllocatorFast<core::vector3df> > normalsBuffer(1000);
	core::array<core::vector2df, core::irrAllocatorFast<core::vector2df> > textureCoordBuffer(1000);

	SObjMtl * currMtl = new SObjMtl(getIndexTypeHint());
	Materials.push_back(currMtl);
	u32 smoothingGroup=0;

	// Process obj information
	const c8* bufPtr = buf;
	core::stringc grpName, mtlName;
	bool mtlChanged=false;
	bool useGroups = !SceneManager->getParameters()->getAttributeAsBool(OBJ_LOADER_IGNORE_GROUPS);
	bool useMaterials = !SceneManager->getParameters()->getAttributeAsBool(OBJ_LOADER_IGNORE_MATERIAL_FILES);
	irr::u32 lineNr = 1;	// only counts non-empty lines, still useful in debugging to locate errors
	core::array<int> faceCorners;
	faceCorners.reallocate(32); // should be large enough
	const core::stringc TAG_OFF = "off";
	irr::u32 degeneratedFaces = 0;

	while(bufPtr != bufEnd)
	{
		switch(bufPtr[0])
		{
		case 'm':	// mtllib (material)
		{
			if (useMaterials)
			{
				// Bit fuzzy definition. Some doc (http://paulbourke.net) says there can be more then one file and they are separated by spaces
				// Other doc (Wikipedia) says it's one file. Which does allow loading mtl files with spaces in the name.
				// Other tools I tested seem to go with the Wikipedia definition
				// Irrlicht did just use first word in Irrlicht 1.8, but with 1.9 we switch to allowing filenames with spaces
				// If this turns out to cause troubles we can maybe try to catch those cases by looking for ".mtl " inside the string
				const c8 * inBuf = goNextWord(bufPtr, bufEnd, false);
				core::stringc name = copyLine(inBuf, bufEnd);

#ifdef _IRR_DEBUG_OBJ_LOADER_
				os::Printer::log("Reading material file",name);
#endif
				readMTL(name.c_str(), relPath);
			}
		}
			break;

		case 'v':               // v, vn, vt
			switch(bufPtr[1])
			{
			case ' ':          // vertex
				{
					core::vector3df vec;
					bufPtr = readVec3(bufPtr, vec, bufEnd);
					vertexBuffer.push_back(vec);
				}
				break;

			case 'n':       // normal
				{
					core::vector3df vec;
					bufPtr = readVec3(bufPtr, vec, bufEnd);
					normalsBuffer.push_back(vec);
				}
				break;

			case 't':       // texcoord
				{
					core::vector2df vec;
					bufPtr = readUV(bufPtr, vec, bufEnd);
					textureCoordBuffer.push_back(vec);
				}
				break;
			}
			break;

		case 'g': // group name
			{
				c8 grp[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(grp, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _IRR_DEBUG_OBJ_LOADER_
	os::Printer::log("Loaded group start",grp, ELL_DEBUG);
#endif
				if (useGroups)
				{
					if (0 != grp[0])
						grpName = grp;
					else
						grpName = "default";
				}
				mtlChanged=true;
			}
			break;

		case 's': // smoothing can be a group or off (equiv. to 0)
			{
				c8 smooth[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(smooth, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _IRR_DEBUG_OBJ_LOADER_
	os::Printer::log("Loaded smoothing group start",smooth, ELL_DEBUG);
#endif
				if (TAG_OFF==smooth)
					smoothingGroup=0;
				else
					smoothingGroup=core::strtoul10(smooth);

				(void)smoothingGroup; // disable unused variable warnings
			}
			break;

		case 'u': // usemtl
			// get name of material
			{
				c8 matName[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(matName, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _IRR_DEBUG_OBJ_LOADER_
	os::Printer::log("Loaded material start",matName, ELL_DEBUG);
#endif
				mtlName=matName;
				mtlChanged=true;
			}
			break;

		case 'f':               // face
		{
			c8 vertexWord[WORD_BUFFER_LENGTH]; // for retrieving vertex data
			video::S3DVertex v;
			// Assign vertex color from currently active material's diffuse color
			if (mtlChanged)
			{
				// retrieve the material
				SObjMtl *useMtl = findMtl(mtlName, grpName);
				// only change material if we found it
				if (useMtl)
					currMtl = useMtl;
				mtlChanged=false;
			}
			if (currMtl)
				v.Color = currMtl->Meshbuffer->Material.DiffuseColor;

			// get all vertices data in this face (current line of obj file)
			IVertexBuffer& mbVertexBuffer = currMtl->Meshbuffer->getVertexBuffer();
			IIndexBuffer& mbIndexBuffer = currMtl->Meshbuffer->getIndexBuffer();
			const core::stringc wordBuffer = copyLine(bufPtr, bufEnd);
			const c8* linePtr = wordBuffer.c_str();
			const c8* const endPtr = linePtr+wordBuffer.size();

			faceCorners.set_used(0); // fast clear

			// read in all vertices
			linePtr = goNextWord(linePtr, endPtr);
			while (0 != linePtr[0])
			{
				// Array to communicate with retrieveVertexIndices()
				// sends the buffer sizes and gets the actual indices
				// if index not set returns -1
				s32 Idx[3];
				Idx[0] = Idx[1] = Idx[2] = -1;

				// read in next vertex's data
				u32 wlength = copyWord(vertexWord, linePtr, WORD_BUFFER_LENGTH, endPtr);
				// this function will also convert obj's 1-based index to c++'s 0-based index
				retrieveVertexIndices(vertexWord, Idx, vertexWord+wlength+1, vertexBuffer.size(), textureCoordBuffer.size(), normalsBuffer.size());
				if ( Idx[0] >= 0 && Idx[0] < (irr::s32)vertexBuffer.size() )
					v.Pos = vertexBuffer[Idx[0]];
				else
				{
					os::Printer::log("Invalid vertex index in this line", wordBuffer.c_str(), ELL_ERROR);
					delete [] buf;
					cleanUp();
					return 0;
				}
				if ( Idx[1] >= 0 && Idx[1] < (irr::s32)textureCoordBuffer.size() )
					v.TCoords = textureCoordBuffer[Idx[1]];
				else
					v.TCoords.set(0.0f,0.0f);
				if ( Idx[2] >= 0 && Idx[2] < (irr::s32)normalsBuffer.size() )
					v.Normal = normalsBuffer[Idx[2]];
				else
				{
					v.Normal.set(0.0f,0.0f,0.0f);
					currMtl->RecalculateNormals=true;
				}

				int vertLocation;
				core::map<video::S3DVertex, int>::Node* n = currMtl->VertMap.find(v);
				if (n)
				{
					vertLocation = n->getValue();
				}
				else
				{
					mbVertexBuffer.push_back(v);
					vertLocation = mbVertexBuffer.size() -1;
					currMtl->VertMap.insert(v, vertLocation);
				}

				faceCorners.push_back(vertLocation);

				// go to next vertex
				linePtr = goNextWord(linePtr, endPtr);
			}

			// triangulate the face
			if ( faceCorners.size() >= 3)
			{
				const int c = faceCorners[0];
				for ( u32 i = 1; i < faceCorners.size() - 1; ++i )
				{
					// Add a triangle
					const int a = faceCorners[i + 1];
					const int b = faceCorners[i];
					if (a != b && a != c && b != c)	// ignore degenerated faces. We can get them when we merge vertices above in the VertMap.
					{
						mbIndexBuffer.push_back(a);
						mbIndexBuffer.push_back(b);
						mbIndexBuffer.push_back(c);
					}
					else
					{
						++degeneratedFaces;
					}
				}
			}
			else
			{
				os::Printer::log("Too few vertices in this line", wordBuffer.c_str());
			}
		}
		break;

		case '#': // comment
		default:
			break;
		}	// end switch(bufPtr[0])
		// eat up rest of line
		bufPtr = goNextLine(bufPtr, bufEnd);
		++lineNr;
	}	// end while(bufPtr && (bufPtr-buf<filesize))

	if ( degeneratedFaces > 0 )
	{
		irr::core::stringc log(degeneratedFaces);
		log += " degenerated faces removed in ";
		log += irr::core::stringc(fullName);
		os::Printer::log(log.c_str(), ELL_INFORMATION);
	}

	SMesh* mesh = new SMesh();

	// Combine all the groups (meshbuffers) into the mesh
	for ( u32 m = 0; m < Materials.size(); ++m )
	{
		if ( Materials[m]->Meshbuffer->getIndexCount() > 0 )
		{
			if ( getIndexTypeHint() == EITH_OPTIMAL
				&& Materials[m]->Meshbuffer->getVertexCount() <= 65536 )
			{
				Materials[m]->Meshbuffer->getIndexBuffer().setType(video::EIT_16BIT);
			}


			Materials[m]->Meshbuffer->recalculateBoundingBox();
			if (Materials[m]->RecalculateNormals)
				SceneManager->getMeshManipulator()->recalculateNormals(Materials[m]->Meshbuffer);
			if (Materials[m]->Meshbuffer->Material.MaterialType == video::EMT_PARALLAX_MAP_SOLID)
			{
				Materials[m]->Meshbuffer->getVertexBuffer().setType(video::EVT_TANGENTS);
			}
			mesh->addMeshBuffer( Materials[m]->Meshbuffer );
		}
	}

	// Create the Animated mesh if there's anything in the mesh
	SAnimatedMesh* animMesh = 0;
	if ( 0 != mesh->getMeshBufferCount() )
	{
		mesh->recalculateBoundingBox();
		animMesh = new SAnimatedMesh();
		animMesh->Type = EAMT_OBJ;
		animMesh->addMesh(mesh);
		animMesh->recalculateBoundingBox();
	}

	// Clean up the allocate obj file contents
	delete [] buf;
	// more cleaning up
	cleanUp();
	mesh->drop();

	return animMesh;
}


const c8* COBJMeshFileLoader::readTextures(const c8* bufPtr, const c8* const bufEnd, SObjMtl* currMaterial, const io::path& relPath)
{
	u8 type=0; // map_Kd - diffuse color texture map
	// map_Ks - specular color texture map
	// map_Ka - ambient color texture map
	// map_Ns - shininess texture map
	if (!strncmp(bufPtr,"map_bump",8) || !strncmp(bufPtr,"map_Bump",8) || !strncmp(bufPtr,"bump",4))
		type=1; // normal map
	else if (!strncmp(bufPtr,"map_d",5) || !strncmp(bufPtr,"map_opacity",11))
		type=2; // opacity map
	else if (!strncmp(bufPtr,"map_refl",8))
		type=3; // reflection map
	// extract new material's name
	c8 textureNameBuf[WORD_BUFFER_LENGTH];
	bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);

	f32 bumpiness = 6.0f;
	bool clamp = false;
	core::vector3df tscale(1.f);  //For  map_Kd texture scaling
	core::vector3df tpos(0.f);    //For  map_Kd texture translation
	// handle options
	while (textureNameBuf[0]=='-')
	{
		if (!strncmp(bufPtr,"-bm",3))
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			currMaterial->Meshbuffer->Material.MaterialTypeParam=core::fast_atof(textureNameBuf);
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			continue;
		}
		else
		if (!strncmp(bufPtr,"-blendu",7))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-blendv",7))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-cc",3))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-clamp",6))
			bufPtr = readBool(bufPtr, clamp, bufEnd);
		else
		if (!strncmp(bufPtr,"-texres",7))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-type",5))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-mm",3))
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		}
		else
		if (!strncmp(bufPtr,"-o",2)) // texture coord translation
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1])))
				tpos.X = core::fast_atof(textureNameBuf);

			// next parameters are optional, so skip rest of loop if no number is found
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!(core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1]))))
				continue;
			tpos.Y = core::fast_atof(textureNameBuf);
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!(core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1]))))
				continue;
			tpos.Z = core::fast_atof(textureNameBuf);
		}
		else
		if (!strncmp(bufPtr,"-s",2)) // texture coord scale
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1])))
				tscale.X = core::fast_atof(textureNameBuf);

			// next parameters are optional, so skip rest of loop if no number is found
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!(core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1]))))
				continue;
			tscale.Y = core::fast_atof(textureNameBuf);
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!(core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1]))))
				continue;
			tscale.Z = core::fast_atof(textureNameBuf);
		}
		else
		if (!strncmp(bufPtr,"-t",2))
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			// next parameters are optional, so skip rest of loop if no number is found
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!(core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1]))))
				continue;
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!(core::isdigit(textureNameBuf[0]) || (textureNameBuf[0] == '-' && core::isdigit(textureNameBuf[1]))))
				continue;
		}
		// get next word
		bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	}

	if ((type==1) && (core::isdigit(textureNameBuf[0])))
	{
		// Haven't found that in any official mtl description, usually bump parameter should only be after -bm
		// But I'll leave it (with added checks in 1.9) as maybe there are some exporters doing this and likely can't be a valid filename
		const char *out=0;
		irr::f32 bumpScale = core::fast_atof(textureNameBuf, &out);
		if ( *out == 0 ) // name is only a number
		{
			currMaterial->Meshbuffer->Material.MaterialTypeParam=bumpScale;
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		}
	}
	if (clamp)
		currMaterial->Meshbuffer->Material.setFlag(video::EMF_TEXTURE_WRAP, video::ETC_CLAMP);

	io::path texname(textureNameBuf);
	if (texname.size() && getMeshTextureLoader())
	{
		video::ITexture * texture = getMeshTextureLoader()->getTexture(texname);
		if ( texture )
		{
			if (type==0)
			{
				currMaterial->Meshbuffer->Material.setTexture(0, texture);
				bool needsTextureMatrix = tscale != core::vector3df(1.f) || tpos != core::vector3df(0.f);
				if (needsTextureMatrix)
				{
					currMaterial->Meshbuffer->Material.getTextureMatrix(0).setTextureScale(tscale.X, tscale.Y);
					currMaterial->Meshbuffer->Material.getTextureMatrix(0).setTextureTranslate(tpos.X, tpos.Y);
				}
			}
			else if (type==1)
			{
				if ( texture->getSource() == video::ETS_FROM_FILE)
					SceneManager->getVideoDriver()->makeNormalMapTexture(texture, bumpiness);
				currMaterial->Meshbuffer->Material.setTexture(1, texture);
				currMaterial->Meshbuffer->Material.MaterialType=video::EMT_PARALLAX_MAP_SOLID;
				currMaterial->Meshbuffer->Material.MaterialTypeParam=0.035f;
			}
			else if (type==2)
			{
				currMaterial->Meshbuffer->Material.setTexture(0, texture);
				currMaterial->Meshbuffer->Material.MaterialType=video::EMT_TRANSPARENT_ADD_COLOR;
			}
			else if (type==3)
			{
	//			currMaterial->Meshbuffer->Material.Textures[1] = texture;
	//			currMaterial->Meshbuffer->Material.MaterialType=video::EMT_REFLECTION_2_LAYER;
			}
			// Set diffuse material color to white so as not to affect texture color
			// Because Maya set diffuse color Kd to black when you use a diffuse color map
			// But is this the right thing to do?
			currMaterial->Meshbuffer->Material.DiffuseColor.set(
				currMaterial->Meshbuffer->Material.DiffuseColor.getAlpha(), 255, 255, 255 );
		}
	}
	return bufPtr;
}


void COBJMeshFileLoader::readMTL(const c8* fileName, const io::path& relPath)
{
	const io::path realFile(fileName);
	io::IReadFile * mtlReader;

	if (FileSystem->existFile(realFile))
		mtlReader = FileSystem->createAndOpenFile(realFile);
	else if (FileSystem->existFile(relPath + realFile))
		mtlReader = FileSystem->createAndOpenFile(relPath + realFile);
	else if (FileSystem->existFile(FileSystem->getFileBasename(realFile)))
		mtlReader = FileSystem->createAndOpenFile(FileSystem->getFileBasename(realFile));
	else
		mtlReader = FileSystem->createAndOpenFile(relPath + FileSystem->getFileBasename(realFile));
	if (!mtlReader)	// fail to open and read file
	{
		os::Printer::log("Could not open material file", realFile, ELL_WARNING);
		return;
	}

	if ( getMeshTextureLoader() )
	{
		getMeshTextureLoader()->setMaterialFile(mtlReader);
		if ( SceneManager->getParameters()->existsAttribute(OBJ_TEXTURE_PATH) )
			getMeshTextureLoader()->setTexturePath(SceneManager->getParameters()->getAttributeAsString(OBJ_TEXTURE_PATH));
	}

	const long filesize = mtlReader->getSize();
	if (!filesize)
	{
		os::Printer::log("Skipping empty material file", realFile, ELL_INFORMATION);	// it's fine some tools export empty mtl files
		mtlReader->drop();
		return;
	}

	c8* buf = new c8[filesize];
	mtlReader->read((void*)buf, filesize);
	const c8* bufEnd = buf+filesize;

	SObjMtl* currMaterial = 0;

	const c8* bufPtr = buf;
	while(bufPtr != bufEnd)
	{
		switch(*bufPtr)
		{
			case 'n': // newmtl
			{
				// if there's an existing material, store it first
				if ( currMaterial )
					Materials.push_back( currMaterial );

				// extract new material's name
				c8 mtlNameBuf[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(mtlNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);

				currMaterial = new SObjMtl(getIndexTypeHint());
				currMaterial->Name = mtlNameBuf;
			}
			break;
			case 'i': // illum - illumination
			if ( currMaterial )
			{
				const u32 COLOR_BUFFER_LENGTH = 16;
				c8 illumStr[COLOR_BUFFER_LENGTH];

				bufPtr = goAndCopyNextWord(illumStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
				currMaterial->Illumination = (c8)atol(illumStr);
			}
			break;
			case 'N':
			if ( currMaterial )
			{
				switch(bufPtr[1])
				{
				case 's': // Ns - shininess
					{
						const u32 COLOR_BUFFER_LENGTH = 16;
						c8 nsStr[COLOR_BUFFER_LENGTH];

						bufPtr = goAndCopyNextWord(nsStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
						f32 shininessValue = core::fast_atof(nsStr);

						// wavefront shininess is from [0, 1000], so scale for OpenGL
						shininessValue *= 0.128f;
						currMaterial->Meshbuffer->Material.Shininess = shininessValue;
					}
				break;
				case 'i': // Ni - refraction index
					{
						c8 tmpbuf[WORD_BUFFER_LENGTH];
						bufPtr = goAndCopyNextWord(tmpbuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
					}
				break;
				}
			}
			break;
			case 'K':
			if ( currMaterial )
			{
				switch(bufPtr[1])
				{
				case 'd':		// Kd = diffuse
					{
						bufPtr = readColor(bufPtr, currMaterial->Meshbuffer->Material.DiffuseColor, bufEnd);

					}
					break;

				case 's':		// Ks = specular
					{
						bufPtr = readColor(bufPtr, currMaterial->Meshbuffer->Material.SpecularColor, bufEnd);
					}
					break;

				case 'a':		// Ka = ambience
					{
						bufPtr=readColor(bufPtr, currMaterial->Meshbuffer->Material.AmbientColor, bufEnd);
					}
					break;
				case 'e':		// Ke = emissive
					{
						currMaterial->Meshbuffer->Material.EmissiveColor.setAlpha(255);
						bufPtr=readColor(bufPtr, currMaterial->Meshbuffer->Material.EmissiveColor, bufEnd);
					}
					break;
				}	// end switch(bufPtr[1])
			}	// end case 'K': if ( 0 != currMaterial )...
			break;
			case 'b': // bump
			case 'm': // texture maps
			if (currMaterial)
			{
				bufPtr=readTextures(bufPtr, bufEnd, currMaterial, relPath);
			}
			break;
			case 'd': // d - transparency
			if ( currMaterial )
			{
				const u32 COLOR_BUFFER_LENGTH = 16;
				c8 dStr[COLOR_BUFFER_LENGTH];

				bufPtr = goAndCopyNextWord(dStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
				f32 dValue = core::fast_atof(dStr);

				currMaterial->Meshbuffer->Material.DiffuseColor.setAlpha( (s32)(dValue * 255) );
				if (dValue<1.0f)
					currMaterial->Meshbuffer->Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
			}
			break;
			case 'T':
			if ( currMaterial )
			{
				switch ( bufPtr[1] )
				{
				case 'f':		// Tf - Transmitivity
					const u32 COLOR_BUFFER_LENGTH = 16;
					c8 redStr[COLOR_BUFFER_LENGTH];
					c8 greenStr[COLOR_BUFFER_LENGTH];
					c8 blueStr[COLOR_BUFFER_LENGTH];

					bufPtr = goAndCopyNextWord(redStr,   bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
					bufPtr = goAndCopyNextWord(greenStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
					bufPtr = goAndCopyNextWord(blueStr,  bufPtr, COLOR_BUFFER_LENGTH, bufEnd);

					f32 transparency = ( core::fast_atof(redStr) + core::fast_atof(greenStr) + core::fast_atof(blueStr) ) / 3;

					currMaterial->Meshbuffer->Material.DiffuseColor.setAlpha( (s32)(transparency * 255) );
					if (transparency < 1.0f)
						currMaterial->Meshbuffer->Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
				}
			}
			break;
			default: // comments or not recognised
			break;
		} // end switch(bufPtr[0])
		// go to next line
		bufPtr = goNextLine(bufPtr, bufEnd);
	}	// end while (bufPtr)

	// end of file. if there's an existing material, store it
	if ( currMaterial )
		Materials.push_back( currMaterial );

	delete [] buf;
	mtlReader->drop();
}


//! Read RGB color
const c8* COBJMeshFileLoader::readColor(const c8* bufPtr, video::SColor& color, const c8* const bufEnd)
{
	const u32 COLOR_BUFFER_LENGTH = 16;
	c8 colStr[COLOR_BUFFER_LENGTH];

	bufPtr = goAndCopyNextWord(colStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
	color.setRed((u32)core::round32(core::fast_atof(colStr)*255.f));
	bufPtr = goAndCopyNextWord(colStr,   bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
	color.setGreen((u32)core::round32(core::fast_atof(colStr)*255.f));
	bufPtr = goAndCopyNextWord(colStr,   bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
	color.setBlue((u32)core::round32(core::fast_atof(colStr)*255.f));
	return bufPtr;
}


//! Read 3d vector of floats
const c8* COBJMeshFileLoader::readVec3(const c8* bufPtr, core::vector3df& vec, const c8* const bufEnd)
{
	const u32 WORD_BUFFER_LENGTH = 256;
	c8 wordBuffer[WORD_BUFFER_LENGTH];

	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.X=-core::fast_atof(wordBuffer); // change handedness
	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.Y=core::fast_atof(wordBuffer);
	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.Z=core::fast_atof(wordBuffer);
	return bufPtr;
}


//! Read 2d vector of floats
const c8* COBJMeshFileLoader::readUV(const c8* bufPtr, core::vector2df& vec, const c8* const bufEnd)
{
	const u32 WORD_BUFFER_LENGTH = 256;
	c8 wordBuffer[WORD_BUFFER_LENGTH];

	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.X=core::fast_atof(wordBuffer);
	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.Y=1-core::fast_atof(wordBuffer); // change handedness
	return bufPtr;
}


//! Read boolean value represented as 'on' or 'off'
const c8* COBJMeshFileLoader::readBool(const c8* bufPtr, bool& tf, const c8* const bufEnd)
{
	const u32 BUFFER_LENGTH = 8;
	c8 tfStr[BUFFER_LENGTH];

	bufPtr = goAndCopyNextWord(tfStr, bufPtr, BUFFER_LENGTH, bufEnd);
	tf = strcmp(tfStr, "off") != 0;
	return bufPtr;
}


COBJMeshFileLoader::SObjMtl* COBJMeshFileLoader::findMtl(const core::stringc& mtlName, const core::stringc& grpName)
{
	COBJMeshFileLoader::SObjMtl* defMaterial = 0;
	// search existing Materials for best match
	// exact match does return immediately, only name match means a new group
	for (u32 i = 0; i < Materials.size(); ++i)
	{
		if ( Materials[i]->Name == mtlName )
		{
			if ( Materials[i]->Group == grpName )
				return Materials[i];
			else
				defMaterial = Materials[i];
		}
	}
	// we found a partial match
	if (defMaterial)
	{
		Materials.push_back(new SObjMtl(*defMaterial));
		Materials.getLast()->Group = grpName;
		return Materials.getLast();
	}
	// we found a new group for a non-existent material
	else if (grpName.size())
	{
		Materials.push_back(new SObjMtl(*Materials[0]));
		Materials.getLast()->Group = grpName;
		return Materials.getLast();
	}
	return 0;
}


//! skip space characters and stop on first non-space
const c8* COBJMeshFileLoader::goFirstWord(const c8* buf, const c8* const bufEnd, bool acrossNewlines)
{
	// skip space characters
	if (acrossNewlines)
		while((buf != bufEnd) && core::isspace(*buf))
			++buf;
	else
		while((buf != bufEnd) && core::isspace(*buf) && (*buf != '\n'))
			++buf;

	return buf;
}


//! skip current word and stop at beginning of next one
const c8* COBJMeshFileLoader::goNextWord(const c8* buf, const c8* const bufEnd, bool acrossNewlines)
{
	// skip current word
	while(( buf != bufEnd ) && !core::isspace(*buf))
		++buf;

	return goFirstWord(buf, bufEnd, acrossNewlines);
}


//! Read until line break is reached and stop at the next non-space character
const c8* COBJMeshFileLoader::goNextLine(const c8* buf, const c8* const bufEnd)
{
	// look for newline characters
	while(buf != bufEnd)
	{
		// found it, so leave
		if (*buf=='\n' || *buf=='\r')
			break;
		++buf;
	}
	return goFirstWord(buf, bufEnd);
}


u32 COBJMeshFileLoader::copyWord(c8* outBuf, const c8* const inBuf, u32 outBufLength, const c8* const bufEnd)
{
	if (!outBufLength)
		return 0;
	if (!inBuf)
	{
		*outBuf = 0;
		return 0;
	}

	u32 i = 0;
	while(inBuf[i])
	{
		if (core::isspace(inBuf[i]) || &(inBuf[i]) == bufEnd)
			break;
		++i;
	}

	u32 length = core::min_(i, outBufLength-1);
	for (u32 j=0; j<length; ++j)
		outBuf[j] = inBuf[j];

	outBuf[length] = 0;
	return length;
}


core::stringc COBJMeshFileLoader::copyLine(const c8* inBuf, const c8* bufEnd)
{
	if (!inBuf)
		return core::stringc();

	const c8* ptr = inBuf;
	while (ptr<bufEnd)
	{
		if (*ptr=='\n' || *ptr=='\r')	// not copying the line end character
			break;
		++ptr;
	}
	return core::stringc(inBuf, (u32)(ptr-inBuf));
}


const c8* COBJMeshFileLoader::goAndCopyNextWord(c8* outBuf, const c8* inBuf, u32 outBufLength, const c8* bufEnd)
{
	inBuf = goNextWord(inBuf, bufEnd, false);
	copyWord(outBuf, inBuf, outBufLength, bufEnd);
	return inBuf;
}


bool COBJMeshFileLoader::retrieveVertexIndices(c8* vertexData, s32* idx, const c8* bufEnd, u32 vbsize, u32 vtsize, u32 vnsize)
{
	c8 word[16] = "";
	const c8* p = goFirstWord(vertexData, bufEnd);
	u32 idxType = 0;	// 0 = posIdx, 1 = texcoordIdx, 2 = normalIdx

	u32 i = 0;
	while ( p != bufEnd )
	{
		if ( ( core::isdigit(*p)) || (*p == '-') )
		{
			// build up the number
			word[i++] = *p;
		}
		else if ( *p == '/' || *p == ' ' || *p == '\0' )
		{
			// number is completed. Convert and store it
			word[i] = '\0';
			// if no number was found index will become 0 and later on -1 by decrement
			idx[idxType] = core::strtol10(word);
			if (idx[idxType]<0)
			{
				switch (idxType)
				{
					case 0:
						idx[idxType] += vbsize;
						break;
					case 1:
						idx[idxType] += vtsize;
						break;
					case 2:
						idx[idxType] += vnsize;
						break;
				}
			}
			else
				idx[idxType]-=1;

			// reset the word
			word[0] = '\0';
			i = 0;

			// go to the next kind of index type
			if (*p == '/')
			{
				if ( ++idxType > 2 )
				{
					// error checking, shouldn't reach here unless file is wrong
					idxType = 0;
				}
			}
			else
			{
				// set all missing values to disable (=-1)
				while (++idxType < 3)
					idx[idxType]=-1;
				++p;
				break; // while
			}
		}

		// go to the next char
		++p;
	}

	return true;
}


void COBJMeshFileLoader::cleanUp()
{
	for (u32 i=0; i < Materials.size(); ++i )
	{
		Materials[i]->Meshbuffer->drop();
		delete Materials[i];
	}

	Materials.clear();
}


} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OBJ_LOADER_

