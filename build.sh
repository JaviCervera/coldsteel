#!/bin/sh
cd `dirname $0`

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo "# Creating folders for Cmake ..."
mkdir _CMAKE
mkdir _CMAKE/_COLDSTEEL
mkdir _CMAKE/_COLDSTEEL_EMSCRIPTEN
mkdir _CMAKE/_IRRLICHT
mkdir _CMAKE/_IRRLICHT_EMSCRIPTEN

echo "# Configuring Irrlicht (Desktop) ..."
cd lib/irrlicht184
cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
make
cd ../..

echo "# Configuring ColdSteel (Desktop) ..."
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
make
mv coldsteel ../../_build/coldsteel
cd ../..

echo "# Configuring Irrlicht (Emscripten) ..."
cd lib/irrlicht190_ogles
emcmake cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
cd ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
emmake make NDEBUG=1
cd ../..

echo "# Configuring ColdSteel (Emscripten) ..."
emcmake cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -B _CMAKE/_COLDSTEEL_EMSCRIPTEN
cd _CMAKE/_COLDSTEEL_EMSCRIPTEN
emmake make NDEBUG=1
mv coldsteel.js ../../_build/coldsteel.js
mv coldsteel.data ../../_build/coldsteel.data
mv coldsteel.wasm ../../_build/coldsteel.wasm
cd ../..
