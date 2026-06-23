@echo off

rem set PATH=%~dp0TDM-GCC-32\bin;%PATH%

rem ---- Desktop build ----

echo # Generating Lua wrapper ...
swig.exe -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo # Generating SDK header ...
swig.exe -xml -xmllite -c++ -o coldsteel.xml coldsteel.i
haxe -m SdkBuilder --interp
del coldsteel.xml

echo # Creating folders for CMake ...
if not exist _CMAKE\_IRRLICHT mkdir _CMAKE\_IRRLICHT
if not exist _CMAKE\_COLDSTEEL mkdir _CMAKE\_COLDSTEEL

echo # Building Irrlicht (Desktop) ...
cd lib/irrlicht190_ogles
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
mingw32-make -j8
cd ../..

echo # Building coldsteel (Desktop) ...
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SVN=ON -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_RC_FLAGS="-F pe-i386" -B _CMAKE/_COLDSTEEL
cd _CMAKE/_COLDSTEEL
mingw32-make -j8
move "coldsteel.exe" "..\..\_build\coldsteel.exe"
move "libbuilder.dll" "..\..\_build\builder.dll"
cd ../..

echo # Building fonttool ...
g++ -m32 -std=c++98 -Os -D_IRR_STATIC_LIB_ -I lib/irrlicht190_ogles/include -L _CMAKE/_IRRLICHT -o _build/fonttool.exe fonttool/fonttool.cpp -lIrrlicht -lopengl32 -lwinmm -lgdi32 -mwindows -s -static-libgcc -static-libstdc++

rem ---- Web (Emscripten) build ----

where emcmake >nul 2>nul
if %errorlevel% equ 0 (
  if not exist _CMAKE\_IRRLICHT_EMSCRIPTEN mkdir _CMAKE\_IRRLICHT_EMSCRIPTEN
  if not exist _CMAKE\_COLDSTEEL_EMSCRIPTEN mkdir _CMAKE\_COLDSTEEL_EMSCRIPTEN

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
) else (
  echo # Emscripten SDK not found, skipping web build ...
)

rem ---- Haxe wrappers ----

echo # Generating Haxe wrappers ...
if not exist _build\haxe mkdir _build\haxe
swig.exe -xml -xmllite -c++ -o _build/haxe/coldsteel.xml coldsteel.i
haxe --run HaxeWrapperBuilder
del _build\haxe\coldsteel.xml

rem ---- Documentation ----

echo # Generating documentation ...
doxygen
python gen_doc.py

pause
