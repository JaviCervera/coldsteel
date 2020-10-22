@echo off

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for Cmake ...
mkdir _CMAKE
cd _CMAKE
mkdir _COLDSTEEL
mkdir _IRRLICHT
cd ..

echo # Configuring Irrlicht ...
cd _CMAKE/_IRRLICHT
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF -DSDL_ENABLED=ON -DSDL_PATH=../../lib/sdl ../../lib/irrlicht
cd ..\..

echo # Configuring ColdSteel ...
cd _CMAKE/_COLDSTEEL
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_BUILD_TYPE=MinSizeRel ../..
cd ..\..
