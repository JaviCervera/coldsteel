@echo off
cd lib/irrlicht190_ogles
emcmake cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT_EMSCRIPTEN
