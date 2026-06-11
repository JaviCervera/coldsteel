#!/bin/sh
cd `dirname $0`

# Detect OS and set C++ compiler
if [ "$(uname)" = "Darwin" ]; then
    CXX="clang++"
    FONTTOOL_LIBS="-lIrrlicht -framework Cocoa -framework IOKit -framework OpenGL -lobjc -lpthread -s"
else
    CXX="g++"
    FONTTOOL_LIBS="-lIrrlicht -lGL -lX11 -lXxf86vm -lpthread -s -static-libgcc -static-libstdc++"
fi

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo "# Generating SDK header ..."
swig -xml -xmllite -c++ -o coldsteel.xml coldsteel.i
haxe -m SdkBuilder --interp
rm coldsteel.xml

echo "# Creating folders for CMake ..."
mkdir _CMAKE
mkdir _CMAKE/_COLDSTEEL
mkdir _CMAKE/_IRRLICHT

echo "# Building Irrlicht (Desktop) ..."
cd lib/irrlicht190_ogles
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -DIRRLICHT_SVN=ON -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
make -j8
cd ../..

echo "# Building coldsteel (Desktop) ..."
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SVN=ON -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
make -j8
mv coldsteel ../../_build/coldsteel
cd ../..

echo "# Building fonttool ..."
$CXX -std=c++98 -Os -D_IRR_STATIC_LIB_ -I lib/irrlicht190_ogles/include -L _CMAKE/_IRRLICHT -o _build/fonttool fonttool/fonttool.cpp $FONTTOOL_LIBS
