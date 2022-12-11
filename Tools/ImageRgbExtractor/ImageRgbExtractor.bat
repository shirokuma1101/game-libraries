setlocal enabledelayedexpansion
@echo off

cd %~dp0

set /p output_file="Output file: "

set x=%*
for %%i in (!x!) do (
    .\ImageRgbExtractor.py -i %%i -o !output_file!
)
