/*!
	\file	SysImageList.cpp         System Image List handler class

	\author	John Young
	\date	7 May 2000

	Copyright (c) 2000 Thistle Software

	This is used for objects that need access to the system image list.  MFC 
	limits one CImageList per HIMAGELIST handle, and we don't want to delete
	the system image (which happens if the handle is attached to the CImageList 
	when it destructs.
*/

#include "stdafx.h"
#include "../include/SysImageList.h"


int CSysImageList::m_nRefCount = 0;


CSysImageList::CSysImageList()
{
     m_pImageList = NULL;

     // We need to implement reference counting to overcome an MFC limitation 
     // whereby you cannot have two CImageLists attached to the one underlying
     // HIMAGELIST. If this was not done then you would get an ASSERT in MFC if 
     // you attached more than once to the same system list returned by 
     // SHGetFileInfo at the same time.
     
     SHFILEINFO sfi;
     HIMAGELIST hSystemImageList = 
               (HIMAGELIST) SHGetFileInfo (_T("C:\\"), 0, 
                                           &sfi, sizeof SHFILEINFO,
                                           SHGFI_SYSICONINDEX | 
                                           SHGFI_SMALLICON);
     if ( m_nRefCount == 0 )
     {
          // First time, make a CImageList and attach it to the system image 
          // list.
          m_pImageList = new CImageList;
          VERIFY ( m_pImageList->Attach (hSystemImageList) );
          
          TRACE ("CSysImageList created (0x%x)  ", hSystemImageList);
     }  
     else 
     {
          // Subsequently, we simply point this at the previously newed object.
          m_pImageList = CImageList::FromHandlePermanent (hSystemImageList);
          ASSERT ( m_pImageList );
		TRACE ("CSysImageList from permanent (0x%x)  ", hSystemImageList);
     }
     m_nRefCount++;
     
     TRACE ("RefCount=%d\n", m_nRefCount);
}

CSysImageList::~CSysImageList()
{
     // Decrement the reference count
     m_nRefCount--;

     // Don't let ~CImageList() delete the system image list.
     m_pImageList->Detach();
     
     // Delete the impage list when there are no more references to it.
     if ( m_nRefCount <= 0 )
          delete m_pImageList;

	TRACE ("~CSysImageList() RefCount %d\n", m_nRefCount);
}

