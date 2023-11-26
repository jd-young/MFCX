//
//   ToolsMgr.h
//

#ifndef   ENVVARS_H
#define   ENVVARS_H

#include <map>
#include <string>

namespace MFCX {

using std::map;
using std::string;

/**  A class to manage evironment variables.  This is used by the CToolThread 
     class.

*/
class CEnvVars
{
public:
	CEnvVars();
	CEnvVars (const map<string, string>& from);
	CEnvVars (const CEnvVars& from);
     CEnvVars& operator= (const CEnvVars& from);
	
     const map<string, string>& GetStringMap() const { return _mapEnvStrs; }

//     void SetEnvs (const map<string, string>& from);

	/// Creates an environment block suitable for CreateProcess().
	TCHAR* GetEnvBlock() const;
	
	/// Creates an environment block suitable for CreateProcess() including
	/// process environment variables.
	TCHAR* GetProcEnvBlock() const;

private:
     /// A map of environment strings.
     map<string, string> _mapEnvStrs;

protected:
     TCHAR* MergeEnvBlock (TCHAR* pOther) const;
     TCHAR* GetEnvBlock (const map<string, string>& other) const;

     static string ExpandMacros (const string& sText, 
                                 const map<string, string>& macros, 
                                 const map<string, string>& lcKeys,
                                 const TCHAR* pszDelim);
     static string ExpandMacros (const string& sText, 
                                 const map<string, string>& macros, 
                                 const map<string, string>& lcKeys,
                                 const TCHAR* pszStartDelim, 
                                 const TCHAR* pszEndDelim);

     /// Creates an environment block from the given variables map.
     static TCHAR* CreateEnvBlock (map<string, string>& vars);
};

}    // namespace MFCX

#endif    //   ENVVARS_H
