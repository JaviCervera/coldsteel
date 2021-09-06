#!/bin/sh
cd `dirname $0`

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo "# Creating folders for Cmake ..."
mkdir _CMAKE
mkdir _CMAKE/_COLDSTEEL
mkdir _CMAKE/_IRRLICHT

echo "# Configuring Irrlicht ..."
cd _CMAKE/_IRRLICHT
cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF ../../lib/irrlicht184
cd ../..

echo "# Configuring ColdSteel ..."
cd _CMAKE/_COLDSTEEL
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel ../..
cd ../..

echo "# Compiling Irrlicht ..."
cd _CMAKE/_IRRLICHT
make
cd ../..

echo "# Compiling ColdSteel ..."
cd _CMAKE/_COLDSTEEL
make
cd ../..
mv _CMAKE/_COLDSTEEL/coldsteel _build/coldsteel
