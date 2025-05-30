// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef IRR_C_VIDEO_OPEN_GL_H_INCLUDED
#define IRR_C_VIDEO_OPEN_GL_H_INCLUDED

#include "IrrCompileConfig.h"

#include "SIrrCreationParameters.h"

namespace irr
{
	class CIrrDeviceWin32;
	class CIrrDeviceLinux;
	class CIrrDeviceSDL;
	class CIrrDeviceMacOSX;
}

#ifdef _IRR_COMPILE_WITH_OPENGL_

#include "IMaterialRendererServices.h"
#include "CNullDriver.h"

#include "COpenGLExtensionHandler.h"
#include "IContextManager.h"

namespace irr
{

namespace video
{
	class IContextManager;

	class COpenGLDriver : public CNullDriver, public IMaterialRendererServices, public COpenGLExtensionHandler
	{
	public:
		// Information about active pipeline state (fixed function vs shader)
		enum E_OPENGL_ACTIVE_PIPELINE
		{
			EOAP_FIXED = 0, // fixed function pipeline.
			EOAP_SHADER, // programmable pipeline.
			EOAP_FIXED_TO_SHADER, // switch from fixed to programmable pipeline.
			EOAP_SHADER_TO_FIXED // switch from programmable to fixed pipeline.
		};

#if defined(_IRR_COMPILE_WITH_WINDOWS_DEVICE_) || defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_COMPILE_WITH_OSX_DEVICE_)
		COpenGLDriver(const SIrrlichtCreationParameters& params, io::IFileSystem* io, IContextManager* contextManager);
#endif

#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_
		COpenGLDriver(const SIrrlichtCreationParameters& params, io::IFileSystem* io, CIrrDeviceSDL* device);
#endif

		bool initDriver();

		//! destructor
		virtual ~COpenGLDriver();

		virtual bool beginScene(u16 clearFlag, SColor clearColor = SColor(255,0,0,0), f32 clearDepth = 1.f, u8 clearStencil = 0,
			const SExposedVideoData& videoData = SExposedVideoData(), core::rect<s32>* sourceRect = 0) IRR_OVERRIDE;

		virtual bool endScene() IRR_OVERRIDE;

		//! sets transformation
		virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat) IRR_OVERRIDE;


		struct SHWBufferLink_opengl : public SHWBufferLink
		{
			SHWBufferLink_opengl(const scene::IMeshBuffer *_MeshBuffer): SHWBufferLink(_MeshBuffer), vbo_verticesID(0),vbo_indicesID(0){}

			GLuint vbo_verticesID; //tmp
			GLuint vbo_indicesID; //tmp

			GLuint vbo_verticesSize; //tmp
			GLuint vbo_indicesSize; //tmp
		};

		//! updates hardware buffer if needed
		virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer) IRR_OVERRIDE;

		//! Create hardware buffer from mesh
		virtual SHWBufferLink *createHardwareBuffer(const scene::IMeshBuffer* mb) IRR_OVERRIDE;

