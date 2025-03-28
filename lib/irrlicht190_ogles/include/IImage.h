// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef IRR_I_IMAGE_H_INCLUDED
#define IRR_I_IMAGE_H_INCLUDED

#include "IReferenceCounted.h"
#include "position2d.h"
#include "rect.h"
#include "SColor.h"
#include "irrAllocator.h"

namespace irr
{
namespace video
{

//! Interface for software image data.
/** Image loaders create these images from files. IVideoDrivers convert
these images into their (hardware) textures.
NOTE: Floating point formats are not well supported yet. Basically only getData() works for them.
*/
class IImage : public virtual IReferenceCounted
{
public:

	//! constructor
	IImage(ECOLOR_FORMAT format, const core::dimension2d<u32>& size, bool deleteMemory) :
		Format(format), Size(size), Data(0), MipMapsData(0), BytesPerPixel(0), Pitch(0), DeleteMemory(deleteMemory), DeleteMipMapsMemory(false)
#if defined(IRRLICHT_sRGB)
		,Format_sRGB(1)
#endif
	{
		BytesPerPixel = getBitsPerPixelFromFormat(Format) / 8;

		// We want the exact pitch even for compressed formats
		if ( Size.Height > 0 )
			Pitch = (irr::u32)(getDataSizeFromFormat(Format, Size.Width, Size.Height) / (size_t)Size.Height);
		else
			Pitch = 0;
	}

	//! destructor
	virtual ~IImage()
	{
		if (DeleteMemory)
			delete[] Data;

		if (DeleteMipMapsMemory)
			Allocator.deallocate(MipMapsData);
	}

	//! Returns the color format
	ECOLOR_FORMAT getColorFormat() const
	{
		return Format;
	}

#if defined(IRRLICHT_sRGB)
	//! Texture is linear/sRGB (should be part of ColorFormat: default yes)
	int get_sRGB() const
	{
		return Format_sRGB;
	}
	void set_sRGB(int val)
	{
		Format_sRGB = val;
	}
#endif

	//! Returns width and height of image data.
	const core::dimension2d<u32>& getDimension() const
	{
		return Size;
	}

	//! Returns bits per pixel.
	u32 getBitsPerPixel() const
	{
		return getBitsPerPixelFromFormat(Format);
	}

	//! Returns bytes per pixel for uncompressed formats
	/** Note: With compressed formats this value tends to be wrong.
	For those you usually have to work with either getBitsPerPixelFromFormat,
	getBitsPerBlockFromFormat or getPitch. Which one of those you'll need 
	depends on the use case. */
	u32 getBytesPerPixel() const
	{
		return BytesPerPixel;
	}

	//! Returns image data size in bytes
	size_t getImageDataSizeInBytes() const
	{
		return getDataSizeFromFormat(Format, Size.Width, Size.Height);
	}

	//! Returns image data size in pixels
	u32 getImageDataSizeInPixels() const
	{
		return Size.Width * Size.Height;
	}

	//! Returns pitch of image
	u32 getPitch() const
	{
		return Pitch;
	}

	//! Returns mask for red value of a pixel
	u32 getRedMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1F << 10;
		case ECF_R5G6B5:
			return 0x1F << 11;
		case ECF_R8G8B8:
			return 0x00FF0000;
		case ECF_A8R8G8B8:
			return 0x00FF0000;
		default:
			return 0x0;
		}
	}

