@echo off

mkdir _build\haxe
swig.exe -xml -xmllite -c++ -o _build/haxe/coldsteel.xml coldsteel.i
haxe --cpp _build/haxe/HaxeWrapperBuilder -m HaxeWrapperBuilder
_build\haxe\HaxeWrapperBuilder\HaxeWrapperBuilder
rem del _build\haxe\coldsteel.xml
rem rmdir /s /q _build\haxe\HaxeWrapperBuilder
