@echo off

echo # Generating AngelScript wrapper ...
python process_defs.py

echo # Creating folders for Cmake ...
mkdir _CMAKE
cd _CMAKE
mkdir _COLDSTEEL
mkdir _COLDSTEEL_RT
mkdir _IRRLICHT
cd ..

echo # Configuring Irrlicht ...
cd _CMAKE/_IRRLICHT
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-DNO_IRR_COMPILE_WITH_SOFTWARE_ -DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_" -DCMAKE_BUILD_TYPE=MinSizeRel -DIRRLICHT_M32=ON -DIRRLICHT_SHARED=OFF -DSDL_ENABLED=ON -DSDL_PATH=../../lib/sdl ../../lib/irrlicht
cd ..\..

echo # Configuring ColdSteel ...
cd _CMAKE/_COLDSTEEL
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_BUILD_TYPE=MinSizeRel ../..
cd ..\..

echo # Configuring ColdSteel (runtime only version) ...
cd _CMAKE/_COLDSTEEL_RT
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-m32 -DRUNTIME_ONLY -DAS_NO_COMPILER" -DCMAKE_EXE_LINKER_FLAGS="-mwindows" -DCMAKE_BUILD_TYPE=MinSizeRel ../..
cd ..\..
