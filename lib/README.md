# Libraries used

## Irrlicht

Win32 version uses Irrlicht 1.8.5.
Other desktop versions use Irrlicht 1.9.0 commit **r6297**.
Emscripten version uses the ogles branch of [Irrlicht SVN](https://sourceforge.net/p/irrlicht/code/HEAD/tree/branches/ogl-es/), commit **r6258**.

The modifications made to the engine are described here:

### Common

Preprocessor definitions (put them in `include/IrrCompileConfig.h`):

```c++
#define NO_IRR_COMPILE_WITH_WINDOWS_CE_DEVICE_
//#define NO_IRR_COMPILE_WITH_CONSOLE_DEVICE
#define NO_IRR_COMPILE_WITH_FB_DEVICE_
#define NO_IRR_COMPILE_WITH_DIRECT3D_8_
#define NO_IRR_COMPILE_WITH_DIRECT3D_9_
//#define NO_IRR_COMPILE_WITH_OPENGL_
#define NO_IRR_COMPILE_WITH_SOFTWARE_
#define NO_IRR_COMPILE_WITH_BURNINGSVIDEO_
#define NO_IRR_LINUX_XCURSOR_
#define NO_IRR_WCHAR_FILESYSTEM
#define NO_IRR_COMPILE_WITH_CG_
#define NO_IRR_COMPILE_WITH_IRR_SCENE_LOADER_
#define NO_IRR_COMPILE_WITH_IRR_MESH_LOADER_
#define NO_IRR_COMPILE_WITH_OGRE_LOADER_
#define NO_IRR_COMPILE_WITH_HALFLIFE_LOADER_
//#define NO_IRR_COMPILE_WITH_MD2_LOADER_
#define NO_IRR_COMPILE_WITH_MD3_LOADER_
#define NO_IRR_COMPILE_WITH_CSM_LOADER_
#define NO_IRR_COMPILE_WITH_BSP_LOADER_
#define NO_IRR_COMPILE_WITH_DMF_LOADER_
#define NO_IRR_COMPILE_WITH_LMTS_LOADER_
#define NO_IRR_COMPILE_WITH_MY3D_LOADER_
#define NO_IRR_COMPILE_WITH_OCT_LOADER_
#define NO_IRR_COMPILE_WITH_LWO_LOADER_
#define NO_IRR_COMPILE_WITH_STL_LOADER_
#define NO_IRR_COMPILE_WITH_PLY_LOADER_
#define NO_IRR_COMPILE_WITH_SMF_LOADER_
#define NO_IRR_COMPILE_WITH_IRR_WRITER_
#define NO_IRR_COMPILE_WITH_COLLADA_WRITER_
#define NO_IRR_COMPILE_WITH_STL_WRITER_
#define NO_IRR_COMPILE_WITH_OBJ_WRITER_
#define NO_IRR_COMPILE_WITH_PLY_WRITER_
#define NO_IRR_COMPILE_WITH_PCX_LOADER_
#define NO_IRR_COMPILE_WITH_PPM_LOADER_
#define NO_IRR_COMPILE_WITH_PSD_LOADER_
#define NO_IRR_COMPILE_WITH_DDS_LOADER_
#define NO_IRR_COMPILE_WITH_WAL_LOADER_
#define NO_IRR_COMPILE_WITH_LMP_LOADER_
#define NO_IRR_COMPILE_WITH_RGB_LOADER_
#define NO_IRR_COMPILE_WITH_BMP_WRITER_
#define NO_IRR_COMPILE_WITH_JPG_WRITER_
#define NO_IRR_COMPILE_WITH_PCX_WRITER_
#define NO_IRR_COMPILE_WITH_PNG_WRITER_
#define NO_IRR_COMPILE_WITH_PPM_WRITER_
#define NO_IRR_COMPILE_WITH_PSD_WRITER_
#define NO_IRR_COMPILE_WITH_TGA_WRITER_
#define NO__IRR_COMPILE_WITH_PAK_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_NPK_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_TAR_ARCHIVE_LOADER_
#define NO__IRR_COMPILE_WITH_WAD_ARCHIVE_LOADER_
```

Also, a couple of changes have been made in `include/SMaterial.h` to change the default use of vertex colors in lighting.

Search for `ColorMaterial(ECM_DIFFUSE), BlendOperation(EBO_NONE),` and replace with:

```c++
ColorMaterial(ECM_DIFFUSE_AND_AMBIENT), BlendOperation(EBO_NONE),
```

Search for `ColorMaterial = value?ECM_DIFFUSE:ECM_NONE; break;` and replace with:

```c++
ColorMaterial = value?ECM_DIFFUSE_AND_AMBIENT:ECM_NONE; break;
```

### macOS (irrlicht190 only)

On file `CIrrDeviceMacOSX.mm`, search for `NSMenu* mainMenu = [[[NSMenu alloc] initWithTitle:@"MainMenu"] autorelease];` and add the following line right before it:

```c++
if (bundleName) {
```

Belowthat, after the line that contains `[NSApp setMainMenu:mainMenu];`, close the block with a new line containing `}`.

Also, search for the line `Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, CreationParams.WindowSize.Width,CreationParams.WindowSize.Height) styleMask:NSTitledWindowMask+NSClosableWindowMask+NSResizableWindowMask backing:type defer:FALSE];` and replace with:

```c++
Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, CreationParams.WindowSize.Width,CreationParams.WindowSize.Height) styleMask:NSTitledWindowMask+NSClosableWindowMask+NSMiniaturizableWindowMask backing:type defer:FALSE];
```

### Emscripten (irrlicht190_ogles only)

To have a compressed data package added by default to the filesystem, add the following as line 11
of file `source/Irrlicht/CIrrDeviceSDL.cpp`:

```c++
#include "IFileSystem.h"
```

And the following right before line 278 (before the call to `createDriver()`):

```c++
#ifdef EMSCRIPTEN
FileSystem->addFileArchive("data.bin", true, false, io::EFAT_ZIP);
#endif
```
