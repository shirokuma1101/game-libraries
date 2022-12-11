setlocal enabledelayedexpansion
@echo off

cd %~dp0
set x=%*

for %%i in (!x!) do (
    .\VideoToSequentialImage.py -i %%i -o test.png -f 30 -w 10
)
