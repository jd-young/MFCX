# Microsoft Developer Studio Generated NMAKE File, Based on TestXList.dsp
!IF "$(CFG)" == ""
CFG=TestXList - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TestXList - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TestXList - Win32 Release" && "$(CFG)" != "TestXList - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TestXList.mak" CFG="TestXList - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestXList - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TestXList - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TestXList - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TestXList.exe"

!ELSE 

ALL : "MFCX - Win32 Release" "$(OUTDIR)\TestXList.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"MFCX - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TestXList.obj"
	-@erase "$(INTDIR)\TestXList.pch"
	-@erase "$(INTDIR)\TestXList.res"
	-@erase "$(INTDIR)\TestXListDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TestXList.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TestXList.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\TestXList.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TestXList.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\TestXList.pdb" /machine:I386 /out:"$(OUTDIR)\TestXList.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TestXList.obj" \
	"$(INTDIR)\TestXListDlg.obj" \
	"$(INTDIR)\TestXList.res" \
	"..\..\lib\MFCXs.lib"

"$(OUTDIR)\TestXList.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TestXList - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TestXList.exe"

!ELSE 

ALL : "MFCX - Win32 Debug" "$(OUTDIR)\TestXList.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"MFCX - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TestXList.obj"
	-@erase "$(INTDIR)\TestXList.pch"
	-@erase "$(INTDIR)\TestXList.res"
	-@erase "$(INTDIR)\TestXListDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TestXList.exe"
	-@erase "$(OUTDIR)\TestXList.ilk"
	-@erase "$(OUTDIR)\TestXList.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TestXList.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\TestXList.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TestXList.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\TestXList.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TestXList.exe" /pdbtype:sept /libpath:"..\..\lib" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TestXList.obj" \
	"$(INTDIR)\TestXListDlg.obj" \
	"$(INTDIR)\TestXList.res" \
	"..\..\lib\MFCXsd.lib"

"$(OUTDIR)\TestXList.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TestXList.dep")
!INCLUDE "TestXList.dep"
!ELSE 
!MESSAGE Warning: cannot find "TestXList.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TestXList - Win32 Release" || "$(CFG)" == "TestXList - Win32 Debug"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TestXList - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TestXList.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TestXList.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TestXList - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TestXList.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TestXList.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TestXList.cpp

"$(INTDIR)\TestXList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TestXList.pch"


SOURCE=.\TestXList.rc

"$(INTDIR)\TestXList.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\TestXListDlg.cpp

"$(INTDIR)\TestXListDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TestXList.pch"


!IF  "$(CFG)" == "TestXList - Win32 Release"

"MFCX - Win32 Release" : 
   cd "\Work\Libraries\MFCX"
   $(MAKE) /$(MAKEFLAGS) /F .\MFCX.mak CFG="MFCX - Win32 Release" 
   cd ".\Test\TestXList"

"MFCX - Win32 ReleaseCLEAN" : 
   cd "\Work\Libraries\MFCX"
   $(MAKE) /$(MAKEFLAGS) /F .\MFCX.mak CFG="MFCX - Win32 Release" RECURSE=1 CLEAN 
   cd ".\Test\TestXList"

!ELSEIF  "$(CFG)" == "TestXList - Win32 Debug"

"MFCX - Win32 Debug" : 
   cd "\Work\Libraries\MFCX"
   $(MAKE) /$(MAKEFLAGS) /F .\MFCX.mak CFG="MFCX - Win32 Debug" 
   cd ".\Test\TestXList"

"MFCX - Win32 DebugCLEAN" : 
   cd "\Work\Libraries\MFCX"
   $(MAKE) /$(MAKEFLAGS) /F .\MFCX.mak CFG="MFCX - Win32 Debug" RECURSE=1 CLEAN 
   cd ".\Test\TestXList"

!ENDIF 


!ENDIF 

