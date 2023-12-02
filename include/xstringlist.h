//
//   XStringList.h     Sortable string list
//
//

#ifndef __MFCX_STRINGLIST_H
#define __MFCX_STRINGLIST_H


class CXStringList : public CStringList
{
public:
	enum SortType { IGNORECASE, CASESENSITIVE };

     void InsertSort (const CString& sNew, SortType type = CASESENSITIVE);

	void CopyFrom (const CStringList& from);

     CStringList& operator= (const CStringList& from)
          { CopyFrom (from);  return *this; }
};

#endif    // __MFCX_STRINGLIST_H
