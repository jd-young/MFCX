//
//   XStringList.cpp     Sortable string list
//
//   Written by John Young, 19-Dec-99
//   Copyright (c) Thistle Software
//
//


#include "stdafx.h"
#include "../include/XStringList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CXStringList::InsertSort (const CString& sNew, 
						 SortType type /*= CASESENSITIVE*/)
{
     POSITION insPos = NULL;
     for (POSITION pos = GetHeadPosition(); pos; )
     {
	     POSITION prev = pos;
          CString s = GetNext (pos);
          if ( (type == CASESENSITIVE && s > sNew) || 
			(type == IGNORECASE && s.CompareNoCase (sNew) > 0) )
		{
			insPos = prev;
			break;
		}
     }

     if ( insPos == NULL )
          AddTail (sNew);
     else InsertBefore (insPos, sNew);     
}

void CXStringList::CopyFrom (const CStringList& from)
{
	RemoveAll();
	for (POSITION pos = from.GetHeadPosition(); pos; )
	{
		CString s = from.GetNext (pos);
		AddTail (s);	
	}
}
