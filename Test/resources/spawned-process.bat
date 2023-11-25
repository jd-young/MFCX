@echo off
:: 
::   spawned-process.bat      A process that is spawned by ThreadTest to test
::                            that a process can be stopped when requested.
::
::   Written by John Young, 22 November 2023
::   Copyright (c) 2023 Thistle Software
::

setlocal enabledelayedexpansion

if "%1"=="" (
    set "duration=5000"    rem 5 seconds
) else (
    set "duration=%1"
)
set "delay=50"           rem 50 milliseconds per iteration

rem Calculate the number of iterations
set /a "iterations=%duration% / %delay%"

echo Printing %iterations% messages every %delay% msecs for %duration% msecs.
:: print out current directory
cd


set /a "centisecs=%delay% / 10"
rem Loop through each iteration
set "start_time=!time: =0!"
for /l %%i in (1,1,%iterations%) do (
    echo Message %%i
    call :wait %centisecs%
)
set "end_time=!time: =0!"

echo Script complete!
echo Start time: %start_time%
echo End time: %end_time%
goto :EOF


:wait centiseconds
for /F "tokens=3,4 delims=:." %%a in ("%time%") do set /A sec=1%%a, msec=1%%b+%1
if %msec% gtr 199 set /A sec+=1, msec-=100
set lim=%sec%%msec%
:waitHere
for /F "tokens=3,4 delims=:." %%a in ("%time%") do if 1%%a1%%b lss %lim% goto waitHere
exit /B
set "end_time=!time: =0!"