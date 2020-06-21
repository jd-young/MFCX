//   TMap.h


#ifndef   __MFCX_TMAP_H
#define   __MFCX_TMAP_H


template<class VALUE>
class TStringMap : public CMapStringToPtr
{
public:

// Construction
	TStringMap (int nBlockSize = 10)
		: CMapStringToPtr (nBlockSize) { }
	
	~TStringMap() { MakeEmpty(); }

	// Lookup
	BOOL Lookup (const TCHAR* key, VALUE& rValue) const
		{ return CMapStringToPtr::Lookup (key, (void*&) rValue); }

	// Lookup and add if not there
	VALUE& operator[](const TCHAR* key)
		{ return (VALUE&) CMapStringToPtr::operator[](key); }

	// add a new key (key, value) pair
	void SetAt (const TCHAR* key, VALUE newValue)
		{ CMapStringToPtr::SetAt (key, newValue); }

	// removing existing (key, ?) pair
	BOOL RemoveKey (const CString& key)
		{ return CMapStringToPtr::RemoveKey(key); }

	// iteration
	void GetNextAssoc(POSITION& rPosition, CString& rKey, VALUE& rValue) const
		{ CMapStringToPtr::GetNextAssoc (rPosition, rKey, (void*&) rValue); }
	
	void MakeEmpty()
	{
	     for (POSITION pos = GetStartPosition(); pos; )
	     {
	          CString key;
	          VALUE value;
	          GetNextAssoc (pos, key, value);
	          delete (VALUE) value;
	     }
		RemoveAll();
	}
};




template<class VALUE>
class TIntMap : public CMapPtrToPtr
{
public:

// Construction
	TIntMap (int nBlockSize = 10)
		: CMapPtrToPtr (nBlockSize) { }
	
	~TIntMap() { MakeEmpty(); }

	// Lookup
	BOOL Lookup (int key, VALUE& rValue) const
		{ return CMapPtrToPtr::Lookup ((void*) key, (void*&) rValue); }

	// Lookup and add if not there
	VALUE& operator[](int key)
		{ return (VALUE&) CMapPtrToPtr::operator[]((void*) key); }

	// add a new key (key, value) pair
	void SetAt (int key, VALUE newValue)
		{ CMapPtrToPtr::SetAt ((void*) key, (void*&) newValue); }

	// removing existing (key, ?) pair
	BOOL RemoveKey (int key)
		{ return CMapPtrToPtr::RemoveKey (key); }

	// iteration
	void GetNextAssoc (POSITION& rPosition, int& rKey, VALUE& rValue) const
		{ CMapPtrToPtr::GetNextAssoc (rPosition, (void*&) rKey, (void*&) rValue); }
	
	void MakeEmpty()
	{
	     for (POSITION pos = GetStartPosition(); pos; )
	     {
	          int key;
	          VALUE value;
	          GetNextAssoc (pos, key, value);
	          delete (VALUE) value;
	     }
	}
};



template<class KEY, class VALUE>
class TMap : public CMapPtrToPtr
{
public:

// Construction
	TMap (int nBlockSize = 10)
		: CMapPtrToPtr (nBlockSize) { }
	
	~TMap() { MakeEmpty(); }

	// Lookup
	BOOL Lookup (KEY key, VALUE& rValue) const
		{ return CMapPtrToPtr::Lookup ((void*) key, (void*&) rValue); }

	// Lookup and add if not there
	VALUE& operator[](KEY key)
		{ return (VALUE&) CMapPtrToPtr::operator[]((void*) key); }

	// add a new key (key, value) pair
	void SetAt (KEY key, VALUE newValue)
		{ CMapPtrToPtr::SetAt (key, newValue); }


	// removing existing (key, ?) pair
	BOOL RemoveKey (KEY key)
		{ return CMapPtrToPtr::RemoveKey (key); }

	// iteration
	void GetNextAssoc (POSITION& rPosition, KEY& rKey, VALUE& rValue) const
		{ CMapPtrToPtr::GetNextAssoc (rPosition, (void*&) rKey, (void*&) rValue); }
	
	void MakeEmpty()
	{
	     for (POSITION pos = GetStartPosition(); pos; )
	     {
	          KEY key;
	          VALUE value;
	          GetNextAssoc (pos, key, value);
	          delete (VALUE) value;
	     }
	}
};



#endif    // __MFCX_TMAP_H