	//! Returns mask for green value of a pixel
	u32 getGreenMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1F << 5;
		case ECF_R5G6B5:
			return 0x3F << 5;
		case ECF_R8G8B8:
			return 0x0000FF00;
		case ECF_A8R8G8B8:
			return 0x0000FF00;
		default:
			return 0x0;
		}
	}

	//! Returns mask for blue value of a pixel
	u32 getBlueMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1F;
		case ECF_R5G6B5:
			return 0x1F;
		case ECF_R8G8B8:
			return 0x000000FF;
		case ECF_A8R8G8B8:
			return 0x000000FF;
		default:
			return 0x0;
		}
	}

	//! Returns mask for alpha value of a pixel
	u32 getAlphaMask() const
	{
		switch (Format)
		{
		case ECF_A1R5G5B5:
			return 0x1 << 15;
		case ECF_R5G6B5:
			return 0x0;
		case ECF_R8G8B8:
			return 0x0;
		case ECF_A8R8G8B8:
			return 0xFF000000;
		default:
			return 0x0;
		}
	}

	//! Use this to get a pointer to the image data.
	/**
	\return Pointer to the image data. What type of data is pointed to
	depends on the color format of the image. For example if the color
	format is ECF_A8R8G8B8, it is of u32. */
	void* getData() const
	{
		return Data;
	}

	//! Lock function. Use this to get a pointer to the image data.
	/** Use getData instead.
	\return Pointer to the image data. What type of data is pointed to
	depends on the color format of the image. For example if the color
	format is ECF_A8R8G8B8, it is of u32. Be sure to call unlock() after
	you don't need the pointer any more. */
	IRR_DEPRECATED void* lock()
	{
		return getData();
	}

	//! Unlock function.
	/** Should be called after the pointer received by lock() is not
	needed anymore. */
	IRR_DEPRECATED void unlock()
	{
	}

	//! Get the mipmap size for this image for a certain mipmap level
	/** level 0 will be full image size. Every further level is half the size.
		Doesn't care if the image actually has mipmaps, just which size would be needed. */
	core::dimension2du getMipMapsSize(u32 mipmapLevel) const
	{
		return getMipMapsSize(Size, mipmapLevel);
	}


	//! Calculate mipmap size for a certain level
	/** level 0 will be full image size. Every further level is half the size.	*/
	static core::dimension2du getMipMapsSize(const core::dimension2du& sizeLevel0, u32 mipmapLevel)
	{
		core::dimension2du result(sizeLevel0);
		u32 i=0;
		while (i != mipmapLevel)
		{
			if (result.Width>1)
				result.Width >>= 1;
			if (result.Height>1)
				result.Height>>=1;
			++i;

			if ( result.Width == 1 && result.Height == 1 && i < mipmapLevel )
				return core::dimension2du(0,0);
		}
		return result;
	}


	//! Get mipmaps data.
	/** Note that different mip levels are just behind each other in memory block.
		So if you just get level 1 you also have the data for all other levels.
		There is no level 0 - use getData to get the original image data.
	*/
	void* getMipMapsData(irr::u32 mipLevel=1) const
	{
		if ( MipMapsData && mipLevel > 0)
		{
			size_t dataSize = 0;
			core::dimension2du mipSize(Size);
			u32 i = 1;	// We want the start of data for this level, not end.

			while (i != mipLevel)
			{
				if (mipSize.Width > 1)
					mipSize.Width >>= 1;

				if (mipSize.Height > 1)
					mipSize.Height >>= 1;

				dataSize += getDataSizeFromFormat(Format, mipSize.Width, mipSize.Height);

				++i;
				if ( mipSize.Width == 1 && mipSize.Height == 1 && i < mipLevel)
					return 0;
			}

			return MipMapsData + dataSize;
		}

		return 0;
	}

	//! Set mipmaps data.
	/** This method allows you to put custom mipmaps data for
	image.
	\param data A byte array with pixel color information
	\param ownForeignMemory If true, the image will use the data
	pointer directly and own it afterward. If false, the memory
	will by copied internally.
	\param deleteMemory Whether the memory is deallocated upon
	destruction. */
	void setMipMapsData(void* data, bool ownForeignMemory, bool deleteMemory)
	{
		if (data != MipMapsData)
		{
			if (DeleteMipMapsMemory)
			{
				Allocator.deallocate(MipMapsData);

				DeleteMipMapsMemory = false;
			}

			if (data)
			{
				if (ownForeignMemory)
				{
					MipMapsData = static_cast<u8*>(data);

					DeleteMipMapsMemory = deleteMemory;
				}
				else
				{
					size_t dataSize = 0;
					u32 width = Size.Width;
					u32 height = Size.Height;

					do
					{
						if (width > 1)
							width >>= 1;

						if (height > 1)
							height >>= 1;

						dataSize += getDataSizeFromFormat(Format, width, height);
					} while (width != 1 || height != 1);

					MipMapsData = Allocator.allocate(dataSize);
					memcpy(MipMapsData, data, dataSize);

					DeleteMipMapsMemory = true;
				}
			}
			else
			{
				MipMapsData = 0;
			}
		}
	}

	//! Returns a pixel
	virtual SColor getPixel(u32 x, u32 y) const = 0;

	//! Sets a pixel
	virtual void setPixel(u32 x, u32 y, const SColor &color, bool blend = false ) = 0;

	//! Copies the image into the target, scaling the image to fit
	/**	NOTE: mipmaps are ignored */
	virtual void copyToScaling(void* target, u32 width, u32 height, ECOLOR_FORMAT format=ECF_A8R8G8B8, u32 pitch=0) =0;

	//! Copies the image into the target, scaling the image to fit
	/**	NOTE: mipmaps are ignored */
	virtual void copyToScaling(IImage* target) =0;

	//! copies this surface into another
	/**	NOTE: mipmaps are ignored */
	virtual void copyTo(IImage* target, const core::position2d<s32>& pos=core::position2d<s32>(0,0)) =0;

	//! copies this surface into another
	/**	NOTE: mipmaps are ignored */
	virtual void copyTo(IImage* target, const core::position2d<s32>& pos, const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect=0) =0;

	//! copies this surface into another, using the alpha mask and cliprect and a color to add with
	/**	NOTE: mipmaps are ignored
	\param combineAlpha - When true then combine alpha channels. When false replace target image alpha with source image alpha.
	*/
	virtual void copyToWithAlpha(IImage* target, const core::position2d<s32>& pos,
			const core::rect<s32>& sourceRect, const SColor &color,
			const core::rect<s32>* clipRect = 0,
			bool combineAlpha=false) =0;

	//! copies this surface into another, scaling it to fit, applying a box filter
	/**	NOTE: mipmaps are ignored */
	virtual void copyToScalingBoxFilter(IImage* target, s32 bias = 0, bool blend = false) = 0;

	//! Flips (mirrors) the image in one or two directions
	/** \param topBottom Flip around central x-axis (vertical flipping)
	\param leftRight Flip around central y-axis (typical mirror, horizontal flipping) */
	virtual void flip(bool topBottom, bool leftRight) = 0;

	//! fills the surface with given color
	virtual void fill(const SColor &color) =0;

	//! Inform whether the image is compressed
	IRR_DEPRECATED bool isCompressed() const
	{
		return IImage::isCompressedFormat(Format);
	}

	//! Check whether the image has MipMaps
	/** \return True if image has MipMaps, else false. */
	IRR_DEPRECATED bool hasMipMaps() const
	{
		return (getMipMapsData() != 0);
	}

	//! get the amount of Bits per Pixel of the given color format
	static u32 getBitsPerPixelFromFormat(const ECOLOR_FORMAT format)
	{
		switch(format)
		{
		case ECF_A1R5G5B5:
			return 16;
		case ECF_R5G6B5:
			return 16;
		case ECF_R8G8B8:
			return 24;
		case ECF_A8R8G8B8:
			return 32;
		case ECF_DXT1:
			return 4;
		case ECF_DXT2:
		case ECF_DXT3:
		case ECF_DXT4:
		case ECF_DXT5:
			return 8;
		case ECF_PVRTC_RGB2:
		case ECF_PVRTC_ARGB2:
		case ECF_PVRTC2_ARGB2:
			return 2;
		case ECF_PVRTC_RGB4:
		case ECF_PVRTC_ARGB4:
		case ECF_PVRTC2_ARGB4:
			return 4;
		case ECF_ETC1:
			return 4;
		case ECF_ETC2_RGB:
			return 4;
		case ECF_ETC2_ARGB:
			return 8;
		case ECF_D16:
			return 16;
		case ECF_D32:
			return 32;
		case ECF_D24S8:
			return 32;
		case ECF_R8:
			return 8;
		case ECF_R8G8:
			return 16;
		case ECF_R16:
			return 16;
		case ECF_R16G16:
			return 32;
		case ECF_R16F:
			return 16;
		case ECF_G16R16F:
			return 32;
		case ECF_A16B16G16R16F:
			return 64;
		case ECF_R32F:
			return 32;
		case ECF_G32R32F:
			return 64;
		case ECF_A32B32G32R32F:
			return 128;
		default:
			return 0;
		}
	}

	// Some (compressed) formats need to ensure blocks of bits stay together, which may
	// not be identical to the amount of bits needed for a pixel.
	// Also a block of bits maybe not be about pixels in a row, but tends
	// to be about things like 4x4 pixel blocks. The order can even be unrelated to the 
	// pixel order in some cases.
	// For uncompressed formats it's the same value as getBitsPerPixelFromFormat
	static u32 getBitsPerBlockFromFormat(const ECOLOR_FORMAT format)
	{
		switch(format)
		{
		case ECF_DXT1:
			return 64;
		case ECF_DXT2:
		case ECF_DXT3:
		case ECF_DXT4:
		case ECF_DXT5:
			return 128;
		case ECF_PVRTC_RGB2:
		case ECF_PVRTC_ARGB2:
		case ECF_PVRTC2_ARGB2:
		case ECF_PVRTC_RGB4:
		case ECF_PVRTC_ARGB4:
		case ECF_PVRTC2_ARGB4:
			return 64;
		case ECF_ETC1:
			return 64;
		case ECF_ETC2_RGB:
			return 64;
		case ECF_ETC2_ARGB:
			return 128;
		default:
			return getBitsPerPixelFromFormat(format);
		}
	}

	//! You should not create images where the result of getDataSizeFromFormat doesn't pass this function
	/** Note that CImage does not yet check for this, but going beyond this limit is not supported well.
	Image loaders should check for this.
	If you don't have the format yet then checking width*height*bytes_per_pixel is mostly fine, but make
	sure to work with size_t so it doesn't clip the result to u32 too early.
	\return true when dataSize is small enough that it should be fine. */
	static bool checkDataSizeLimit(size_t dataSize)
	{
		// 2gb for now. Could be we could do more on some platforms, but we still will run into
		// problems right now then for example in then color converter (which currently still uses
		// s32 for sizes).
		return (size_t)(s32)(dataSize) == dataSize;
	}

	//! calculate image data size in bytes for selected format, width and height.
	static size_t getDataSizeFromFormat(ECOLOR_FORMAT format, u32 width, u32 height)
	{
		size_t imageSize = 0;

		switch (format)
		{
		case ECF_DXT1:
			imageSize = (size_t)((width + 3) / 4) * ((height + 3) / 4) * 8;
			break;
		case ECF_DXT2:
		case ECF_DXT3:
		case ECF_DXT4:
		case ECF_DXT5:
			imageSize = (size_t)((width + 3) / 4) * ((height + 3) / 4) * 16;
			break;
		case ECF_PVRTC_RGB2:
		case ECF_PVRTC_ARGB2:
			imageSize = ((size_t)core::max_<u32>(width, 16) * core::max_<u32>(height, 8) * 2 + 7) / 8;
			break;
		case ECF_PVRTC_RGB4:
		case ECF_PVRTC_ARGB4:
			imageSize = ((size_t)core::max_<u32>(width, 8) * core::max_<u32>(height, 8) * 4 + 7) / 8;
			break;
		case ECF_PVRTC2_ARGB2:
			imageSize = (size_t)core::ceil32(width / 8.0f) * core::ceil32(height / 4.0f) * 8;
			break;
		case ECF_PVRTC2_ARGB4:
		case ECF_ETC1:
		case ECF_ETC2_RGB:
			imageSize = (size_t)core::ceil32(width / 4.0f) * core::ceil32(height / 4.0f) * 8;
			break;
		case ECF_ETC2_ARGB:
			imageSize = (size_t)core::ceil32(width / 4.0f) * core::ceil32(height / 4.0f) * 16;
			break;
		default: // uncompressed formats
			imageSize = (size_t)getBitsPerPixelFromFormat(format) / 8 * width;
			imageSize *= height;
			break;
		}

		return imageSize;
	}

