# Microsoft Developer Studio Generated NMAKE File, Based on MFCX.dsp
!IF "$(CFG)" == ""
CFG=MFCX - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MFCX - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MFCX - Win32 Release" && "$(CFG)" != "MFCX - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MFCX.mak" CFG="MFCX - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MFCX - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MFCX - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "MFCX - Win32 Release"

OUTDIR=.\lib
INTDIR=.\Objs\Release
# Begin Custom Macros
OutDir=.\lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\MFCXs.lib"

!ELSE 

ALL : "maketables - Win32 Release" "$(OUTDIR)\MFCXs.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"maketables - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\BrowseForFolder.obj"
	-@erase "$(INTDIR)\chartables.obj"
	-@erase "$(INTDIR)\ColourPicker.obj"
	-@erase "$(INTDIR)\ColourPopup.obj"
	-@erase "$(INTDIR)\Crypt.obj"
	-@erase "$(INTDIR)\DataQueue.obj"
	-@erase "$(INTDIR)\Directory.obj"
	-@erase "$(INTDIR)\Dragger.obj"
	-@erase "$(INTDIR)\DragMultiTree.obj"
	-@erase "$(INTDIR)\DriveCB.obj"
	-@erase "$(INTDIR)\Filename.obj"
	-@erase "$(INTDIR)\FolderCB.obj"
	-@erase "$(INTDIR)\GKeyboard.obj"
	-@erase "$(INTDIR)\GMouse.obj"
	-@erase "$(INTDIR)\GradStatic.obj"
	-@erase "$(INTDIR)\ImageButton.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\MenuButton.obj"
	-@erase "$(INTDIR)\MFCX.pch"
	-@erase "$(INTDIR)\MFCX.res"
	-@erase "$(INTDIR)\MFCX_DLL.obj"
	-@erase "$(INTDIR)\MultiTree.obj"
	-@erase "$(INTDIR)\pcre.obj"
	-@erase "$(INTDIR)\PersistColourDlg.obj"
	-@erase "$(INTDIR)\PersistListCtrl.obj"
	-@erase "$(INTDIR)\Regex.obj"
	-@erase "$(INTDIR)\RegexString.obj"
	-@erase "$(INTDIR)\RegKey.obj"
	-@erase "$(INTDIR)\SBDestination.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\Splasher.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysImageList.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VerInfo.obj"
	-@erase "$(INTDIR)\XListBox.obj"
	-@erase "$(INTDIR)\XListCtrl.obj"
	-@erase "$(INTDIR)\xml.obj"
	-@erase "$(INTDIR)\XRecentFile.obj"
	-@erase "$(INTDIR)\XScrollView.obj"
	-@erase "$(INTDIR)\XString.obj"
	-@erase "$(INTDIR)\XStringList.obj"
	-@erase "$(INTDIR)\XTime.obj"
	-@erase "$(INTDIR)\XTreeCtrl.obj"
	-@erase "$(OUTDIR)\MFCXs.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\expat\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MFCX_LOG_USELOG" /Fp"$(INTDIR)\MFCX.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\MFCX.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MFCX.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\MFCXs.lib" 
LIB32_OBJS= \
	"$(INTDIR)\BrowseForFolder.obj" \
	"$(INTDIR)\chartables.obj" \
	"$(INTDIR)\ColourPicker.obj" \
	"$(INTDIR)\ColourPopup.obj" \
	"$(INTDIR)\Crypt.obj" \
	"$(INTDIR)\DataQueue.obj" \
	"$(INTDIR)\Directory.obj" \
	"$(INTDIR)\Dragger.obj" \
	"$(INTDIR)\DragMultiTree.obj" \
	"$(INTDIR)\DriveCB.obj" \
	"$(INTDIR)\Filename.obj" \
	"$(INTDIR)\FolderCB.obj" \
	"$(INTDIR)\GKeyboard.obj" \
	"$(INTDIR)\GMouse.obj" \
	"$(INTDIR)\GradStatic.obj" \
	"$(INTDIR)\ImageButton.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\MenuButton.obj" \
	"$(INTDIR)\MFCX_DLL.obj" \
	"$(INTDIR)\MultiTree.obj" \
	"$(INTDIR)\pcre.obj" \
	"$(INTDIR)\PersistColourDlg.obj" \
	"$(INTDIR)\PersistListCtrl.obj" \
	"$(INTDIR)\Regex.obj" \
	"$(INTDIR)\RegexString.obj" \
	"$(INTDIR)\RegKey.obj" \
	"$(INTDIR)\SBDestination.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\Splasher.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysImageList.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\VerInfo.obj" \
	"$(INTDIR)\XListBox.obj" \
	"$(INTDIR)\XListCtrl.obj" \
	"$(INTDIR)\xml.obj" \
	"$(INTDIR)\XRecentFile.obj" \
	"$(INTDIR)\XScrollView.obj" \
	"$(INTDIR)\XString.obj" \
	"$(INTDIR)\XStringList.obj" \
	"$(INTDIR)\XTime.obj" \
	"$(INTDIR)\XTreeCtrl.obj" \
	"$(INTDIR)\MFCX.res"

