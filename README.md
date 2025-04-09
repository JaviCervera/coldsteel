# ![icon](logo.png) ColdSteel

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

Check out [this readme](_build/README.md).

NOTE: When the instructions given in the file above refer to the root folder, it means `_build`.

## TODO

* Rename `MeshEntity` to `Model`. Add `LoadModel` directly (no need for `LoadMesh` -> `CreateModel` -> `FreeMesh`).
* DrawWorld should render all cameras, and these should have clear mode, render target etc.
* Add font editor.
* Try to use Asyncify on Emscripten, remove Loop entrypoint from Lua, and add RefreshScreen.
* Do not handle screen setup differently on Emscripten.
* Embed Lua into the CLI tool, so scripts are compiled when packaged.
* Use Irrlicht's embedded zlib to create the assets pkg, and merge `coldsteel` and `csrun`.
* Add ButtonHit, KeyHit, JoystickButtonHit.
* Add particles support & editor.
* Fix audio positioning.
* Add more samples and demos.
* Add SciTE based Lua editor.
* Add mesh viewer with material support and LoadMeshEx which provides a materials file as well.
* GUI styles.
* Add shader support.
* Add documentation (with Sphinx? - also check if SWIG parses Doxygen comments).
* Update Haxe support with `Entity` instead of `ISceneNode`, etc, and add instructions.
* ODE physics?
