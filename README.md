# ![icon](logo.png) ColdSteel

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

## Generating documentation

```shell
doxygen
python gen_doc.py
```

## TODO

* Documentation index.html should contain all modules. Each module should list all functions, each function should have its own html file.
* ColdSteelSDK should export all engine functions to the dlls.
* Use custom Lua bindings instead of SWIG based ones (reduce file size).
* Build with VC6 and Irrlicht 1.3.1 on a separate branch.
* Add Doxygen documentation (maybe use Sphinx + Breathe?).
* Add Pelican gh page website.
* Add itch.io page with precompiled binary for Win95 (7z self-extract archive?).