"$(OUTDIR)\MFCXs.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MFCX - Win32 Debug"

OUTDIR=.\lib
INTDIR=.\Objs\Debug
# Begin Custom Macros
OutDir=.\lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\MFCXsd.lib"

!ELSE 

ALL : "maketables - Win32 Debug" "$(OUTDIR)\MFCXsd.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"maketables - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\BrowseForFolder.obj"
	-@erase "$(INTDIR)\chartables.obj"
	-@erase "$(INTDIR)\ColourPicker.obj"
	-@erase "$(INTDIR)\ColourPopup.obj"
	-@erase "$(INTDIR)\Crypt.obj"
	-@erase "$(INTDIR)\DataQueue.obj"
	-@erase "$(INTDIR)\Directory.obj"
	-@erase "$(INTDIR)\Dragger.obj"
	-@erase "$(INTDIR)\DragMultiTree.obj"
	-@erase "$(INTDIR)\DriveCB.obj"
	-@erase "$(INTDIR)\Filename.obj"
	-@erase "$(INTDIR)\FolderCB.obj"
	-@erase "$(INTDIR)\GKeyboard.obj"
	-@erase "$(INTDIR)\GMouse.obj"
	-@erase "$(INTDIR)\GradStatic.obj"
	-@erase "$(INTDIR)\ImageButton.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\MenuButton.obj"
	-@erase "$(INTDIR)\MFCX.pch"
	-@erase "$(INTDIR)\MFCX.res"
	-@erase "$(INTDIR)\MFCX_DLL.obj"
	-@erase "$(INTDIR)\MultiTree.obj"
	-@erase "$(INTDIR)\pcre.obj"
	-@erase "$(INTDIR)\PersistColourDlg.obj"
	-@erase "$(INTDIR)\PersistListCtrl.obj"
	-@erase "$(INTDIR)\Regex.obj"
	-@erase "$(INTDIR)\RegexString.obj"
	-@erase "$(INTDIR)\RegKey.obj"
	-@erase "$(INTDIR)\SBDestination.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\Splasher.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysImageList.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VerInfo.obj"
	-@erase "$(INTDIR)\XListBox.obj"
	-@erase "$(INTDIR)\XListCtrl.obj"
	-@erase "$(INTDIR)\xml.obj"
	-@erase "$(INTDIR)\XRecentFile.obj"
	-@erase "$(INTDIR)\XScrollView.obj"
	-@erase "$(INTDIR)\XString.obj"
	-@erase "$(INTDIR)\XStringList.obj"
	-@erase "$(INTDIR)\XTime.obj"
	-@erase "$(INTDIR)\XTreeCtrl.obj"
	-@erase "$(OUTDIR)\MFCXsd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\expat\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MFCX_LOG_USELOG" /Fp"$(INTDIR)\MFCX.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\MFCX.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MFCX.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\MFCXsd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\BrowseForFolder.obj" \
	"$(INTDIR)\chartables.obj" \
	"$(INTDIR)\ColourPicker.obj" \
	"$(INTDIR)\ColourPopup.obj" \
	"$(INTDIR)\Crypt.obj" \
	"$(INTDIR)\DataQueue.obj" \
	"$(INTDIR)\Directory.obj" \
	"$(INTDIR)\Dragger.obj" \
	"$(INTDIR)\DragMultiTree.obj" \
	"$(INTDIR)\DriveCB.obj" \
	"$(INTDIR)\Filename.obj" \
	"$(INTDIR)\FolderCB.obj" \
	"$(INTDIR)\GKeyboard.obj" \
	"$(INTDIR)\GMouse.obj" \
	"$(INTDIR)\GradStatic.obj" \
	"$(INTDIR)\ImageButton.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\MenuButton.obj" \
	"$(INTDIR)\MFCX_DLL.obj" \
	"$(INTDIR)\MultiTree.obj" \
	"$(INTDIR)\pcre.obj" \
	"$(INTDIR)\PersistColourDlg.obj" \
	"$(INTDIR)\PersistListCtrl.obj" \
	"$(INTDIR)\Regex.obj" \
	"$(INTDIR)\RegexString.obj" \
	"$(INTDIR)\RegKey.obj" \
	"$(INTDIR)\SBDestination.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\Splasher.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysImageList.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\VerInfo.obj" \
	"$(INTDIR)\XListBox.obj" \
	"$(INTDIR)\XListCtrl.obj" \
	"$(INTDIR)\xml.obj" \
	"$(INTDIR)\XRecentFile.obj" \
	"$(INTDIR)\XScrollView.obj" \
	"$(INTDIR)\XString.obj" \
	"$(INTDIR)\XStringList.obj" \
	"$(INTDIR)\XTime.obj" \
	"$(INTDIR)\XTreeCtrl.obj" \
	"$(INTDIR)\MFCX.res"

