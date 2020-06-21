//
//   XStringList.h     Sortable string list
//
//

#ifndef _MFCX_STRINGLIST_H
#define _MFCX_STRINGLIST_H


class CXStringList : public CStringList
{
public:
	enum SortType { IGNORECASE, CASESENSITIVE };

     void InsertSort (const CString& sNew, SortType type = CASESENSITIVE);

	void CopyFrom (const CStringList& from);

     CStringList& operator= (const CStringList& from)
          { CopyFrom (from);  return *this; }
};

#endif    // _MFCX_STRINGLIST_H
