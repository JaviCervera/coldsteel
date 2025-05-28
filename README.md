# ![icon](logo.png) ColdSteel

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

## Generating documentation

```shell
doxygen
sphinx-build -b html -Dbreathe_projects.my_project=doc_out/xml . doc_out/sphinx
```

## TODO

* Fix audio positioning.
* Add Doxygen documentation (maybe use Sphinx + Breathe?).
* Add Pelican gh page website.
* Add itch.io page with precompiled binary for Win95 (7z self-extract archive?).
