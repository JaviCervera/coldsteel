@echo off

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
..\..\mingw_342\bin\cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF ../../lib/irrlicht185
..\..\mingw_342\bin\mingw32-make
cd ../..

echo # Building ColdSteel (Desktop) ...
cd _CMAKE/_COLDSTEEL
..\..\mingw_342\bin\cmake -G "MinGW Makefiles" -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_RC_FLAGS="-F pe-i386" -DCMAKE_BUILD_TYPE=Release ../..
..\..\mingw_342\bin\mingw32-make
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
cd ../..

pause
