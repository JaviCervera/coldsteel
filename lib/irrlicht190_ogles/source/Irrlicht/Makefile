VERSION_MAJOR = 1
VERSION_MINOR = 9
VERSION_RELEASE = 0
# Irrlicht Engine 1.9.0
# Makefile for Linux
#
# To use, just run:
#
# make
#
# This will compile Irrlicht, create a static lib (libIrrlicht.a), and copy it
# into the subdirectory lib/Linux. That's all.
#
# If you switch any settings (like debug/release/static/shared) then first run:
#
# make clean
#
# If you want Irrlicht to be compiled as shared lib (libIrrlicht.so.versionnumber), then run:
#
# make sharedlib
# make install
#
# If you want to compile in release mode run:
#
# make NDEBUG=1
#
# For cross-compilation for Win32 under Linux, just use the win32 targets. You have to set
# at least CXX, CC, and AR to the proper binaries.
#
# For compiling on MinGW you can run it with:
#
# make win32
#
# Or as MinGW by default has CC set to cc - but has no compiler by that name - you might have to do:
# make CC=gcc win32

#List of object files, separated based on engine architecture
IRRMESHLOADER = CBSPMeshFileLoader.o CMD2MeshFileLoader.o CMD3MeshFileLoader.o CMS3DMeshFileLoader.o CB3DMeshFileLoader.o C3DSMeshFileLoader.o COgreMeshFileLoader.o COBJMeshFileLoader.o CColladaFileLoader.o CCSMLoader.o CDMFLoader.o CLMTSMeshFileLoader.o CMY3DMeshFileLoader.o COCTLoader.o CXMeshFileLoader.o CIrrMeshFileLoader.o CSTLMeshFileLoader.o CLWOMeshFileLoader.o CPLYMeshFileLoader.o CSMFMeshFileLoader.o CMeshTextureLoader.o
IRRMESHWRITER = CColladaMeshWriter.o CIrrMeshWriter.o CSTLMeshWriter.o COBJMeshWriter.o CPLYMeshWriter.o CB3DMeshWriter.o
IRRMESHOBJ = $(IRRMESHLOADER) $(IRRMESHWRITER) \
	CSkinnedMesh.o CBoneSceneNode.o CMeshSceneNode.o \
	CAnimatedMeshSceneNode.o CAnimatedMeshMD2.o CAnimatedMeshMD3.o \
	CQ3LevelMesh.o CQuake3ShaderSceneNode.o CAnimatedMeshHalfLife.o
IRROBJ = CBillboardSceneNode.o CCameraSceneNode.o CDummyTransformationSceneNode.o CEmptySceneNode.o CGeometryCreator.o CLightSceneNode.o CMeshManipulator.o CMetaTriangleSelector.o COctreeSceneNode.o COctreeTriangleSelector.o CSceneCollisionManager.o CSceneManager.o CShadowVolumeSceneNode.o CSkyBoxSceneNode.o CSkyDomeSceneNode.o CTerrainSceneNode.o CTerrainTriangleSelector.o CVolumeLightSceneNode.o CCubeSceneNode.o CSphereSceneNode.o CTextSceneNode.o CTriangleBBSelector.o CTriangleSelector.o CWaterSurfaceSceneNode.o CMeshCache.o CDefaultSceneNodeAnimatorFactory.o CDefaultSceneNodeFactory.o CSceneLoaderIrr.o
IRRPARTICLEOBJ = CParticleAnimatedMeshSceneNodeEmitter.o CParticleBoxEmitter.o CParticleCylinderEmitter.o CParticleMeshEmitter.o CParticlePointEmitter.o CParticleRingEmitter.o CParticleSphereEmitter.o CParticleAttractionAffector.o CParticleFadeOutAffector.o CParticleGravityAffector.o CParticleRotationAffector.o CParticleSystemSceneNode.o CParticleScaleAffector.o
IRRANIMOBJ = CSceneNodeAnimatorCameraFPS.o CSceneNodeAnimatorCameraMaya.o CSceneNodeAnimatorCollisionResponse.o CSceneNodeAnimatorDelete.o CSceneNodeAnimatorFlyCircle.o CSceneNodeAnimatorFlyStraight.o CSceneNodeAnimatorFollowSpline.o CSceneNodeAnimatorRotation.o CSceneNodeAnimatorTexture.o
IRRDRVROBJ = CNullDriver.o COpenGLCacheHandler.o COpenGLDriver.o COpenGLNormalMapRenderer.o COpenGLParallaxMapRenderer.o COpenGLShaderMaterialRenderer.o COpenGLSLMaterialRenderer.o COpenGLExtensionHandler.o \
	CD3D9Driver.o CD3D9HLSLMaterialRenderer.o CD3D9NormalMapRenderer.o CD3D9ParallaxMapRenderer.o CD3D9ShaderMaterialRenderer.o CD3D9Texture.o \
	COGLESDriver.o COGLESExtensionHandler.o COGLES2Driver.o COGLES2ExtensionHandler.o COGLES2FixedPipelineRenderer.o COGLES2MaterialRenderer.o COGLES2NormalMapRenderer.o COGLES2ParallaxMapRenderer.o COGLES2Renderer2D.o CWebGL1Driver.o \
	CGLXManager.o CWGLManager.o CEGLManager.o
