@echo off

mkdir _build\haxe
swig.exe -xml -xmllite -c++ -o _build/haxe/coldsteel.xml coldsteel.i
haxe -m HaxeWrapperBuilder --interp
del _build\haxe\coldsteel.xml
