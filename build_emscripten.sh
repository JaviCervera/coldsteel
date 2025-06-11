#!/bin/sh
cd `dirname $0`

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo "# Creating folders for CMake ..."
mkdir _CMAKE
mkdir _CMAKE/_COLDSTEEL_EMSCRIPTEN
mkdir _CMAKE/_IRRLICHT_EMSCRIPTEN

echo "# Building Irrlicht (Emscripten) ..."
cd lib/irrlicht190_ogles
emcmake cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
cd ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
emmake make NDEBUG=1
cd ../..

echo "# Building ColdSteel (Emscripten) ..."
emcmake cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SVN=ON -B _CMAKE/_COLDSTEEL_EMSCRIPTEN
cd _CMAKE/_COLDSTEEL_EMSCRIPTEN
emmake make NDEBUG=1
mv coldsteel.html ../../_build/coldsteel.html
mv coldsteel.js ../../_build/coldsteel.js
mv coldsteel.data ../../_build/coldsteel.data
mv coldsteel.wasm ../../_build/coldsteel.wasm
cd ../..
