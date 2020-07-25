//
//	XML.h			XML class
//

#ifndef	__MFXC_XML_H
#define	__MFXC_XML_H


class CXmlParser;

/*!  An XML class.  This class allows you to read and parse an XML 
	stream or file and process it in a consistent way.  It is a C++ class that
	is produced by the CXmlParser Expat library wrapper class 
	(http://expat.sourceforge.com/).
	
	The data read from a file is parsed into an internal tree, and 
	accessed using GetRoot, and GetChild/GetNext.
	
	The following is an example of usage.

\verbatim
	char file[] = "c:\myprojects\myfile.xml";
	CXml xml;
	if ( !xml.ReadFile (file) )
	{
		LOGERR ("Couldn't parse XML file '%s': %s on line %d\n", 
				file, xml.GetLastError(), xml.GetErrorLine());
	}
	
	ProcessJournal (xml.GetRoot());
	
	
	void ProcessJournal (CXml::CElement* pElem)
	{
		if ( pElem == NULL )
			return;
		
		ProcessElem (pElem);
		CXml::CElement* pChild = pElem->GetChild();
		while ( pChild )
		{
			ProcessJournal (pChild);
			pChild = pChild->GetNext();
		}
	}
	
	
	void ProcessElem (CXml::CElement* pElem)
	{
		if ( pElem->GetName() == 'Person' )
			ProcessPerson (pElem);
		else if ( pElem->GetName() == 'Factory' )
			ProcessFactory (pElem);
			:
	}

\endverbatim
*/
class CXml
{
public:
	CXml();		///< Constructs an empty CXml object.
	~CXml();		///< Cleans up and deletes the XML tree memory.


	/**	An XML element.  An element has a name and zero or more 
		attributes, and zero or more children.
		
		Access to parent, child, next and previous siblings is made 
		easy by calling GetParent, GetChild, GetNext, and 
		GetPrevious.  To access attribute values, GetAttribute(), 
		GetIntAttrib, GetBoolAttrib, GetFloatAttrib, and GetStrAttrib 
		are provided.
	*/
	class CElement
	{
	friend CXml;
	friend CXmlParser;

	public:
		CElement();
		~CElement();

		CString GetName() const { return m_sName; }
		CString GetAttribute (const char* pszAttrib) const;
		int     GetIntAttrib (const char* pszAttrib) const;
		bool    GetBoolAttrib (const char* pszAttrib) const;
		double  GetFloatAttrib (const char* pszAttrib) const;
		CString GetStrAttrib (const char* pszAttrib) const
		     { return GetAttribute (pszAttrib); }
		
		bool GetAttributes (CStringArray& arrAttribs) const;

		CElement* GetParent() const { return m_pParent; }
		CElement* GetChild() const { return m_pChild; }
		CElement* GetNext() const { return m_pNext; }
		CElement* GetPrev() const { return m_pPrev; }

		/// Returns the number of child elements.
          int GetChildCount() const;
		//void SetName (const char* pszName) { m_sName = pszName; }

	protected:
		/// Takes a list of NULL terminated key / value pairs and sets the attributes. 
		void SetAttributes (const char** pAttr);

		// Attributes
		CString m_sName;				     /// < The name of the element.
		CString m_sContent;				     ///< The text within the element.
		CMapStringToString m_mapAttributes;     ///< The attributes in the start tag.

		CElement* m_pParent;
		CElement* m_pChild;
		CElement* m_pNext;
		CElement* m_pPrev;
	};

// Attributes
public:
	/// Returns the root element (there is only one), or NULL if the
	/// tree is empty.
     const CElement* GetRoot() const { return m_pRoot; }

	// Not implemented. 
	//const CElement* GetElement (const char*) const;
     
     /// Returns the name of the given element.
	CString GetElementName (CElement* pElement);

	/// Returns the last error message.
     CString GetLastError() const;
     
     /// Returns the line number where the last error occurred.
     int  GetErrorLine() const;

     /// Returns the line number where the last error occurred.
     int  GetErrorColumn() const;

protected:
	CElement* m_pRoot;
	CString m_sError;
	int  m_nErrorLine;
     int  m_nErrorColumn;

// Operations
public:
	void MakeEmpty();

	/// Reads and parses the given (open) file.
	bool ReadFile (CFile* pFile);
	
	/// Reads and parses the given file.
	bool ReadFile (const char* pszFilename);

	/// Writes the internal XML tree data to the given file - not 
	/// implemented
	bool WriteFile (CFile* pFile) const;

	/// Writes the internal XML tree data to the given file - not 
	/// implemented
	bool WriteFile (const char* pszFilename) const;

	/// Parses the given XML data stream.
	bool Parse (const TCHAR* pBuf, long len = -1);

	/// Adds a child element under the given parent.
     CElement* AddChild (CElement* pParent, CElement* pNew);

	/// Helper to replace entities (like &) with their equivalent 
	/// (like &amp;).
     static CString ReplaceEntities (const char* pszText);
};


#endif	// __MFXC_XML_H
