@echo off

set PATH=%~dp0TDM-GCC-32\bin;%PATH%

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo # Creating folders for CMake ...
mkdir _CMAKE\_IRRLICHT
mkdir _CMAKE\_COLDSTEEL

echo # Building Irrlicht (Desktop) ...
cd _CMAKE/_IRRLICHT
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF ../../lib/irrlicht185
mingw32-make -j8
cd ../..

echo # Building coldsteel (Desktop) ...
cd _CMAKE/_COLDSTEEL
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DLEGACY_IRRLICHT=ON -DCMAKE_RC_FLAGS="-O coff" ../..
mingw32-make -j8
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
cd ../..

pause
