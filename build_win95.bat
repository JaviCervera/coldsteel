@echo off

set PATH=%~dp0TDM-GCC-32\bin;%PATH%

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo # Generating SDK header ...
swig.exe -xml -xmllite -c++ -o coldsteel.xml coldsteel.i
haxe -m SdkBuilder --interp
del coldsteel.xml

echo # Creating folders for CMake ...
mkdir _CMAKE\_COLDSTEEL

echo # Building coldsteel (Desktop) ...
cd _CMAKE/_COLDSTEEL
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DLEGACY_IRRLICHT=ON -DCMAKE_RC_FLAGS="-O coff" ../..
mingw32-make -j8
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
move "libbuilder.dll" "..\..\_build\builder.dll"
cd ../..

pause
