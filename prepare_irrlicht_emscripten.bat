@echo off
cd lib/irrlicht190_ogles
emcmake cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_SHARED=OFF -B ../../_CMAKE/_IRRLICHT_EMSCRIPTEN