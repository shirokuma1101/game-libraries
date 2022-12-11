setlocal enabledelayedexpansion
@echo off

cd %~dp0

set x=%*
for %%i in (!x!) do (
    ffmpeg -i %%i -acodec pcm_s16le -f wav "%%~ni.wav"
)
