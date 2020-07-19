@echo off

echo # Generating AngelScript wrapper ...
python process_defs.py

echo # Creating folders for Cmake ...
mkdir _CMAKE
cd _CMAKE
mkdir _ASTRO
mkdir _ASTRO_RT
mkdir _IRRLICHT
cd ..

echo # Configuring Irrlicht ...
cd _CMAKE/_IRRLICHT
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF -DSDL_ENABLED=ON -DSDL_PATH=../../lib/sdl ../../lib/irrlicht
cd ..\..

echo # Configuring Astro ...
cd _CMAKE/_ASTRO
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_BUILD_TYPE=MinSizeRel ../..
cd ..\..

echo # Configuring Astro (runtime only version) ...
cd _CMAKE/_ASTRO_RT
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-m32 -DRUNTIME_ONLY -DAS_NO_COMPILER" -DCMAKE_EXE_LINKER_FLAGS="-mwindows" -DCMAKE_BUILD_TYPE=MinSizeRel ../..
cd ..\..
