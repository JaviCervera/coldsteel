@echo off

echo # Building coldsteel cli
g++ -O2 -static -s -fno-rtti -o _build/coldsteel.exe coldsteel.cpp lib/lua/*.c

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for CMake ...
mkdir _CMAKE\_IRRLICHT
mkdir _CMAKE\_COLDSTEEL

echo # Building Irrlicht (Desktop) ...
cd lib/irrlicht190_ogles
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
mingw32-make -j8
cd ../..

echo # Building csrun (Desktop) ...
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_RC_FLAGS="-F pe-i386" -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
mingw32-make -j8
move "csrun.exe" "..\..\_build\csrun.exe"
cd ../..

pause