IRRIMAGEOBJ = CColorConverter.o CImage.o CImageLoaderBMP.o CImageLoaderDDS.o CImageLoaderJPG.o CImageLoaderPCX.o CImageLoaderPNG.o CImageLoaderPSD.o CImageLoaderPVR.o CImageLoaderTGA.o CImageLoaderPPM.o CImageLoaderWAL.o CImageLoaderRGB.o \
	CImageWriterBMP.o CImageWriterJPG.o CImageWriterPCX.o CImageWriterPNG.o CImageWriterPPM.o CImageWriterPSD.o CImageWriterTGA.o
IRRVIDEOOBJ = CVideoModeList.o CFPSCounter.o $(IRRDRVROBJ) $(IRRIMAGEOBJ)
IRRSWRENDEROBJ = CSoftwareDriver.o CSoftwareTexture.o CTRFlat.o CTRFlatWire.o CTRGouraud.o CTRGouraudWire.o  \
	CTRTextureFlat.o CTRTextureFlatWire.o CTRTextureGouraud.o CTRTextureGouraudAdd.o CTRTextureGouraudNoZ.o \
	CTRTextureGouraudWire.o CZBuffer.o
IRRBURNINGOBJ = CSoftwareDriver2.o CSoftwareTexture2.o CDepthBuffer.o IBurningShader.o CTRNormalMap.o CTRParallaxMap.o \
	CTRStencilShadow.o CTRTextureGouraudVertexAlpha2.o CTRTextureGouraudNoZ2.o CTRTextureLightMap2_M2.o \
	CTRTextureLightMap2_M1.o CTRTextureLightMapGouraud2_M4.o CTRTextureLightMap2_M4.o  CTRTextureGouraud2.o CTRGouraud2.o \
	CTRGouraudAlphaNoZ2.o CTRTextureDetailMap2.o CTRTextureGouraudAdd2.o CTRTextureGouraudAddNoZ2.o CTRTextureWire2.o \
	CTRTextureLightMap2_Add.o CTRTextureBlend.o CTRTextureGouraudAlpha.o burning_shader_color.o \
	CTRTextureGouraudAlphaNoZ.o  CBurningShader_Raster_Reference.o CTR_transparent_reflection_2_layer.o CTRGouraudNoZ2.o
