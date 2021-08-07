@echo off
rem  Build.bat    - Builds the MFCX libraries.
rem 
rem  Written by John Young, 17 May 2003
rem  Copyright (c) Thistle Software
rem
rem USAGE
rem		Build <All | Debug | Release> [clean | rebuild]
rem
rem		where
rem			All		means all the build types (Debug, Beta, & Release)
rem			Debug	means the Debug build
rem			Beta		means the Beta build
rem			Release	means the Release build
rem			clean	means just clean (don't to a build)
rem			rebuild	means clean then build
rem
rem		If 'clean' and 'rebuild' are omitted, then a build is done.
rem

echo "This is out of date - build using VS2017"
goto End

set MSVS="C:\Program Files (x86)\Microsoft Visual Studio"
set VCVARS=%MSVS%\VC98\Bin\VCVARS32.BAT
set MSDEV=%MSVS%\Common\MSDev98\Bin\msdev.exe

set BUILDOPT=
if /I "%1"=="all" set BUILD="All"
if /I "%1"=="debug" set BUILD="MFCX - Win32 Debug"
if /I "%1"=="release" set BUILD="MFCX - Win32 Release"
if "%1"=="" goto Error

if /I "%2"=="clean" set BUILDOPT=/CLEAN
if /I "%2"=="rebuild" set BUILDOPT=/REBUILD

echo Building %BUILD%...

set PATH_OLD=%PATH%
set INCLUDE_OLD=%INCLUDE%
set LIB_OLD=%LIB%
call %VCVARS%

echo Building binary
%MSDEV% MFCX.dsp /MAKE %BUILD% %BUILDOPT% /USEENV
pause

echo Restore INCLUDE and LIB
set PATH=%PATH_OLD%
set INCLUDE=%INCLUDE_OLD%
set LIB=%LIB_OLD%

set INCLUDE_OLD=
set LIB_OLD=

echo Done!
goto End

:Error
echo No build specified (should be 'Debug', 'Release', or 'All').

:End
