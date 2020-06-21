# Microsoft Developer Studio Project File - Name="MFCX_DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MFCX_DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MFCX_DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MFCX_DLL.mak" CFG="MFCX_DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MFCX_DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MFCX_DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MFCX_DLL - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "objs\ReleaseDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\expat\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "MFCX_LOG_USELOG" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 version.lib xmlparse.lib xmltok.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"lib/MFCX.dll" /libpath:"..\expat\lib"

!ELSEIF  "$(CFG)" == "MFCX_DLL - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFCX_DLL___Win32_Debug"
# PROP BASE Intermediate_Dir "MFCX_DLL___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "objs\DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\expat\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "MFCX_LOG_USELOG" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib xmlparse.lib xmltok.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"lib/MFCXd.dll" /pdbtype:sept /libpath:"..\expat\lib"

!ENDIF 

# Begin Target

# Name "MFCX_DLL - Win32 Release"
# Name "MFCX_DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\BrowseForFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\src\regex\chartables.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ColourPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Crypt.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Directory.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Dragger.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DragMultiTree.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DriveCB.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Filename.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FolderCB.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GradStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ImageButton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MFCX_DLL.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MultiTree.cpp
# End Source File
# Begin Source File

SOURCE=.\src\regex\pcre.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PersistColourDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PersistListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Regex.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RegexString.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RegKey.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SBDestination.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Splasher.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\src\SysImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xml.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XRecentFile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XScrollView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XString.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XTime.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XTreeCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\BrowseForFolder.h
# End Source File
# Begin Source File

SOURCE=.\include\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=.\include\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\include\DataQueue.h
# End Source File
# Begin Source File

SOURCE=.\include\Directory.h
# End Source File
# Begin Source File

SOURCE=.\include\Dragger.h
# End Source File
# Begin Source File

SOURCE=.\include\DragMultiTree.h
# End Source File
# Begin Source File

SOURCE=.\include\Filename.h
# End Source File
# Begin Source File

SOURCE=.\include\GKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\include\GMouse.h
# End Source File
# Begin Source File

SOURCE=.\src\regex\internal.h
# End Source File
# Begin Source File

SOURCE=.\include\Logger.h
# End Source File
# Begin Source File

SOURCE=.\include\MenuButton.h
# End Source File
# Begin Source File

SOURCE=.\include\MFCX.h
# End Source File
# Begin Source File

SOURCE=.\MFCX_DLL.h
# End Source File
# Begin Source File

SOURCE=.\include\MultiTree.h
# End Source File
# Begin Source File

SOURCE=.\include\PersistColourDlg.h
# End Source File
# Begin Source File

SOURCE=.\include\Regex.h
# End Source File
# Begin Source File

SOURCE=.\include\RegexString.h
# End Source File
# Begin Source File

SOURCE=.\include\RegKey.h
# End Source File
# Begin Source File

SOURCE=.\include\SBDestination.h
# End Source File
# Begin Source File

SOURCE=.\include\Sound.h
# End Source File
# Begin Source File

SOURCE=.\include\Splasher.h
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\include\TArray.h
# End Source File
# Begin Source File

SOURCE=.\include\Thread.h
# End Source File
# Begin Source File

SOURCE=.\include\Time.h
# End Source File
# Begin Source File

SOURCE=.\include\TList.h
# End Source File
# Begin Source File

SOURCE=.\include\TMap.h
# End Source File
# Begin Source File

SOURCE=.\include\Utils.h
# End Source File
# Begin Source File

SOURCE=.\include\VerInfo.h
# End Source File
# Begin Source File

SOURCE=.\include\XComboBox.h
# End Source File
# Begin Source File

SOURCE=.\include\XListBox.h
# End Source File
# Begin Source File

SOURCE=.\include\xml.h
# End Source File
# Begin Source File

SOURCE=.\include\XRecentFile.h
# End Source File
# Begin Source File

SOURCE=.\include\XScrollView.h
# End Source File
# Begin Source File

SOURCE=.\include\XString.h
# End Source File
# Begin Source File

SOURCE=.\include\XTreeCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\include\MFCX.rc
# End Source File
# Begin Source File

SOURCE=.\include\res\MFCX.rc2
# End Source File
# Begin Source File

SOURCE=.\include\MFCXres.h
# End Source File
# Begin Source File

SOURCE=.\include\res\xlbDelete.bmp
# End Source File
# Begin Source File

SOURCE=.\include\res\xlbMoveDown.bmp
# End Source File
# Begin Source File

SOURCE=.\include\res\xlbMoveUp.bmp
# End Source File
# Begin Source File

SOURCE=.\include\res\xlbNew.bmp
# End Source File
# End Group
# End Target
# End Project
