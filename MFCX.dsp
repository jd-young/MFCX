# Microsoft Developer Studio Project File - Name="MFCX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MFCX - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MFCX.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/PrEditor2/Tools/predsec", XGFAAAAA"
# PROP Scc_LocalPath "..\..\projects\preditor\tools\predsec"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MFCX - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "Objs\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\expat\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MFCX_LOG_USELOG" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\MFCXs.lib"

!ELSEIF  "$(CFG)" == "MFCX - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "Objs\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\expat\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MFCX_LOG_USELOG" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\MFCXsd.lib"

!ENDIF 

# Begin Target

# Name "MFCX - Win32 Release"
# Name "MFCX - Win32 Debug"
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

SOURCE=.\src\regex\internal.h
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

SOURCE=.\include\Crypt.h
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

SOURCE=.\include\DriveCB.h
# End Source File
# Begin Source File

SOURCE=.\src\DriveCB.h
# End Source File
# Begin Source File

SOURCE=.\include\Filename.h
# End Source File
# Begin Source File

SOURCE=.\include\FolderCB.h
# End Source File
# Begin Source File

SOURCE=.\src\FolderCB.h
# End Source File
# Begin Source File

SOURCE=.\include\GKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\include\GMouse.h
# End Source File
# Begin Source File

SOURCE=.\include\GradStatic.h
# End Source File
# Begin Source File

SOURCE=.\src\GradStatic.h
# End Source File
# Begin Source File

SOURCE=.\include\ImageButton.h
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

SOURCE=.\include\MFCX.rc
# End Source File
# Begin Source File

SOURCE=.\src\MFCX_DLL.h
# End Source File
# Begin Source File

SOURCE=.\include\MFCXres.h
# End Source File
# Begin Source File

SOURCE=.\include\MultiTree.h
# End Source File
# Begin Source File

SOURCE=.\include\PersistColourDlg.h
# End Source File
# Begin Source File

SOURCE=.\include\PersistListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\src\PersistListCtrl.h
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

SOURCE=.\include\SysImageList.h
# End Source File
# Begin Source File

SOURCE=.\src\SysImageList.h
# End Source File
# Begin Source File

SOURCE=.\include\TArray.h
# End Source File
# Begin Source File

SOURCE=.\include\Thread.h
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

SOURCE=.\include\XListBox.h
# End Source File
# Begin Source File

SOURCE=.\include\XListCtrl.h
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

SOURCE=.\include\XStringList.h
# End Source File
# Begin Source File

SOURCE=.\include\XTime.h
# End Source File
# Begin Source File

SOURCE=.\include\XTreeCtrl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\res\folders.bmp
# End Source File
# Begin Source File

SOURCE=.\include\res\folders_disabled.bmp
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
# End Target
# End Project
