@echo off

echo # Compiling Irrlicht ...
cd _CMAKE/_IRRLICHT
mingw32-make
cd ..

echo # Compiling Astro ...
mingw32-make
cd ..
move "_CMAKE\astro_exe.exe" "_build\astro.exe"