// Define to check for all compressed image formats cases in a switch
#define IRR_CASE_IIMAGE_COMPRESSED_FORMAT\
	case ECF_DXT1:\
	case ECF_DXT2:\
	case ECF_DXT3:\
	case ECF_DXT4:\
	case ECF_DXT5:\
	case ECF_PVRTC_RGB2:\
	case ECF_PVRTC_ARGB2:\
	case ECF_PVRTC2_ARGB2:\
	case ECF_PVRTC_RGB4:\
	case ECF_PVRTC_ARGB4:\
	case ECF_PVRTC2_ARGB4:\
	case ECF_ETC1:\
	case ECF_ETC2_RGB:\
	case ECF_ETC2_ARGB:

	//! check if this is compressed color format
	static bool isCompressedFormat(const ECOLOR_FORMAT format)
	{
		switch(format)
		{
			IRR_CASE_IIMAGE_COMPRESSED_FORMAT
				return true;
			default:
				return false;
		}
	}

	//! check if the color format is only viable for depth/stencil textures
	static bool isDepthFormat(const ECOLOR_FORMAT format)
	{
		switch(format)
		{
			case ECF_D16:
			case ECF_D32:
			case ECF_D24S8:
				return true;
			default:
				return false;
		}
	}

	//! Check if the color format uses floating point values for pixels
	static bool isFloatingPointFormat(const ECOLOR_FORMAT format)
	{
		if (isCompressedFormat(format))
			return false;

		switch(format)
		{
		case ECF_R16F:
		case ECF_G16R16F:
		case ECF_A16B16G16R16F:
		case ECF_R32F:
		case ECF_G32R32F:
		case ECF_A32B32G32R32F:
			return true;
		default:
			break;
		}
		return false;
	}

#if defined(PATCH_SUPERTUX_8_0_1_with_1_9_0)
	static bool isRenderTargetOnlyFormat(const ECOLOR_FORMAT format)
	{
		switch (format)
		{
		case ECF_A1R5G5B5:
		case ECF_R5G6B5:
		case ECF_R8G8B8:
		case ECF_A8R8G8B8:
			return false;
		default:
			return true;
		}
	}
#endif

protected:
	ECOLOR_FORMAT Format;
	core::dimension2d<u32> Size;

	u8* Data;
	u8* MipMapsData;

	u32 BytesPerPixel;
	u32 Pitch;

	bool DeleteMemory;
	bool DeleteMipMapsMemory;

	core::irrAllocator<u8> Allocator;
#if defined(IRRLICHT_sRGB)
	int Format_sRGB;
#endif
};


} // end namespace video
} // end namespace irr

#endif
