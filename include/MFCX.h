//
//   MFCX.h         MFC eXtension library
//
//   Written by John Young, 23 April 2000
//   Copyright (c) 2000 Young Associates
//

#ifndef   __MFCX_H
#define   __MFCX_H

// Version is 0xMMmmrr - No build number :-)
#define MFCX_VERSION	0x300000

#ifdef MFCXDLL
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"mfcxud.lib") 
			#pragma message("Automatically linking with mfcxud.dll")
		#else
			#pragma comment(lib,"mfcxd.lib") 
			#pragma message("Automatically linking with mfcxd.dll")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"mfcxu.lib") 
			#pragma message("Automatically linking with mfcxu.dll") 
		#else
			#pragma comment(lib,"mfcx.lib") 
			#pragma message("Automatically linking with mfcx.dll") 
		#endif
	#endif
#else
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"mfcxsud.lib") 
			#pragma message("Automatically linking with mfcxsud.lib") 
		#else
			#pragma comment(lib,"mfcxsd.lib") 
			#pragma message("Automatically linking with mfcxsd.lib") 
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"mfcxsu.lib") 
			#pragma message("Automatically linking with mfcxsU.lib") 
		#else
			#pragma comment(lib,"mfcxs.lib") 
			#pragma message("Automatically linking with mfcxs.lib") 
		#endif
	#endif
#endif


#include "BrowseForFolder.h"	// Folder picker
#include "ColourPicker.h"	// Colour picker
#include "ColourPopup.h"		// Colour picker
#include "Crypt.h"            // Encrypt / decrypt library
#include "DataQueue.h"		// Thread-safe queue
#include "Directory.h"		// Directory functions
#include "DragMultiTree.h"	// Tree control that allows drag'n'drop
#include "Dragger.h"		// Dragging functionality
#include "DriveCB.h"		// A drive combo-box chooser
#include "Filename.h"		// Filename manipulation
#include "FolderCB.h"		// A folder combo-box chooser
#include "GradStatic.h"		// A gradient static control
#include "GKeyboard.h"		// The global keyboard object
#include "GMouse.h"			// The global mouse object
#include "ImageButton.h"      // A button with a picture on it.
#include "Logger.h"			// A debug logger
#include "MenuButton.h"		// A button with a menu
#include "MultiTree.h"		// Multi-select tree
#include "PersistColourDlg.h"	// Colour selector that remembers previous selections
#include "PersistListCtrl.h"	// A persistent CXListCtrl
#include "RegKey.h"			// Registry key
#include "Regex.h"			// A regular expression class
#include "RegexString.h"		// Regular expression string
#include "SBDestination.h"	// Folder picker
#include "Sound.h"			// Sound manipulation
#include "Splasher.h"		// A splasher screen
#include "StringUtil.h"		// A string utility
#include "TArray.h"			// Any array template
#include "TList.h"			// A list template
#include "TMap.h"			// A map template
#include "Thread.h"			// Thread manipulation
#include "SysImageList.h"	// A sytem image list
#include "Utils.h"			// General utilities
#include "VerInfo.h"		// Version information
#include "XListBox.h"		// An extended list box
#include "XRecentFile.h"		// An extended recent files class
#include "XString.h"         	// Extended string
#include "XStringList.h"		// Extended string list that can be sorted
#include "XTreeCtrl.h"        // Extended tree control 
#include "XTime.h"			// Time processing

#endif // __MFCX_H
