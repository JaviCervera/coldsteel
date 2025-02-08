@echo off

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for CMake ...
rem mkdir _CMAKE
mkdir _CMAKE\_COLDSTEEL

echo # Building ColdSteel (Desktop) ...
cmake -G "MinGW Makefiles" -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_RC_FLAGS="-F pe-i386" -DCMAKE_BUILD_TYPE=Release -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
mingw32-make
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
cd ../..

pause
