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

set BUILDOPT=
if /I "%1"=="all" set BUILD="All"
if /I "%1"=="debug" set BUILD="MFCX - Win32 Debug"
if /I "%1"=="release" set BUILD="MFCX - Win32 Release"
if "%1"=="" goto Error

if /I "%2"=="clean" set BUILDOPT=/CLEAN
if /I "%2"=="rebuild" set BUILDOPT=/REBUILD

echo Building %BUILD%...

call "\Program Files\Microsoft Visual Studio\VC98\Bin\VCVARS32.BAT"
msdev MFCX.dsp /MAKE %BUILD% %BUILDOPT% /USEENV

echo Done!
goto End

:Error
echo No build specified (should be 'Debug', 'Release', or 'All').

:End
