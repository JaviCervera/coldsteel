@echo off

rem set PATH=%~dp0TDM-GCC-32\bin;%PATH%

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo # Generating SDK header ...
swig.exe -xml -xmllite -c++ -o coldsteel.xml coldsteel.i
haxe -m SdkBuilder --interp
del coldsteel.xml

echo # Creating folders for CMake ...
mkdir _CMAKE\_IRRLICHT
mkdir _CMAKE\_COLDSTEEL

echo # Building Irrlicht (Desktop) ...
cd lib/irrlicht185
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
mingw32-make -j8
cd ../..

echo # Building coldsteel (Desktop) ...
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_RC_FLAGS="-F pe-i386" -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
mingw32-make -j8
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
move "libbuilder.dll" "..\..\_build\builder.dll"
cd ../..

pause
