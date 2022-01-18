@echo off

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for CMake ...
mkdir _CMAKE
cd _CMAKE
mkdir _COLDSTEEL
mkdir _COLDSTEEL_EMSCRIPTEN
mkdir _IRRLICHT
mkdir _IRRLICHT_EMSCRIPTEN
cd ..

echo # Building Irrlicht (Desktop) ...
cd lib/irrlicht185
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
mingw32-make
cd ../..

echo # Building ColdSteel (Desktop) ...
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_BUILD_TYPE=MinSizeRel -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
mingw32-make
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
cd ../..

echo # Building Irrlicht (Emscripten) ...
call prepare_irrlicht_emscripten.bat
cd ../..
call build_irrlicht_emscripten.bat
cd ../..

echo # Building ColdSteel (Emscripten) ...
call prepare_coldsteel_emscripten.bat
call build_coldsteel_emscripten.bat
move "coldsteel.js" "..\..\_build\coldsteel.js"
move "coldsteel.data" "..\..\_build\coldsteel.data"
move "coldsteel.wasm" "..\..\_build\coldsteel.wasm"
cd ../..

pause
