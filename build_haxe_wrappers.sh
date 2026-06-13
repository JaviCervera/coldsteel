#!/bin/sh
cd `dirname $0`

mkdir -p _build/haxe
swig -xml -xmllite -c++ -o _build/haxe/coldsteel.xml coldsteel.i
haxe --run HaxeWrapperBuilder
rm _build/haxe/coldsteel.xml
