![icon](logo.png)

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

ColdSteel is a multi-platform 3D game engine released under the GPL3 license, based on Lua, Irrlicht and SoLoud.

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

## TODO

* Particle functions.
* Top aligned text in EditBox.
* Built in editor.
* Expose shader API.
* Add Pelican gh page website.
* Add itch.io page with precompiled binary for Win95 (7z self-extract archive?).
