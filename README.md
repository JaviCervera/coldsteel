# ![icon](logo.png) ColdSteel

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

## Generating documentation

```shell
doxygen
python gen_doc.py
```

## TODO

* Remove XML module.
* Revert surface indices to zero-based (or we'll have one less max vertices per surface!)
* Add Doxygen documentation (maybe use Sphinx + Breathe?).
* Add Pelican gh page website.
* Add itch.io page with precompiled binary for Win95 (7z self-extract archive?).
