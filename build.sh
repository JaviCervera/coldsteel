#!/bin/sh
cd `dirname $0`

echo "# Building coldsteel cli"
g++ -O2 -s -fno-rtti -o _build/coldsteel coldsteel.cpp

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo "# Creating folders for CMake ..."
mkdir _CMAKE
mkdir _CMAKE/_COLDSTEEL
mkdir _CMAKE/_IRRLICHT

echo "# Building Irrlicht (Desktop) ..."
cd lib/irrlicht190_ogles
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
make
cd ../..

echo "# Building csrun (Desktop) ..."
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
make
mv csrun ../../_build/csrun
cd ../..
