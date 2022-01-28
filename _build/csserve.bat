@echo off
cd /D "%~dp0"
python csbuild.py html %1
echo Launching server...
echo - Go to http://localhost:8000 on your browser.
echo - Ctrl+C and confirm to finish.
cd ..
cd %1.build
python -m http.server
