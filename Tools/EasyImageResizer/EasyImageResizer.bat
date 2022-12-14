setlocal enabledelayedexpansion
@echo off

cd /d %~dp0

set /p resolution="Resolution: "
set /p compression="Compression: "

set x=%*
for %%i in (!x!) do (
    .\EasyImageResizer.py -i %%i -r !resolution! -c !compression!
    echo %%i resized to !resolution! and compressed to !compression!
)
