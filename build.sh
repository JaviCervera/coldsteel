#!/bin/sh
cd `dirname $0`

# Detect OS and set C++ compiler
if [ "$(uname)" = "Darwin" ]; then
    CXX="clang++"
    FONTTOOL_LIBS="-lIrrlicht -framework Cocoa -framework IOKit -framework OpenGL -lobjc -lpthread -s"
else
    CXX="g++"
    FONTTOOL_LIBS="-lIrrlicht -lGL -lX11 -lXxf86vm -lEGL -lpthread -s -static-libgcc -static-libstdc++"
fi

emscripten_build() {
  mkdir -p _CMAKE/_IRRLICHT_EMSCRIPTEN
  mkdir -p _CMAKE/_COLDSTEEL_EMSCRIPTEN

  echo "# Building Irrlicht (Emscripten) ..."
  cd lib/irrlicht190_ogles
  emcmake cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SHARED=OFF -DCMAKE_CXX_FLAGS="-sUSE_SDL=1" -B ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
  cd ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
  emmake make NDEBUG=1 CPATH=""
  cd ../..

  echo "# Building ColdSteel (Emscripten) ..."
  emcmake cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DIRRLICHT_SVN=ON -B _CMAKE/_COLDSTEEL_EMSCRIPTEN
  cd _CMAKE/_COLDSTEEL_EMSCRIPTEN
  emmake make NDEBUG=1
  mv coldsteel.html ../../_build/coldsteel.html
  mv coldsteel.js ../../_build/coldsteel.js
  mv coldsteel.data ../../_build/coldsteel.data
  mv coldsteel.wasm ../../_build/coldsteel.wasm
  cd ../..
}

LUAJIT=0
case "${1:-}" in
  --luajit)
    LUAJIT=1
    ;;
  --emscripten|--web)
    echo "# Generating Lua wrapper ..."
    swig -lua -c++ -o src/lua_wrapper.cc coldsteel.i
    if ! command -v emcmake >/dev/null 2>&1; then
      echo "Emscripten SDK not found!" >&2
      exit 1
    fi
    emscripten_build
    exit 0
    ;;
esac

# ---- Desktop build ----

echo "# Generating Lua wrapper ..."
swig -lua -c++ -o src/lua_wrapper.cc coldsteel.i

echo "# Generating SDK header ..."
swig -xml -xmllite -c++ -o coldsteel.xml coldsteel.i
haxe -m SdkBuilder --interp
rm coldsteel.xml

echo "# Creating folders for CMake ..."
mkdir -p _CMAKE/_COLDSTEEL
mkdir -p _CMAKE/_IRRLICHT

echo "# Building Irrlicht (Desktop) ..."
cd lib/irrlicht190_ogles
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT
cd ../../_CMAKE/_IRRLICHT
make -j8
cd ../..

if [ "$LUAJIT" = "1" ]; then
  echo "# Building LuaJIT ..."
  mkdir -p _CMAKE/_LUAJIT
  if [ "$(uname)" = "Darwin" ]; then
    export MACOSX_DEPLOYMENT_TARGET=$(sw_vers -productVersion | awk -F. '{print $1"."$2}')
  fi
  make -C lib/luajit-2.1/src -j8 BUILDMODE=static
  cp lib/luajit-2.1/src/libluajit.a _CMAKE/_LUAJIT/libluajit.a
  rm -f lib/luajit-2.1/src/*.o lib/luajit-2.1/src/host/*.o
  rm -f lib/luajit-2.1/src/libluajit.a
  rm -f lib/luajit-2.1/src/host/minilua lib/luajit-2.1/src/host/buildvm
fi

echo "# Building coldsteel (Desktop) ..."
if [ "$LUAJIT" = "1" ]; then
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SVN=ON -DLUAJIT=ON -B _CMAKE/_COLDSTEEL
else
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SVN=ON -B _CMAKE/_COLDSTEEL
fi
cd _CMAKE/_COLDSTEEL
make -j8
mv coldsteel ../../_build/coldsteel
if [ "$(uname)" = "Darwin" ]; then
    mv libbuilder.dylib ../../_build/builder.dylib
else
    mv libbuilder.so ../../_build/builder.so
fi
cd ../..

echo "# Building fonttool ..."
$CXX -std=c++98 -Os -D_IRR_STATIC_LIB_ -I lib/irrlicht190_ogles/include -L _CMAKE/_IRRLICHT -o _build/fonttool fonttool/fonttool.cpp $FONTTOOL_LIBS

# ---- Web (Emscripten) build ----

if [ "$LUAJIT" != "1" ] && command -v emcmake >/dev/null 2>&1; then
  emscripten_build
elif [ "$LUAJIT" != "1" ]; then
  echo "# Emscripten SDK not found, skipping web build ..."
fi

# ---- Haxe wrappers ----

echo "# Generating Haxe wrappers ..."
mkdir -p _build/haxe
swig -xml -xmllite -c++ -o _build/haxe/coldsteel.xml coldsteel.i
haxe --run HaxeWrapperBuilder
rm _build/haxe/coldsteel.xml

# ---- Documentation ----

echo "# Generating documentation ..."
doxygen
python3 gen_doc.py
