@echo off

python process_defs.py

mkdir _CMAKE
cd _CMAKE
cmake -G "MinGW Makefiles" -DCMAKE_C_FLAGS=-m32 -DCMAKE_BUILD_TYPE=MinSizeRel ..
mingw32-make
cd ..
move "_CMAKE\astro_exe.exe" "_build\astro.exe"
pause
