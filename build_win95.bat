@echo off

echo # Building coldsteel cli
g++ -O2 -static -s -fno-rtti -o _build/coldsteel.exe coldsteel.cpp lib/lua/*.c

set PATH=%~dp0TDM-GCC-32\bin;%PATH%

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for CMake ...
mkdir _CMAKE\_IRRLICHT
mkdir _CMAKE\_COLDSTEEL

echo # Building Irrlicht (Desktop) ...
cd _CMAKE/_IRRLICHT
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF ../../lib/irrlicht185
mingw32-make -j8
cd ../..

echo # Building csrun (Desktop) ...
cd _CMAKE/_COLDSTEEL
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DLEGACY_IRRLICHT=ON -DCMAKE_RC_FLAGS="-O coff" ../..
mingw32-make -j8
move "csrun.exe" "..\..\_build\csrun.exe"
cd ../..

pause
