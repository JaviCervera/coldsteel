# ![icon](logo.png) ColdSteel

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

## Generating documentation

```shell
doxygen
sphinx-build -b html -Dbreathe_projects.my_project=doc_out/xml . doc_out/sphinx
```

## TODO

* GUI controls expect an IGUIFont*, so instances of Font* cannot be passed.
* Fix audio positioning.
* Fix manual mesh creation crashing on Irrlicht 1.9.
* Add Doxygen documentation (maybe use Sphinx + Breathe?).
* Add Pelican gh page website.
