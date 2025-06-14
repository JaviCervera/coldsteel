@echo off

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo # Creating folders for CMake ...
mkdir _CMAKE\_COLDSTEEL_EMSCRIPTEN
mkdir _CMAKE\_IRRLICHT_EMSCRIPTEN

echo # Building Irrlicht (Emscripten) ...
cd lib/irrlicht190_ogles
call emcmake cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
cd ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
call emmake make NDEBUG=1 -j8
cd ../..

echo # Building ColdSteel (Emscripten) ...
call emcmake cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SVN=ON -B _CMAKE/_COLDSTEEL_EMSCRIPTEN
cd _CMAKE/_COLDSTEEL_EMSCRIPTEN
call emmake make NDEBUG=1 -j8
move "coldsteel.html" "..\..\_build\coldsteel.html"
move "coldsteel.js" "..\..\_build\coldsteel.js"
move "coldsteel.data" "..\..\_build\coldsteel.data"
move "coldsteel.wasm" "..\..\_build\coldsteel.wasm"
cd ../..

pause
