/**
	\file	XML.cpp			An XML class.

	\author	John Young
	\date	2000

	Copyright (c) 2000 Thistle Software
	
	This class wraps the expat XML parser written by James Clark
	(http://www.jclark.com/xml/expat.html).
*/

#include "stdafx.h"
#include "../include/xml.h"
#include <expat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**  An XML parser class that wraps the expat library 
     (http://expat.sourceforge.com/).  It uses the CXml companion class that 
     wraps XML.
*/
class CXmlParser
{
public:
     /// Constructs an XML parser.
	CXmlParser (CXml& xml);

     /// Cleans up the parser.
	~CXmlParser();

     /// Parses the given data.
	bool Parse (const char* pData, int len = -1);

     /// Gets the error description of the last error that occurred.
	const CString& GetErrorStr() const { return m_sError; }

     /// Gets the line number in which the last error occurred.
	int   GetErrorLine() const { return m_nErrorLine; }

     /// Gets the column number in which the last error occurred.
	int   GetErrorColumn() const { return m_nErrorColumn; }

protected:
	CXml&      m_xml;	
	XML_Parser m_xmlParser;
	CXml::CElement* m_pCurElement;

	static void OnStartElement (void* pUser, const XML_Char* pszName, const XML_Char** pAttr);
	static void OnEndElement (void *pUser, const XML_Char *pszName);
	static void OnCharData (void* pUser, const XML_Char* pszText, int len);

	void OnStartElement (const XML_Char* pszName, const XML_Char** pAttr);
	void OnEndElement (const XML_Char *pszName);
	void OnCharData (const XML_Char* pszText, int len);

// Error processing
	CString    m_sError;
	int 	      m_nErrorLine;
	int 		 m_nErrorColumn;
};



CXml::CXml()
{
	m_pRoot = NULL;
	m_nErrorLine = -1;
}

CXml::~CXml()
{
	MakeEmpty();
}

void CXml::MakeEmpty()
{
	delete m_pRoot;
	m_pRoot = NULL;
}

bool CXml::ReadFile (const char* pszFilename)
{
	CFile file;
	if ( file.Open (pszFilename, CFile::modeRead | CFile::shareDenyNone) )
		return ReadFile (&file);
	return false;
}

bool CXml::ReadFile (CFile* pFile)
{
	CFileStatus status;
	if ( pFile->GetStatus (status) )
	{
		CString sBuf;
		TCHAR* s = sBuf.GetBuffer (status.m_size);
		if ( pFile->Read (s, status.m_size) )
		{
			MakeEmpty();
			return Parse (s, status.m_size);
		}
	}
	return false;
}

bool CXml::WriteFile (const char* pszFilename) const
{
     ASSERT ( false );
	return false;
}

bool CXml::WriteFile (CFile* pFile) const
{
     ASSERT ( false );
	return false;
}



bool CXml::Parse (const TCHAR* pBuf, long len /*= -1*/)
{
	CXmlParser xmlParser (*this);
	if ( xmlParser.Parse (pBuf, len) )
	     return true;
	else
	{
	     m_sError = xmlParser.GetErrorStr();
	     m_nErrorLine = xmlParser.GetErrorLine();
          m_nErrorColumn = xmlParser.GetErrorColumn();
     	return false;
	}
}


CXml::CElement* CXml::AddChild (CXml::CElement* pParent, CXml::CElement* pNew)
{
	CElement* pLast = pParent ? pParent->m_pChild : NULL;
	if ( pLast )
	{
		while ( pLast->m_pNext )
			pLast = pLast->m_pNext;
		pLast->m_pNext = pNew;
		pNew->m_pPrev = pLast;
	}
	else 
		if ( pParent )
			pParent->m_pChild = pNew;
		else m_pRoot = pNew;
	pNew->m_pParent = pParent;
	return pNew;
}

/*static*/ CString CXml::ReplaceEntities (const char* pszText)
{
	CString sText = pszText;
	
	sText.Replace ("&", "&amp;");
	sText.Replace ("<", "&lt;");
	sText.Replace (">", "&gt;");
	sText.Replace ("'", "&apos;");
	sText.Replace ("\"", "&quot;");
	
	return sText;
}


CString CXml::GetLastError() const
{
     return m_sError;
}


int  CXml::GetErrorLine() const
{
     return m_nErrorLine;
}



CXml::CElement::CElement()
{
	m_pParent = 
	m_pChild = 
	m_pNext = 
	m_pPrev = NULL;
}

CXml::CElement::~CElement()
{
	// Delete all the children
	// TODO: Does this delete all the children?
	while ( m_pChild )
		delete m_pChild;
	
	if ( m_pNext )
		m_pNext->m_pPrev = m_pPrev;
	
	if ( m_pPrev )
		m_pPrev->m_pNext = m_pNext;
	else if ( m_pParent )
		m_pParent->m_pChild = m_pNext;
}

/*!	Sets the attributes that were found in an element start tag.

	Attributes are key-value pairs, and are passed in as a vector of character
	pointer pairs (key followed by value).  The pairs are terminated by a null
	pointer.

\param	pAttrs

*/
void CXml::CElement::SetAttributes (const char** pAttrs)
{
	CString sKey, sValue;
	while ( *pAttrs )
	{
		sKey = *pAttrs++;
		ASSERT ( *pAttrs );
		sValue = *pAttrs ? *pAttrs++ : "";

		CString sDummy;
		if ( !m_mapAttributes.Lookup (sKey, sDummy) )
			m_mapAttributes.SetAt (sKey, sValue);
	}
}


CString CXml::CElement::GetAttribute (const char* pszAttrib) const
{
     CString sValue;
     if ( m_mapAttributes.Lookup (pszAttrib, sValue) )
          return sValue;
     return "";
}

/*!  Returns the integer value of the given attribute.  

     Integers follow the C/C++ convention of leading '0' for octals, leading 
     '0x' for hexidecimal, otherwise it is an integer.

\param    pszAttrib      The attribute to read.
\return   The integer value.  If the attribute is not present, then zero is 
          returned.
*/
int CXml::CElement::GetIntAttrib (const char* pszAttrib) const
{
     CString sValue = GetAttribute (pszAttrib);
     char* psz = NULL;
     return strtoul (sValue, &psz, 0);     
}


bool CXml::CElement::GetBoolAttrib (const char* pszAttrib) const
{
     CString sValue = GetAttribute (pszAttrib);
     sValue.MakeLower();
     return sValue == "yes" || sValue == "true";
}


double CXml::CElement::GetFloatAttrib (const char* pszAttrib) const
{
     ASSERT (false );
     return 0.0;
}



bool CXml::CElement::GetAttributes (CStringArray& arrAttribs) const
{
     arrAttribs.RemoveAll();
     for (POSITION pos = m_mapAttributes.GetStartPosition(); pos; )
     {
          CString sAttrib;
          CString sValue;
          m_mapAttributes.GetNextAssoc (pos, sAttrib, sValue);
          arrAttribs.Add (sAttrib);
     }
     return true;
}



////////////////////////////////////////////////////////////////////////////////

CXmlParser::CXmlParser (CXml& xml) 
 : 	m_xml (xml)
{
	m_pCurElement = NULL;
	m_nErrorLine = -1;
	m_nErrorColumn = -1;

	m_xmlParser = XML_ParserCreate (NULL);

	if ( m_xmlParser )
	{	
		XML_SetUserData (m_xmlParser, this);
		XML_SetElementHandler (m_xmlParser, OnStartElement, OnEndElement);
		XML_SetCharacterDataHandler (m_xmlParser, OnCharData);
	}
}

CXmlParser::~CXmlParser()
{
	XML_ParserFree (m_xmlParser);
	m_xmlParser = NULL;
}


bool CXmlParser::Parse (const char* pData, int len)
{
	if ( len == -1 )
		len = strlen (pData);
	if ( XML_Parse (m_xmlParser, pData, len, 1) == XML_STATUS_ERROR ) 
	{
		m_sError = XML_ErrorString (XML_GetErrorCode (m_xmlParser));
		m_nErrorLine = XML_GetCurrentLineNumber (m_xmlParser);
          m_nErrorColumn = XML_GetCurrentColumnNumber (m_xmlParser);
		return false;
	}
	return true;
}

/*static*/ void CXmlParser::OnStartElement (void* pUser, const XML_Char* pszName, const XML_Char** pAttr)
{
	CXmlParser* pThis = (CXmlParser*) pUser;
	pThis->OnStartElement (pszName, pAttr);
}

void CXmlParser::OnStartElement (const XML_Char* pszName, const XML_Char** pAttr)
{
	m_pCurElement = m_xml.AddChild (m_pCurElement, new CXml::CElement);
	m_pCurElement->m_sName = pszName;
	m_pCurElement->SetAttributes (pAttr);
}


/*static*/ void CXmlParser::OnEndElement (void *pUser, const XML_Char *pszName)
{
	CXmlParser* pThis = (CXmlParser*) pUser;
	pThis->OnEndElement (pszName);
}

void CXmlParser::OnEndElement (const XML_Char *pszName)
{
	ASSERT ( m_pCurElement );
	m_pCurElement = m_pCurElement->GetParent();
}


/*static*/ void CXmlParser::OnCharData (void* pUser, const XML_Char* pszText, int len)
{
	CXmlParser* pThis = (CXmlParser*) pUser;
	pThis->OnCharData (pszText, len);
}

void CXmlParser::OnCharData (const XML_Char* pszText, int len)
{
     ASSERT ( m_pCurElement );
	m_pCurElement->m_sContent += CString (pszText, len);
}

