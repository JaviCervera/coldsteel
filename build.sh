#!/bin/sh
cd `dirname $0`

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo "# Creating folders for CMake ..."
mkdir _CMAKE
mkdir _CMAKE/_COLDSTEEL
mkdir _CMAKE/_IRRLICHT
mkdir _CMAKE/_SDL2

echo "# Building SDL2 (Desktop) ..."
cd lib/sdl2
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DVIDEO_VULKAN=OFF -DVIDEO_OPENGLES=OFF -B ../../_CMAKE/_SDL2
cd ../../_CMAKE/_SDL2
make
cd ../..
mkdir _CMAKE/_SDL2/include/SDL2
cp lib/sdl2/include/* _CMAKE/_SDL2/include/SDL2
cp _CMAKE/_SDL2/include/SDL_config.h _CMAKE/_SDL2/include/SDL2
cp _CMAKE/_SDL2/include/SDL_revision.h _CMAKE/_SDL2/include/SDL2
mkdir _CMAKE/_SDL2/lib
cp _CMAKE/_SDL2/libSDL2.a _CMAKE/_SDL2/lib
cp _CMAKE/_SDL2/libSDL2main.a _CMAKE/_SDL2/lib

echo "# Building Irrlicht (Desktop) ..."
cd lib/irrlicht185
cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF -DSDL2_ENABLED=ON -DSDL2_PATH=../../_CMAKE/_SDL2 -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
make
cd ../..

echo "# Building ColdSteel (Desktop) ..."
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
make
mv coldsteel ../../_build/coldsteel
cd ../..
