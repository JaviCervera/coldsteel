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
mkdir _SDL2
cd ..

echo # Building SDL2 (Desktop) ...
cd lib/sdl2
cmake -G "MinGW Makefiles" -DCMAKE_C_FLAGS=-m32 -DCMAKE_BUILD_TYPE=MinSizeRel -DSDL_SHARED=OFF -DSDL_ATOMIC=OFF -DSDL_CPUINFO=OFF -DSDL_DLOPEN=OFF -DSDL_FILE=OFF -DSDL_FILESYSTEM=OFF -DSDL_RENDER=OFF -DSDL_THREADS=OFF -DVIDEO_VULKAN=OFF -DVIDEO_OPENGLES=OFF -DDIRECTX=OFF -DRENDER_D3D=OFF ../../lib/sdl2 -B ../../_CMAKE/_SDL2
cd ../../_CMAKE/_SDL2
mingw32-make
cd ../..
xcopy "lib\sdl2\include" "_CMAKE\_SDL2\include\SDL2\" /y
xcopy "_CMAKE\_SDL2\include\*.h" "_CMAKE\_SDL2\include\SDL2\" /y
xcopy "_CMAKE\_SDL2\*.a" "_CMAKE\_SDL2\lib\" /y

echo # Building Irrlicht (Desktop) ...
cd lib/irrlicht185
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -DSDL2_ENABLED=ON -DSDL2_PATH=../../_CMAKE/_SDL2 -B ../../_CMAKE/_IRRLICHT
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
