@echo off

echo # Compiling Irrlicht ...
cd _CMAKE\_IRRLICHT
mingw32-make
cd ..\..

echo # Compiling Astro ...
cd _CMAKE\_ASTRO
mingw32-make
cd ..\..
move "_CMAKE\_ASTRO\astro.exe" "_build\astro.exe"

echo # Compiling Astro (runtime only version) ...
cd _CMAKE\_ASTRO_RT
mingw32-make
cd ..\..
move "_CMAKE\_ASTRO_RT\astro.exe" "_build\astro_rt.exe"
