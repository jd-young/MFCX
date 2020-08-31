//
//	VerInfo.h		CVersionInfo class
//


#ifndef	__MFCX_VERINFO_H
#define	__MFCX_VERINFO_H

/**
 * Extracts version inforamtion from an application's 
 */
class CVersionInfo
{
public:
     CVersionInfo();     

     int  GetMajorVer() { return m_nMajorVer; }   ///< Returns the first number in PRODUCTVERSION
     int  GetMinorVer() { return m_nMinorVer; }   ///< Returns the second number in PRODUCTVERSION
     int  GetRevision() { return m_nRevision; }   ///< Returns the third number in PRODUCTVERSION
     int  GetBuildNr() { return m_nBuildNr; }     ///< Returns the fourth number in PRODUCTVERSION

     const TCHAR* GetName() { return m_sName; }     ///< Returns "InternalName"

     const TCHAR* GetDescription() { return m_sDescription; }///< Returns "ProductName"
     const TCHAR* GetAuthor() { return m_sAuthor; }          ///< Returns "Author"     
     const TCHAR* GetVersion() { return m_sVersion; }        ///< Returns M.m or M.m.r if there is a revision number.
     const TCHAR* GetCopyright() { return m_sCopyright; }    ///< Returns "LegalCopyright"
     const TCHAR* GetBetaBuild() { return m_sBetaBuild; }    ///< Returns "BetaBuild"
     const TCHAR* GetBuildDate() { return m_sBuildDate; }    ///< Returns "BuildDate"

protected:
     /// Helper to extract the four digits from the PRODUCTVERSION.
     static BOOL ExtractVersion (const TCHAR* s, int* nMaj, int* nMin, 
									    int* nRev, int* nBuild);
     BOOL ExtractVersion (const TCHAR* s);

private:
     CString m_sName;
     CString m_sDescription;
     CString m_sAuthor;
     CString m_sVersion;
     CString m_sCopyright;
     CString m_sBetaBuild;
     CString m_sBuildDate;

     int m_nMajorVer;
     int m_nMinorVer;
     int m_nRevision;
     int m_nBuildNr;
};

#endif	// __MFCX_VERINFO_H
