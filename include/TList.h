//	TList.h        Typed pointer list template

#ifndef	__MFCX_TLIST_H
#define	__MFCX_TLIST_H

/// Typed pointer list template that cleans up after itself
template <class TYPE> 
class TList : public CPtrList
{
public:
     /// Constructor
	TList (int nBlockSize = 10) : CPtrList (nBlockSize) {}
	
	/// Destructor that will delete all the pointer objects in the list.
	~TList() { MakeEmpty(); }

	///  Gets a reference to the head (without removing it)
	TYPE& GetHead()		{ return (TYPE&)CPtrList::GetHead(); }
	
	/// Copies the head object and returns it.
	TYPE  GetHead() const	{ return (TYPE)CPtrList::GetHead(); }

     /// Gets a reference to the tail object.
	TYPE& GetTail()		{ return (TYPE&)CPtrList::GetTail(); }

     /// Copies the object from the tail and returns it.
	TYPE  GetTail() const	{ return (TYPE)CPtrList::GetTail(); }

	///  Removes the head from the list.  (Don't call on an empty list!).
	TYPE RemoveHead()		{ return (TYPE)CPtrList::RemoveHead(); }

     /// 	Removes the tail from the list.  (Don't call on empty list!).
	TYPE RemoveTail()		{ return (TYPE)CPtrList::RemoveTail(); }

	// iteration
	TYPE& GetNext (POSITION& pos)		     { return (TYPE&)CPtrList::GetNext(pos); }
	TYPE  GetNext (POSITION& pos) const	{ return (TYPE)CPtrList::GetNext(pos); }
	TYPE& GetPrev (POSITION& pos)		     { return (TYPE&)CPtrList::GetPrev(pos); }
	TYPE  GetPrev (POSITION& pos) const	{ return (TYPE)CPtrList::GetPrev(pos); }

	// getting/modifying an element at a given position
	TYPE& GetAt (POSITION position)		{ return (TYPE&)CPtrList::GetAt(position); }
	TYPE  GetAt (POSITION position) const	{ return (TYPE)CPtrList::GetAt(position); }
	void  SetAt (POSITION pos, TYPE newElement){ CPtrList::SetAt(pos, newElement); }

	POSITION AddHead (TYPE newElement)		{ return CPtrList::AddHead(newElement); }
	POSITION AddTail (TYPE newElement)		{ return CPtrList::AddTail(newElement); }

	// add another list of elements before head or after tail
	void AddHead (TList<TYPE>* pNewList)	{ CPtrList::AddHead(pNewList); }
	void AddTail (TList<TYPE>* pNewList)	{ CPtrList::AddTail(pNewList); }

	void MakeEmpty()
	{
		while ( !IsEmpty() )
			delete (TYPE) CPtrList::RemoveHead();
	}
};

#endif	// __MFCX_TLIST_H
