@echo off
:: 
::   seed-reg.bat Seeds the registry with entries required for the RegKeyTest 
::   unit test.
::
::   Written by John Young, 1 March 2023
::   Copyright (c) 2023 Thistle Software
::

:: The compatability layer is to stop regedit from prompting for an admin 
:: password, and the /S regedit switch is to stop it popping up a warning.
set BASE_DIR=%~dp0
cmd /min /C "set __COMPAT_LAYER=RUNASINVOKER && start regedit /S %BASE_DIR%\RegKeyTest.reg" 
