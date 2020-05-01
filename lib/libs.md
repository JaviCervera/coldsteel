# Libraries used

## Irrlicht 1.8.4

Preprocessor definitions (put them in `include/IrrCompileConfig.h`):

```c++
#define NO_IRR_COMPILE_WITH_SDL_DEVICE_
#define NO_IRR_COMPILE_WITH_DIRECT3D_8_
#define NO_IRR_COMPILE_WITH_DIRECT3D_9_
#define NO_IRR_COMPILE_WITH_BURNINGSVIDEO_
#define NO_IRR_LINUX_XCURSOR_
#define NO_IRR_WCHAR_FILESYSTEM
#define NO_IRR_COMPILE_WITH_CG_
#define NO_IRR_COMPILE_WITH_IRR_SCENE_LOADER_
#define NO_IRR_COMPILE_WITH_IRR_MESH_LOADER_
#define NO_IRR_COMPILE_WITH_OGRE_LOADER_
#define NO_IRR_COMPILE_WITH_HALFLIFE_LOADER_
#define NO_IRR_COMPILE_WITH_MD2_LOADER_
#define NO_IRR_COMPILE_WITH_MD3_LOADER_
#define NO_IRR_COMPILE_WITH_CSM_LOADER_
#define NO_IRR_COMPILE_WITH_BSP_LOADER_
#define NO_IRR_COMPILE_WITH_DMF_LOADER_
#define _IRR_COMPILE_WITH_LMTS_LOADER_
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

### Windows

It has been compiled with MinGW-W64-builds-4.3.5, which was configured with the following flags:

```bash
--mode=gcc-8.1.0
--buildroot=/c/mingw810
--jobs=2
--rev=0
--rt-version=trunk
--bootstrap
--threads=win32
--exceptions=sjlj
--arch=i686
--bin-compress
```

Irrlicht's Code::Blocks project was used to build it. The *Win32 - Release - fast math - static* target has been used.
In *Project / Build Options...*, the following options were selected:

* Static linking [-static]
* Target x86 (32bit) [-m32]
* Optimize generated code (for size) [-Os]
* Strip all symbols from binary (minimizes size) [-s]


### macOS

Architecture has been changed to *Universal (32/64-bit Intel)*. On *macOS Deployment Target*, it has been set to *10.6*.
On the top of the file `CIrrDeviceMacOSX.mm`, the following needed to be added:

```objective-c
#include <Carbon/Carbon.h>
```

On that same file, the line 499

```objective-c
[NSApp setDelegate:(id<NSFileManagerDelegate>)[[[AppDelegate alloc] initWithDevice:this] autorelease]];
```

was changed to

```objective-c
[NSApp setDelegate:(id<NSApplicationDelegate>)[[[AppDelegate alloc] initWithDevice:this] autorelease]];
```

and the line 500

```objective-c
[NSBundle loadNibNamed:@"MainMenu" owner:[NSApp delegate]];
```

was commented.
