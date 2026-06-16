![icon](logo.png)

[![build_emscripten](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml/badge.svg)](https://github.com/JaviCervera/coldsteel/actions/workflows/build_emscripten.yml)

## Generating documentation

```shell
doxygen
python gen_doc.py
```

## TODO

* Remove all `std` usage, rely only on `irr::core` to reduce disk usage.
* Restore GUI functions and samples.
* Use `Entity`, `Camera`, etc on Haxe wrapper instead of `ISceneNode`, `ICameraSceneNode`, etc.
* Unify build scripts (no separate desktop, web, haxe wrapper, doc).
* Fix lighting on Emscripten.
* Add Pelican gh page website.
* Add itch.io page with precompiled binary for Win95 (7z self-extract archive?).
