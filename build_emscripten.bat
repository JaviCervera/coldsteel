@echo off

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cpp coldsteel.i

echo # Creating folders for CMake ...
mkdir _CMAKE
cd _CMAKE
mkdir _COLDSTEEL_EMSCRIPTEN
mkdir _IRRLICHT_EMSCRIPTEN
cd ..

echo # Building Irrlicht (Emscripten) ...
emcmake cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF -B _CMAKE/_IRRLICHT_EMSCRIPTEN
cd _CMAKE/_IRRLICHT_EMSCRIPTEN
emmake make NDEBUG=1
cd ../..

echo # Building ColdSteel (Emscripten) ...
emcmake cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -B _CMAKE/_COLDSTEEL_EMSCRIPTEN
cd _CMAKE/_COLDSTEEL_EMSCRIPTEN
emmake make NDEBUG=1
move "csrun.js" "..\..\_build\coldsteel.js"
move "csrun.data" "..\..\_build\coldsteel.data"
move "csrun.wasm" "..\..\_build\coldsteel.wasm"
cd ../..

pause