IRRIOOBJ = CFileList.o CFileSystem.o CLimitReadFile.o CMemoryFile.o CReadFile.o CWriteFile.o CXMLReader.o CXMLWriter.o CWADReader.o CZipReader.o CPakReader.o CNPKReader.o CTarReader.o CMountPointReader.o irrXML.o CAttributes.o lzma/LzmaDec.o
IRROTHEROBJ = CIrrDeviceSDL.o CIrrDeviceLinux.o CIrrDeviceConsole.o CIrrDeviceStub.o CIrrDeviceWin32.o CIrrDeviceFB.o CLogger.o COSOperator.o Irrlicht.o os.o leakHunter.o 	CProfiler.o utf8.o
IRRGUIOBJ = CGUIButton.o CGUICheckBox.o CGUIComboBox.o CGUIContextMenu.o CGUIEditBox.o CGUIEnvironment.o CGUIFileOpenDialog.o CGUIFont.o CGUIImage.o CGUIInOutFader.o CGUIListBox.o CGUIMenu.o CGUIMeshViewer.o CGUIMessageBox.o CGUIModalScreen.o CGUIScrollBar.o CGUISpinBox.o CGUISkin.o CGUIStaticText.o CGUITabControl.o CGUITable.o CGUIToolBar.o CGUIWindow.o CGUIColorSelectDialog.o CDefaultGUIElementFactory.o CGUISpriteBank.o CGUIImageList.o CGUITreeView.o CGUIProfiler.o
ZLIBOBJ = zlib/adler32.o zlib/compress.o zlib/crc32.o zlib/deflate.o zlib/inffast.o zlib/inflate.o zlib/inftrees.o zlib/trees.o zlib/uncompr.o zlib/zutil.o
JPEGLIBOBJ = jpeglib/jcapimin.o jpeglib/jcapistd.o jpeglib/jccoefct.o jpeglib/jccolor.o jpeglib/jcdctmgr.o jpeglib/jchuff.o jpeglib/jcinit.o jpeglib/jcmainct.o jpeglib/jcmarker.o jpeglib/jcmaster.o jpeglib/jcomapi.o jpeglib/jcparam.o jpeglib/jcprepct.o jpeglib/jcsample.o jpeglib/jctrans.o jpeglib/jdapimin.o jpeglib/jdapistd.o jpeglib/jdatadst.o jpeglib/jdatasrc.o jpeglib/jdcoefct.o jpeglib/jdcolor.o jpeglib/jddctmgr.o jpeglib/jdhuff.o jpeglib/jdinput.o jpeglib/jdmainct.o jpeglib/jdmarker.o jpeglib/jdmaster.o jpeglib/jdmerge.o jpeglib/jdpostct.o jpeglib/jdsample.o jpeglib/jdtrans.o jpeglib/jerror.o jpeglib/jfdctflt.o jpeglib/jfdctfst.o jpeglib/jfdctint.o jpeglib/jidctflt.o jpeglib/jidctfst.o jpeglib/jidctint.o jpeglib/jmemmgr.o jpeglib/jmemnobs.o jpeglib/jquant1.o jpeglib/jquant2.o jpeglib/jutils.o jpeglib/jcarith.o jpeglib/jdarith.o jpeglib/jaricom.o
LIBPNGOBJ = libpng/png.o libpng/pngerror.o libpng/pngget.o libpng/pngmem.o libpng/pngpread.o libpng/pngread.o libpng/pngrio.o libpng/pngrtran.o libpng/pngrutil.o libpng/pngset.o libpng/pngtrans.o libpng/pngwio.o libpng/pngwrite.o libpng/pngwtran.o libpng/pngwutil.o
LIBAESGM = aesGladman/aescrypt.o aesGladman/aeskey.o aesGladman/aestab.o aesGladman/fileenc.o aesGladman/hmac.o aesGladman/prng.o aesGladman/pwd2key.o aesGladman/sha1.o aesGladman/sha2.o
BZIP2OBJ = bzip2/blocksort.o bzip2/huffman.o bzip2/crctable.o bzip2/randtable.o bzip2/bzcompress.o bzip2/decompress.o bzip2/bzlib.o

# Next variable is for additional scene nodes etc. of customized Irrlicht versions
EXTRAOBJ =
LINKOBJ = $(IRRMESHOBJ) $(IRROBJ) $(IRRPARTICLEOBJ) $(IRRANIMOBJ) \
	$(IRRVIDEOOBJ) $(IRRSWRENDEROBJ) $(IRRBURNINGOBJ) $(IRRIOOBJ) $(IRROTHEROBJ) \
	$(IRRGUIOBJ) $(ZLIBOBJ) $(JPEGLIBOBJ) $(LIBPNGOBJ) $(LIBAESGM) \
	$(BZIP2OBJ) $(EXTRAOBJ)

emscripten: EMSCRIPTEN=1

###############
#Compiler flags

CXXINCS = -I../../include -Izlib -Ijpeglib -Ilibpng
CPPFLAGS += $(CXXINCS) -DIRRLICHT_EXPORTS=1
CXXFLAGS += -Wall -pipe -fno-exceptions -fno-rtti
#CXXFLAGS += -std=gnu++11 -U__STRICT_ANSI__
ifndef NDEBUG
CPPFLAGS += -g -D_DEBUG
else
CPPFLAGS += -O3
endif
ifdef PROFILE
CPPFLAGS += -pg
endif
CFLAGS += -DPNG_THREAD_UNSAFE_OK -DPNG_NO_MMX_CODE -DPNG_NO_MNG_FEATURES -DPNG_ARM_NEON_OPT=0
ifdef EMSCRIPTEN
  CXXFLAGS += -std=gnu++11 -U__STRICT_ANSI__
  ifdef WASM
    # TODO: shouldn't this use CPPFLAGS?
    CXXFLAGS += -s WASM=1
  endif  
