![icon](logo.png)

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

A 3D game engine for Windows 95, macOS, Linux and the web (using Emscripten) released under the GPL3 license. Based on Irrlicht, Lua and SoLoud.

## Features

- **Lua scripting** — full game logic in Lua with automatic bindings
- **3D rendering** — hardware-accelerated via Irrlicht (OpenGL, Direct3D)
- **Audio** — positional sound and music via SoLoud
- **GUI** — built-in UI controls (buttons, edit boxes, lists, etc.)
- **Sprites & particles** — billboard sprites, terrain, water, lightmaps
- **Builder SDK** — extend the engine with C++ plugins
- **Cross-platform** — Windows, Linux, macOS, **Web** (Emscripten)
- **Haxe bindings** — use ColdSteel from Haxe

## Building

### Windows

```
build.bat
```

Requires MinGW (tdm-gcc), SWIG, CMake, Haxe, Python, Doxygen.
When [Emscripten](https://emscripten.org) is installed the web build runs automatically.

### Linux / macOS

```
./build.sh
```

Requires g++/clang++, SWIG, CMake, Haxe, Python 3, Doxygen.
When [Emscripten](https://emscripten.org) is installed the web build runs automatically.

To build only the web target (used by CI):

```
./build.sh --emscripten
```

Build outputs go to `_build/`.

## Creating a module

ColdSteel can be extended with C/C++ shared libraries (`.dll`/`.dylib`/`.so`) that are loaded at runtime.

### Module structure

A module is a shared library that exports one entry point: `<name>_load`. ColdSteel calls it when Lua runs `load("<name>")`.

```c
// mymodule.c
#include "sdk.h"

static ColdSteelSDK *g_sdk = NULL;

static int MyFunc(void *context)
{
    int a = g_sdk->GetIntArg(context, 1);
    int b = g_sdk->GetIntArg(context, 2);
    g_sdk->PushInt(context, a + b);
    return 1;
}

extern "C" int mymodule_load(ColdSteelSDK *sdk)
{
    g_sdk = sdk;
    g_sdk->RegisterFunction("MyFunc", MyFunc);
    return COLDSTEEL_SDK_VERSION;
}
```

### Compiling

Point your compiler at the ColdSteel SDK headers and link nothing else — the SDK struct provides all engine access.

```bash
g++ -shared -o mymodule.so mymodule.c -I path/to/coldsteel/_build/sdk
```

(Replace `.so` with `.dll` on Windows or `.dylib` on macOS.)

### Emscripten (Web)

Modules work on the web target too. Compile with the same Emscripten SDK used to build ColdSteel:

```bash
emcc -shared -o mymodule.wasm mymodule.c -I path/to/coldsteel/_build/sdk
```

Deploy `mymodule.wasm` as a static asset alongside the game output (`.js`, `.wasm`, `.html`, `.data`). ColdSteel fetches it at runtime when `load()` is called.

The Lua usage is identical to the desktop version.

### Using from Lua

```lua
load("mymodule")
print(MyFunc(3, 4))   --> 7
```

If the module was compiled against a different `COLDSTEEL_SDK_VERSION` than the running ColdSteel, `load` will fail with a clear error message.

### Notes

- The `_load` function **must** return `COLDSTEEL_SDK_VERSION` — otherwise loading fails.
- The `context` pointer passed to your registered function is the Lua state; use the SDK's `Get*Arg` / `Push*` helpers to marshal arguments and return values.
- The return value of your registered function is the number of values you pushed onto the Lua stack.

## TODO

* Built in editor.
* Add Pelican gh page website.
* Add itch.io page with precompiled binary for Win95 (7z self-extract archive?).
