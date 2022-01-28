@echo off
cd /D "%~dp0"
python csbuild.py desktop %1
cd %1.build
"%~nx0"
