@echo off

echo # Building coldsteel cli
g++ -O2 -static -s -fno-rtti -o _build/coldsteel.exe coldsteel.cpp

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for CMake ...
rem mkdir _CMAKE
mkdir _CMAKE\_IRRLICHT
mkdir _CMAKE\_COLDSTEEL

echo # Use old version on MinGW
set CC=%~dp0mingw_342/bin/mingw32-gcc.exe
set CXX=%~dp0mingw_342/bin/mingw32-g++.exe

echo # Building Irrlicht (Desktop) ...
cd _CMAKE/_IRRLICHT
..\..\mingw_342\bin\cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF ../../lib/irrlicht185
..\..\mingw_342\bin\mingw32-make -j8
cd ../..

echo # Building csrun (Desktop) ...
cd _CMAKE/_COLDSTEEL
..\..\mingw_342\bin\cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_RC_FLAGS="-F pe-i386" ../..
..\..\mingw_342\bin\mingw32-make -j8
move "csrun.exe" "..\..\_build\csrun.exe"
cd ../..

pause
