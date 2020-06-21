//	TArray.h        Typed pointer array template

#ifndef	__MFCX_TARRAY_H
#define	__MFCX_TARRAY_H


template<class TYPE>
class TArray : public CPtrArray
{
public:
     TArray() {}
     ~TArray() { MakeEmpty(); }

	// Accessing elements
	TYPE GetAt(int nIndex) const
		{ return (TYPE)CPtrArray::GetAt(nIndex); }
	TYPE& ElementAt(int nIndex)
		{ return (TYPE&)CPtrArray::ElementAt(nIndex); }
	void SetAt(int nIndex, TYPE ptr)
		{ CPtrArray::SetAt(nIndex, ptr); }

	// Potentially growing the array
	void SetAtGrow(int nIndex, TYPE newElement)
	   { CPtrArray::SetAtGrow(nIndex, newElement); }
	int Add(TYPE newElement)
	   { return CPtrArray::Add(newElement); }
	int Append(const TArray<TYPE>& src)
	   { return CPtrArray::Append(src); }
	void Copy(const TArray<TYPE>& src)
		{ CPtrArray::Copy(src); }

	// Operations that move elements around
	void InsertAt(int nIndex, TYPE newElement, int nCount = 1)
		{ CPtrArray::InsertAt(nIndex, newElement, nCount); }
	void InsertAt(int nStartIndex, TArray<TYPE>* pNewArray)
	   { CPtrArray::InsertAt(nStartIndex, pNewArray); }

	// overloaded operator helpers
	TYPE operator[](int nIndex) const
		{ return (TYPE)CPtrArray::operator[](nIndex); }
	TYPE& operator[](int nIndex)
		{ return (TYPE&)CPtrArray::operator[](nIndex); }
	
	void MakeEmpty()
	     {
	          for (int i = 0; i < GetSize(); i++)
	               delete (TYPE) GetAt (i);
	          RemoveAll();
	     }
};

#endif    // __MFCX_TARRAY_H
