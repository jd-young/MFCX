//
//   SysImageList.h
//


#ifndef   __MFCX_SYSIMAGELIST_H
#define   __MFCX_SYSIMAGELIST_H

class CSysImageList
{
public:
     CSysImageList();
     ~CSysImageList();
     CImageList* GetImageList() { return m_pImageList; }

private:
     CImageList* m_pImageList;
     static int m_nRefCount;
};


#endif    // __MFCX_SYSIMAGELIST_H