"$(OUTDIR)\MFCXsd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MFCX.dep")
!INCLUDE "MFCX.dep"
!ELSE 
!MESSAGE Warning: cannot find "MFCX.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MFCX - Win32 Release" || "$(CFG)" == "MFCX - Win32 Debug"
SOURCE=.\src\BrowseForFolder.cpp

"$(INTDIR)\BrowseForFolder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\regex\chartables.cpp

"$(INTDIR)\chartables.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\ColourPicker.cpp

"$(INTDIR)\ColourPicker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\ColourPopup.cpp

"$(INTDIR)\ColourPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Crypt.cpp

"$(INTDIR)\Crypt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\DataQueue.cpp

"$(INTDIR)\DataQueue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Directory.cpp

"$(INTDIR)\Directory.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Dragger.cpp

"$(INTDIR)\Dragger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\DragMultiTree.cpp

"$(INTDIR)\DragMultiTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\DriveCB.cpp

"$(INTDIR)\DriveCB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Filename.cpp

"$(INTDIR)\Filename.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\FolderCB.cpp

"$(INTDIR)\FolderCB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\GKeyboard.cpp

"$(INTDIR)\GKeyboard.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\GMouse.cpp

"$(INTDIR)\GMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\GradStatic.cpp

"$(INTDIR)\GradStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\ImageButton.cpp

"$(INTDIR)\ImageButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Logger.cpp

"$(INTDIR)\Logger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\MenuButton.cpp

"$(INTDIR)\MenuButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\MFCX_DLL.cpp

"$(INTDIR)\MFCX_DLL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\MultiTree.cpp

"$(INTDIR)\MultiTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\regex\pcre.cpp

"$(INTDIR)\pcre.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\PersistColourDlg.cpp

"$(INTDIR)\PersistColourDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\PersistListCtrl.cpp

"$(INTDIR)\PersistListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Regex.cpp

"$(INTDIR)\Regex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\RegexString.cpp

"$(INTDIR)\RegexString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\RegKey.cpp

"$(INTDIR)\RegKey.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\SBDestination.cpp

"$(INTDIR)\SBDestination.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Sound.cpp

"$(INTDIR)\Sound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Splasher.cpp

"$(INTDIR)\Splasher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\StdAfx.cpp

!IF  "$(CFG)" == "MFCX - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\expat\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MFCX_LOG_USELOG" /Fp"$(INTDIR)\MFCX.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MFCX.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MFCX - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\expat\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MFCX_LOG_USELOG" /Fp"$(INTDIR)\MFCX.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MFCX.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\SysImageList.cpp

"$(INTDIR)\SysImageList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Thread.cpp

"$(INTDIR)\Thread.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\Utils.cpp

"$(INTDIR)\Utils.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\VerInfo.cpp

"$(INTDIR)\VerInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XListBox.cpp

"$(INTDIR)\XListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XListCtrl.cpp

"$(INTDIR)\XListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\xml.cpp

"$(INTDIR)\xml.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XRecentFile.cpp

"$(INTDIR)\XRecentFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XScrollView.cpp

"$(INTDIR)\XScrollView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XString.cpp

"$(INTDIR)\XString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XStringList.cpp

"$(INTDIR)\XStringList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XTime.cpp

"$(INTDIR)\XTime.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\XTreeCtrl.cpp

"$(INTDIR)\XTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MFCX.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\include\MFCX.rc

!IF  "$(CFG)" == "MFCX - Win32 Release"


"$(INTDIR)\MFCX.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\MFCX.res" /i "include" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "MFCX - Win32 Debug"


"$(INTDIR)\MFCX.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\MFCX.res" /i "include" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "MFCX - Win32 Release"

"maketables - Win32 Release" : 
   cd ".\src\regex\maketables"
   $(MAKE) /$(MAKEFLAGS) /F .\maketables.mak CFG="maketables - Win32 Release" 
   cd "..\..\.."

"maketables - Win32 ReleaseCLEAN" : 
   cd ".\src\regex\maketables"
   $(MAKE) /$(MAKEFLAGS) /F .\maketables.mak CFG="maketables - Win32 Release" RECURSE=1 CLEAN 
   cd "..\..\.."

!ELSEIF  "$(CFG)" == "MFCX - Win32 Debug"

"maketables - Win32 Debug" : 
   cd ".\src\regex\maketables"
   $(MAKE) /$(MAKEFLAGS) /F .\maketables.mak CFG="maketables - Win32 Debug" 
   cd "..\..\.."

"maketables - Win32 DebugCLEAN" : 
   cd ".\src\regex\maketables"
   $(MAKE) /$(MAKEFLAGS) /F .\maketables.mak CFG="maketables - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\..\.."

!ENDIF 


!ENDIF 

