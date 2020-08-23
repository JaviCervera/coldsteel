#!/bin/sh
cd `dirname $0`

python3 process_defs.py

mkdir _CMAKE
cd _CMAKE
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel ..
make
cd ..
mv _CMAKE/coldsteel_exe _build/coldsteel.bin
