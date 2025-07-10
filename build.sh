#!/bin/sh
cd `dirname $0`

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo # Generating SDK header ...
swig -xml -xmllite -c++ -o coldsteel.xml coldsteel.i
haxe -m SdkBuilder --interp
rm coldsteel.xml

echo "# Creating folders for CMake ..."
mkdir _CMAKE
mkdir _CMAKE/_COLDSTEEL
mkdir _CMAKE/_IRRLICHT

echo "# Building Irrlicht (Desktop) ..."
cd lib/irrlicht190_ogles
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
make -j8
cd ../..

echo "# Building coldsteel (Desktop) ..."
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
make -j8
mv coldsteel ../../_build/coldsteel
cd ../..