endif

sharedlib sharedlib_osx: CPPFLAGS += -fPIC
# TODO: also necessary on sharedlib_osx?
sharedlib: LDFLAGS += -fPIC

#multilib handling
BITS := $(shell getconf LONG_BIT)
ifneq ($(BITS), 32)
	LIBSELECT := $(BITS)
endif

#Linux specific options
staticlib sharedlib install: SYSTEM = Linux
STATIC_LIB = libIrrlicht.a
LIB_PATH = ../../lib/$(SYSTEM)
PREFIX ?= /usr/local
INSTALL_DIR ?= $(PREFIX)/lib$(LIBSELECT)
sharedlib install: SHARED_LIB = libIrrlicht.so
sharedlib: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm
staticlib sharedlib: CXXINCS += -I/usr/X11R6/include

#OSX specific options
staticlib_osx sharedlib_osx install_osx: SYSTEM = MacOSX
staticlib_osx sharedlib_osx: IRROTHEROBJ += MacOSX/CIrrDeviceMacOSX.o MacOSX/OSXClipboard.o MacOSX/AppDelegate.o
staticlib_osx sharedlib_osx: CXXINCS += -IMacOSX -I/usr/X11R6/include
sharedlib_osx install_osx: SHARED_LIB = libIrrlicht.dylib
staticlib_osx sharedlib_osx: LDFLAGS += --no-export-all-symbols --add-stdcall-alias
sharedlib_osx: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm
# for non-X11 app
#sharedlib_osx: LDFLAGS += -framework cocoa -framework carbon -framework opengl -framework IOKit

#Windows specific options
IRRLICHT_DLL := ../../bin/Win32-gcc/Irrlicht.dll
sharedlib_win32 staticlib_win32: SYSTEM = Win32-gcc
sharedlib_win32: LDFLAGS += -lgdi32 -lopengl32 -ld3dx9d -lwinmm -Wl,--add-stdcall-alias
#sharedlib_win32: LDFLAGS += -lSDL
#choose either -DIRR_COMPILE_WITH_DX9_DEV_PACK or -DNO_IRR_COMPILE_WITH_DIRECT3D_9_ depending if you need dx9
#sharedlib_win32 staticlib_win32: CPPFLAGS += -DIRR_COMPILE_WITH_DX9_DEV_PACK
sharedlib_win32 staticlib_win32: CPPFLAGS += -DNO_IRR_COMPILE_WITH_DIRECTINPUT_JOYSTICK_ -DNO_IRR_COMPILE_WITH_DIRECT3D_9_
staticlib_win32: CPPFLAGS += -D_IRR_STATIC_LIB_

#emscripten specific options
staticlib_emscripten: SYSTEM = emscripten


# You might want to disable features you don't need in Irrlicht here to reduce Irrlicht library.
# This can also help to reduce the .js size when compiling for emscripten.
# Note you can also disable the same features by modifying IrrCompileConfig.h which is slightly 
# safer as you have then the same setting when compiling Irrlicht and your project (but needs 
# modifying source-code while you can simply copy the Makefile and create your own settings)

# xml - use a lot, but also takes  a lot of space. Check if you have any formats using it.s
#CXXFLAGS += -DNO_IRR_COMPILE_WITH_XML_ 
# Get rid of encrypted zip files or bzip encrypted zip's or lzma encrypte zip's - or even all zip-file support.
#CXXFLAGS += -DNO_IRR_COMPILE_WITH_ZIP_ENCRYPTION_ -DNO_IRR_COMPILE_WITH_BZIP2_ -DNO_IRR_COMPILE_WITH_LZMA_ -DNO_IRR_COMPILE_WITH_ZLIB_
# Disable mesh writers 
#CXXFLAGS += -DNO_IRR_COMPILE_WITH_IRR_WRITER_ -DNO_IRR_COMPILE_WITH_COLLADA_WRITER_ -DNO_IRR_COMPILE_WITH_STL_WRITER_ 
#CXXFLAGS += -DNO_IRR_COMPILE_WITH_OBJ_WRITER_ -DNO_IRR_COMPILE_WITH_PLY_WRITER_ -DNO_IRR_COMPILE_WITH_B3D_WRITER_
# Disable image writers
#CXXFLAGS += -DNO_IRR_COMPILE_WITH_BMP_WRITER_ -DNO_IRR_COMPILE_WITH_JPG_WRITER_ -DNO_IRR_COMPILE_WITH_PCX_WRITER_
#CXXFLAGS += -DNO_IRR_COMPILE_WITH_PNG_WRITER_ -DNO_IRR_COMPILE_WITH_PPM_WRITER_ -DNO_IRR_COMPILE_WITH_TGA_WRITER_