		//! Delete hardware buffer (only some drivers can)
		virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer) IRR_OVERRIDE;

		//! Draw hardware buffer
		virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer) IRR_OVERRIDE;

		//! Create occlusion query.
		/** Use node for identification and mesh for occlusion test. */
		virtual void addOcclusionQuery(scene::ISceneNode* node,
				const scene::IMesh* mesh=0) IRR_OVERRIDE;

		//! Remove occlusion query.
		virtual void removeOcclusionQuery(scene::ISceneNode* node) IRR_OVERRIDE;

		//! Run occlusion query. Draws mesh stored in query.
		/** If the mesh shall not be rendered visible, use
		overrideMaterial to disable the color and depth buffer. */
		virtual void runOcclusionQuery(scene::ISceneNode* node, bool visible=false) IRR_OVERRIDE;

		//! Update occlusion query. Retrieves results from GPU.
		/** If the query shall not block, set the flag to false.
		Update might not occur in this case, though */
		virtual void updateOcclusionQuery(scene::ISceneNode* node, bool block=true) IRR_OVERRIDE;

		//! Return query result.
		/** Return value is the number of visible pixels/fragments.
		The value is a safe approximation, i.e. can be larger then the
		actual value of pixels. */
		virtual u32 getOcclusionQueryResult(const scene::ISceneNode* node) const IRR_OVERRIDE;

		//! Create render target.
		virtual IRenderTarget* addRenderTarget() IRR_OVERRIDE;

		//! draws a vertex primitive list
		virtual void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
				const void* indexList, u32 primitiveCount,
				E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) IRR_OVERRIDE;

		//! draws a vertex primitive list in 2d
		virtual void draw2DVertexPrimitiveList(const void* vertices, u32 vertexCount,
				const void* indexList, u32 primitiveCount,
				E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) IRR_OVERRIDE;

		//! queries the features of the driver, returns true if feature is available
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const IRR_OVERRIDE
		{
			return FeatureEnabled[feature] && COpenGLExtensionHandler::queryFeature(feature);
		}

		//! Disable a feature of the driver.
		virtual void disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag=true) IRR_OVERRIDE;

		//! Sets a material. All 3d drawing functions draw geometry now
		//! using this material.
		//! \param material: Material to be used from now on.
		virtual void setMaterial(const SMaterial& material) IRR_OVERRIDE;

		virtual void draw2DImage(const video::ITexture* texture, const core::position2d<s32>& destPos,
			const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
			SColor color = SColor(255, 255, 255, 255), bool useAlphaChannelOfTexture = false) IRR_OVERRIDE;

		virtual void draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
			const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
			const video::SColor* const colors = 0, bool useAlphaChannelOfTexture = false) IRR_OVERRIDE;

		// Was some helper function used in previous texture locking. Currently not accessible for users
		// and also no longer needed internally (unless we switch back to old texture lock code).
		// It seems to draw the texture to a fullscreen quad. Also allows drawing one side of a cubemap texture.
		void draw2DImageQuad(const video::ITexture* texture, u32 layer, bool flip);

		//! draws a set of 2d images, using a color and the alpha channel of the
		//! texture if desired.
		void draw2DImageBatch(const video::ITexture* texture,
				const core::array<core::position2d<s32> >& positions,
				const core::array<core::rect<s32> >& sourceRects,
				const core::rect<s32>* clipRect,
				SColor color,
				bool useAlphaChannelOfTexture) IRR_OVERRIDE;

		//! draws a set of 2d images, using a color and the alpha
		/** channel of the texture if desired. The images are drawn
		beginning at pos and concatenated in one line. All drawings
		are clipped against clipRect (if != 0).
		The subtextures are defined by the array of sourceRects
		and are chosen by the indices given.
		\param texture: Texture to be drawn.
		\param pos: Upper left 2d destination position where the image will be drawn.
		\param sourceRects: Source rectangles of the image.
		\param indices: List of indices which choose the actual rectangle used each time.
		\param clipRect: Pointer to rectangle on the screen where the image is clipped to.
		This pointer can be 0. Then the image is not clipped.
		\param color: Color with which the image is colored.
		Note that the alpha component is used: If alpha is other than 255, the image will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of the texture is
		used to draw the image. */
		virtual void draw2DImageBatch(const video::ITexture* texture,
				const core::position2d<s32>& pos,
				const core::array<core::rect<s32> >& sourceRects,
				const core::array<s32>& indices,
				s32 kerningWidth=0,
				const core::rect<s32>* clipRect=0,
				SColor color=SColor(255,255,255,255),
				bool useAlphaChannelOfTexture=false) IRR_OVERRIDE;

		//! draw an 2d rectangle
		virtual void draw2DRectangle(SColor color, const core::rect<s32>& pos,
			const core::rect<s32>* clip = 0) IRR_OVERRIDE;

		//!Draws an 2d rectangle with a gradient.
		virtual void draw2DRectangle(const core::rect<s32>& pos,
			SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
			const core::rect<s32>* clip = 0) IRR_OVERRIDE;

		//! Draws a 2d line.
		virtual void draw2DLine(const core::position2d<s32>& start,
					const core::position2d<s32>& end,
					SColor color=SColor(255,255,255,255)) IRR_OVERRIDE;

		//! Draws a single pixel
		virtual void drawPixel(u32 x, u32 y, const SColor & color) IRR_OVERRIDE;

		//! Draws a 3d box
		virtual void draw3DBox( const core::aabbox3d<f32>& box, SColor color = SColor(255,255,255,255 ) ) IRR_OVERRIDE;

		//! Draws a 3d line.
		virtual void draw3DLine(const core::vector3df& start,
					const core::vector3df& end,
					SColor color = SColor(255,255,255,255)) IRR_OVERRIDE;

		//! \return Returns the name of the video driver. Example: In case of the Direct3D8
		//! driver, it would return "Direct3D8.1".
		virtual const wchar_t* getName() const IRR_OVERRIDE;

		//! deletes all dynamic lights there are
		virtual void deleteAllDynamicLights() IRR_OVERRIDE;

		//! adds a dynamic light, returning an index to the light
		//! \param light: the light data to use to create the light
		//! \return An index to the light, or -1 if an error occurs
		virtual s32 addDynamicLight(const SLight& light) IRR_OVERRIDE;

		//! Turns a dynamic light on or off
		//! \param lightIndex: the index returned by addDynamicLight
		//! \param turnOn: true to turn the light on, false to turn it off
		virtual void turnLightOn(s32 lightIndex, bool turnOn) IRR_OVERRIDE;

		//! returns the maximal amount of dynamic lights the device can handle
		virtual u32 getMaximalDynamicLightAmount() const IRR_OVERRIDE;

		//! Sets the dynamic ambient light color. The default color is
		//! (0,0,0,0) which means it is dark.
		//! \param color: New color of the ambient light.
		virtual void setAmbientLight(const SColorf& color) IRR_OVERRIDE;

		//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
		//! this: First, draw all geometry. Then use this method, to draw the shadow
		//! volume. Then, use IVideoDriver::drawStencilShadow() to visualize the shadow.
		virtual void drawStencilShadowVolume(const core::array<core::vector3df>& triangles, bool zfail, u32 debugDataVisible=0) IRR_OVERRIDE;

		//! Fills the stencil shadow with color. After the shadow volume has been drawn
		//! into the stencil buffer using IVideoDriver::drawStencilShadowVolume(), use this
		//! to draw the color of the shadow.
		virtual void drawStencilShadow(bool clearStencilBuffer=false,
			video::SColor leftUpEdge = video::SColor(0,0,0,0),
			video::SColor rightUpEdge = video::SColor(0,0,0,0),
			video::SColor leftDownEdge = video::SColor(0,0,0,0),
			video::SColor rightDownEdge = video::SColor(0,0,0,0)) IRR_OVERRIDE;

		//! sets a viewport
		virtual void setViewPort(const core::rect<s32>& area, bool clipToRenderTarget=true) IRR_OVERRIDE;

		//! Sets the fog mode.
		virtual void setFog(SColor color, E_FOG_TYPE fogType, f32 start,
			f32 end, f32 density, bool pixelFog, bool rangeFog) IRR_OVERRIDE;

		//! Only used by the internal engine. Used to notify the driver that
		//! the window was resized.
		virtual void OnResize(const core::dimension2d<u32>& size) IRR_OVERRIDE;

		//! Returns type of video driver
		virtual E_DRIVER_TYPE getDriverType() const IRR_OVERRIDE;

		//! get color format of the current color buffer
		virtual ECOLOR_FORMAT getColorFormat() const IRR_OVERRIDE;

		//! Returns the transformation set by setTransform
		virtual const core::matrix4& getTransform(E_TRANSFORMATION_STATE state) const IRR_OVERRIDE;

		//! Can be called by an IMaterialRenderer to make its work easier.
		void setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial,
			bool resetAllRenderstates);

		//! Compare in SMaterial doesn't check texture parameters, so we should call this on each OnRender call.
		virtual void setTextureRenderStates(const SMaterial& material, bool resetAllRenderstates);

		//! Get a vertex shader constant index.
		virtual s32 getVertexShaderConstantID(const c8* name) IRR_OVERRIDE;

		//! Get a pixel shader constant index.
		virtual s32 getPixelShaderConstantID(const c8* name) IRR_OVERRIDE;

		//! Sets a vertex shader constant.
		virtual void setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1) IRR_OVERRIDE;

		//! Sets a pixel shader constant.
		virtual void setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1) IRR_OVERRIDE;

		//! Sets a constant for the vertex shader based on an index.
		virtual bool setVertexShaderConstant(s32 index, const f32* floats, int count) IRR_OVERRIDE;

		//! Int interface for the above.
		virtual bool setVertexShaderConstant(s32 index, const s32* ints, int count) IRR_OVERRIDE;

		//! Uint interface for the above.
		virtual bool setVertexShaderConstant(s32 index, const u32* ints, int count) IRR_OVERRIDE;

		//! Sets a constant for the pixel shader based on an index.
		virtual bool setPixelShaderConstant(s32 index, const f32* floats, int count) IRR_OVERRIDE;

		//! Int interface for the above.
		virtual bool setPixelShaderConstant(s32 index, const s32* ints, int count) IRR_OVERRIDE;

		//! Uint interface for the above.
		virtual bool setPixelShaderConstant(s32 index, const u32* ints, int count) IRR_OVERRIDE;

		//! disables all textures beginning with the optional fromStage parameter. Otherwise all texture stages are disabled.
		//! Returns whether disabling was successful or not.
		bool disableTextures(u32 fromStage=0);

		//! Adds a new material renderer to the VideoDriver, using
		//! extGLGetObjectParameteriv(shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &status)
		//! pixel and/or vertex shaders to render geometry.
		virtual s32 addShaderMaterial(const c8* vertexShaderProgram, const c8* pixelShaderProgram,
			IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial, s32 userData) IRR_OVERRIDE;

		//! Adds a new material renderer to the VideoDriver, using GLSL to render geometry.
		virtual s32 addHighLevelShaderMaterial(
				const c8* vertexShaderProgram,
				const c8* vertexShaderEntryPointName,
				E_VERTEX_SHADER_TYPE vsCompileTarget,
				const c8* pixelShaderProgram,
				const c8* pixelShaderEntryPointName,
				E_PIXEL_SHADER_TYPE psCompileTarget,
				const c8* geometryShaderProgram,
				const c8* geometryShaderEntryPointName = "main",
				E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
				scene::E_PRIMITIVE_TYPE inType = scene::EPT_TRIANGLES,
				scene::E_PRIMITIVE_TYPE outType = scene::EPT_TRIANGLE_STRIP,
				u32 verticesOut = 0,
				IShaderConstantSetCallBack* callback = 0,
				E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
				s32 userData = 0) IRR_OVERRIDE;

		//! Returns a pointer to the IVideoDriver interface. (Implementation for
		//! IMaterialRendererServices)
		virtual IVideoDriver* getVideoDriver() IRR_OVERRIDE;

		//! Returns the maximum amount of primitives (mostly vertices) which
		//! the device is able to render with one drawIndexedTriangleList
		//! call.
		virtual u32 getMaximalPrimitiveCount() const IRR_OVERRIDE;

		virtual ITexture* addRenderTargetTexture(const core::dimension2d<u32>& size,
				const io::path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN) IRR_OVERRIDE;

		//! Creates a render target texture for a cubemap
		ITexture* addRenderTargetTextureCubemap(const irr::u32 sideLen,
				const io::path& name, const ECOLOR_FORMAT format) IRR_OVERRIDE;

		virtual bool setRenderTargetEx(IRenderTarget* target, u16 clearFlag, SColor clearColor = SColor(255,0,0,0),
			f32 clearDepth = 1.f, u8 clearStencil = 0) IRR_OVERRIDE;

		virtual void clearBuffers(u16 flag, SColor color = SColor(255,0,0,0), f32 depth = 1.f, u8 stencil = 0) IRR_OVERRIDE;

		//! Returns an image created from the last rendered frame.
		virtual IImage* createScreenShot(video::ECOLOR_FORMAT format=video::ECF_UNKNOWN, video::E_RENDER_TARGET target=video::ERT_FRAME_BUFFER) IRR_OVERRIDE;

		//! checks if an OpenGL error has happened and prints it (+ some internal code which is usually the line number)
		//! for performance reasons only available in debug mode
		bool testGLError(int code=0);

		//! Set/unset a clipping plane.
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param plane: The plane itself.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual bool setClipPlane(u32 index, const core::plane3df& plane, bool enable=false) IRR_OVERRIDE;

		//! Enable/disable a clipping plane.
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual void enableClipPlane(u32 index, bool enable) IRR_OVERRIDE;

		//! Enable the 2d override material
		virtual void enableMaterial2D(bool enable=true) IRR_OVERRIDE;

		//! Returns the graphics card vendor name.
		virtual core::stringc getVendorInfo() IRR_OVERRIDE {return VendorName;}

		//! Returns the maximum texture size supported.
		virtual core::dimension2du getMaxTextureSize() const IRR_OVERRIDE;

		//! Removes a texture from the texture cache and deletes it, freeing lot of memory.
		virtual void removeTexture(ITexture* texture) IRR_OVERRIDE;

		//! Check if the driver supports creating textures with the given color format
		virtual bool queryTextureFormat(ECOLOR_FORMAT format) const IRR_OVERRIDE;

		//! Used by some SceneNodes to check if a material should be rendered in the transparent render pass
		virtual bool needsTransparentRenderPass(const irr::video::SMaterial& material) const IRR_OVERRIDE;

		//! Convert E_PRIMITIVE_TYPE to OpenGL equivalent
		GLenum primitiveTypeToGL(scene::E_PRIMITIVE_TYPE type) const;

		//! Convert E_BLEND_FACTOR to OpenGL equivalent
		GLenum getGLBlend(E_BLEND_FACTOR factor) const;

		//! Get ZBuffer bits.
		GLenum getZBufferBits() const;

		bool getColorFormatParameters(ECOLOR_FORMAT format, GLint& internalFormat, GLenum& pixelFormat,
			GLenum& pixelType, void(**converter)(const void*, u32, void*)) const;

		//! Return info about active pipeline state.
		E_OPENGL_ACTIVE_PIPELINE getActivePipelineState() const;

		//! Set info about active pipeline state.
		void setActivePipelineState(E_OPENGL_ACTIVE_PIPELINE state);

		//! Get current material.
		const SMaterial& getCurrentMaterial() const;

		//! Rest renderstates forcing stuff like OnSetMaterial to be called
		void DoResetRenderStates() 
		{
			ResetRenderStates = true;
		}

		COpenGLCacheHandler* getCacheHandler() const;

	private:

		bool updateVertexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);
		bool updateIndexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);

		void uploadClipPlane(u32 index);

		//! inits the parts of the open gl driver used on all platforms
		bool genericDriverInit();

		virtual ITexture* createDeviceDependentTexture(const io::path& name, IImage* image) IRR_OVERRIDE;

		virtual ITexture* createDeviceDependentTextureCubemap(const io::path& name, const core::array<IImage*>& image) IRR_OVERRIDE;
		
		//! creates a transposed matrix in supplied GLfloat array to pass to OpenGL
		inline void getGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m);
		inline void getGLTextureMatrix(GLfloat gl_matrix[16], const core::matrix4& m);

		//! get native wrap mode value
		GLint getTextureWrapMode(const u8 clamp);

		//! sets the needed renderstates
		void setRenderStates3DMode();

		//! sets the needed renderstates
		void setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel);

		void createMaterialRenderers();

		//! Assign a hardware light to the specified requested light, if any
		//! free hardware lights exist.
		//! \param[in] lightIndex: the index of the requesting light
		void assignHardwareLight(u32 lightIndex);

		//! helper function for render setup.
		void getColorBuffer(const void* vertices, u32 vertexCount, E_VERTEX_TYPE vType);

		//! helper function doing the actual rendering.
		void renderArray(const void* indexList, u32 primitiveCount,
				scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		COpenGLCacheHandler* CacheHandler;

		core::stringw Name;
		core::matrix4 Matrices[ETS_COUNT];
		core::array<u8> ColorBuffer;

		//! enumeration for rendering modes such as 2d and 3d for minimizing the switching of renderStates.
		enum E_RENDER_MODE
		{
			ERM_NONE = 0,	// no render state has been set yet.
			ERM_2D,		// 2d drawing rendermode
			ERM_3D		// 3d rendering mode
		};

		E_RENDER_MODE CurrentRenderMode;
		//! bool to make all renderstates reset if set to true.
		bool ResetRenderStates;
		bool Transformation3DChanged;
		u8 AntiAlias;

		SMaterial Material, LastMaterial;

		struct SUserClipPlane
		{
			SUserClipPlane() : Enabled(false) {}
			core::plane3df Plane;
			bool Enabled;
		};
		core::array<SUserClipPlane> UserClipPlanes;

		core::stringc VendorName;

		core::matrix4 TextureFlipMatrix;

		//! Color buffer format
		ECOLOR_FORMAT ColorFormat;

		E_OPENGL_ACTIVE_PIPELINE ActivePipelineState;

		SIrrlichtCreationParameters Params;

		//! All the lights that have been requested; a hardware limited
		//! number of them will be used at once.
		struct RequestedLight
		{
			RequestedLight(SLight const & lightData)
				: LightData(lightData), HardwareLightIndex(-1), DesireToBeOn(true) { }

			SLight	LightData;
			s32	HardwareLightIndex; // GL_LIGHT0 - GL_LIGHT7
			bool	DesireToBeOn;
		};
		core::array<RequestedLight> RequestedLights;

		//! Built-in 2D quad for 2D rendering.
		S3DVertex Quad2DVertices[4];
		static const u16 Quad2DIndices[4];

		#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_
			CIrrDeviceSDL *SDLDevice;
		#endif

		IContextManager* ContextManager;

		E_DEVICE_TYPE DeviceType;
	};

} // end namespace video
} // end namespace irr


#endif // _IRR_COMPILE_WITH_OPENGL_
#endif
