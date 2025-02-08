@echo off

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for CMake ...
mkdir _CMAKE
cd _CMAKE
mkdir _COLDSTEEL
mkdir _IRRLICHT
cd ..

echo # Building Irrlicht (Desktop) ...
cd lib/irrlicht190
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
mingw32-make
cd ../..

echo # Building ColdSteel (Desktop) ...
cmake -G "MinGW Makefiles" -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_RC_FLAGS="-F pe-i386" -DCMAKE_BUILD_TYPE=Release -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
mingw32-make
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
cd ../..

pause