STATIC_LIB = libIrrlicht.a
LIB_PATH = ../../lib/$(SYSTEM)
INSTALL_DIR = /usr/local/lib

VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_RELEASE)
SHARED_FULLNAME = $(SHARED_LIB).$(VERSION)
SONAME = $(SHARED_LIB).$(VERSION_MAJOR).$(VERSION_MINOR)

####################
# All target, builds Irrlicht as static lib (libIrrlicht.a) and copies it into lib/Linux
all linux : staticlib

emscripten: staticlib_emscripten

# Builds Irrlicht as shared lib (libIrrlicht.so.versionNumber) and copies it into lib/Linux
sharedlib: $(LINKOBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared -Wl,-soname,$(SONAME) -o $(SHARED_FULLNAME) $^ $(LDFLAGS)
	mkdir -p $(LIB_PATH)
	cp $(SHARED_FULLNAME) $(LIB_PATH)

# Builds Irrlicht as static lib (libIrrlicht.a)
$(STATIC_LIB): $(LINKOBJ)
	$(AR) rs $@ $^

# Copies static lib into ../../lib/($SYSTEM)
staticlib staticlib_osx staticlib_emscripten: $(STATIC_LIB)
	mkdir -p $(LIB_PATH)
	cp $^ $(LIB_PATH)

# Builds Irrlicht as dll (Irrlicht.dll) into ../../bin/Win32-gcc
all_win32 win32: sharedlib_win32
sharedlib_win32: $(IRRLICHT_DLL)
../../bin/Win32-gcc/Irrlicht.dll: $(LINKOBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS) -Wl,--out-implib,../../lib/Win32-gcc/$(STATIC_LIB)
# Copies static lib into /lib/Win32-gcc
staticlib_win32: $(STATIC_LIB)
	cp $^ $(LIB_PATH)

# Builds Irrlicht as shared lib (libIrrlicht.so.versionNumber) and copies it into /lib/MacOSX
sharedlib_osx: $(LINKOBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -dynamiclib -Wl,-install_name,$(SONAME) -o $(SHARED_FULLNAME) $^ $(LDFLAGS)
	cp $(SHARED_FULLNAME) $(LIB_PATH)

# Installs Irrlicht if it was created as shared lib
install install_osx:
	$(RM) -r $(INSTALL_DIR)/../include/irrlicht
	mkdir -p $(INSTALL_DIR)/../include/irrlicht
	cp ../../include/*.h $(INSTALL_DIR)/../include/irrlicht/
	cp $(LIB_PATH)/$(SHARED_FULLNAME) $(INSTALL_DIR)
	cd $(INSTALL_DIR) && ln -s -f $(SHARED_FULLNAME) $(SONAME)
	cd $(INSTALL_DIR) && ln -s -f $(SONAME) $(SHARED_LIB)
#	ldconfig -n $(INSTALL_DIR)

TAGS:
	ctags *.cpp ../../include/*.h *.h

# Create dependency files for automatic recompilation
%.d:%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ $<

# Create dependency files for automatic recompilation
%.d:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MM -MF $@ $<

# Create object files from objective-c code
%.o:%.mm
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

ifneq ($(MAKECMDGOALS),clean)
-include $(LINKOBJ:.o=.d)
endif

help:
	@echo "Available targets for Irrlicht"
	@echo " sharedlib: Build shared library Irrlicht.so for Linux"
	@echo " staticlib: Build static library Irrlicht.a for Linux"
	@echo " emscripten: Build static emscripen library on Linux"
	@echo " install: Copy shared library to $(INSTALL_DIR)"
	@echo ""
	@echo " sharedlib_win32: Build shared library Irrlicht.dll for Windows"
	@echo " staticlib_win32: Build static library Irrlicht.a for Windows"
	@echo ""
	@echo " clean: Clean up directory"

# Cleans all temporary files and compilation results.
clean:
	$(RM) $(LINKOBJ) $(SHARED_FULLNAME) $(STATIC_LIB) $(LINKOBJ:.o=.d)

.PHONY: all sharedlib staticlib sharedlib_win32 staticlib_win32 emscripten staticlib_emscripten help install clean
