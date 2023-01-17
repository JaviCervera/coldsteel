#!/bin/sh
cd `dirname $0`

mkdir _build/haxe
swig -xml -xmllite -c++ -o _build/haxe/coldsteel.xml coldsteel.i
haxe -m HaxeWrapperBuilder --interp
rm _build/haxe/coldsteel.xml
