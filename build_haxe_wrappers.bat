@echo off

mkdir _build\haxe
swig.exe -xml -xmllite -c++ -o _build/haxe/coldsteel.xml coldsteel.i
haxe --run HaxeWrapperBuilder
del _build\haxe\coldsteel.xml
