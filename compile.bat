@echo off

echo # Compiling Irrlicht ...
cd _CMAKE\_IRRLICHT
mingw32-make
cd ..\..

echo # Compiling ColdSteel ...
cd _CMAKE\_COLDSTEEL
mingw32-make
cd ..\..
move "_CMAKE\_COLDSTEEL\coldsteel.exe" "_build\coldsteel.exe"
