// Settings.cpp: implementation of the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Settings.h"

//#include "reg.h"
#include <tchar.h>

//#define ERRBOX(s) MessageBox(NULL, s, IDS_APPTITLE, MB_ICONSTOP|MB_SETFOREGROUND);
//#define IDS_RUNKEY _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run")
//#define IDS_APPTITLE	_T("SC-KL")
//#define IDS_ENCSTRING _T("")

#define IDS_ROOTKEYPATH _T("Software\\SoftCentral\\SC-DiskInfo")
#define IDS_DISPLAYFORMAT _T("dfmt")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettings::CSettings()
{
	m_szRootKeyPath = IDS_ROOTKEYPATH;
}
/*CSettings::CSettings(CSettings &r)
{
	m_szRootKeyPath = r.m_szRootKeyPath;
}
CSettings& CSettings::operator=(CSettings &r)
{
	if (&r != this)
	{
		m_szRootKeyPath = r.m_szRootKeyPath;
	}
	return *this;
}
*/
CSettings::~CSettings()
{

}

long CSettings::DeleteKey(LPCTSTR szName)
{
	long lRet=ERROR_PATH_NOT_FOUND;
	HKEY hKey = GetRootKey(KEY_WRITE);
	
	if (hKey)
	{
		lRet = RegDeleteKey(hKey, szName);
		RegCloseKey(hKey);
	}
	return lRet;
}

void CSettings::Set(LPCTSTR szValueName, LPCTSTR szValue)
{
	HKEY hKey = GetRootKey(KEY_WRITE);
	if (hKey)
	{
		RegSetValueEx(hKey, szValueName, 0, REG_SZ, (LPBYTE)szValue, (_tcslen(szValue)+1)*sizeof(TCHAR));
		RegCloseKey(hKey);
	}
}

void CSettings::Set(LPCTSTR szValueName, DWORD dwValue)
{
	HKEY hKey = GetRootKey(KEY_WRITE);
	if (hKey)
	{
		RegSetValueEx(hKey, szValueName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(dwValue));
		RegCloseKey(hKey);
	}
}

BOOL CSettings::Set(LPCTSTR szValueName, BYTE* pData, DWORD dwcbData, const HKEY hKey )
{
	HKEY hRKey = (hKey) ? hKey : GetRootKey(KEY_WRITE);
	BOOL bRet=FALSE;
	if (hRKey)
	{
		bRet =(ERROR_SUCCESS == 
			RegSetValueEx(hRKey, szValueName, 0, REG_BINARY, pData, dwcbData));
		if (hRKey != hKey)
			RegCloseKey(hRKey);
	}
	return bRet;
}

long CSettings::GetS(LPCTSTR szValueName, LPTSTR szValue, LPDWORD pdwSize)
{
	HKEY hKey = GetRootKey(KEY_READ);
	long lRet = ERROR_PATH_NOT_FOUND;

	if (hKey)
	{
		DWORD dwType;
		lRet = RegQueryValueEx(hKey, szValueName, 0, &dwType, (LPBYTE)szValue, pdwSize);
		RegCloseKey(hKey);
	}
	return lRet;
}

long CSettings::GetS(LPCTSTR szValueName, CString &szValue)
{
	HKEY hKey = GetRootKey(KEY_READ);
	long lRet = ERROR_PATH_NOT_FOUND;

	if (hKey)
	{
		DWORD dwType,dwSize=0;

		if (0 == 
			(lRet = RegQueryValueEx(hKey, szValueName, 0, &dwType
						, (LPBYTE)0, &dwSize)))
		{	
			lRet = RegQueryValueEx(hKey, szValueName, 0, &dwType
						, (LPBYTE)szValue.GetBufferSetLength(dwSize+1)
						, &dwSize);

			szValue.ReleaseBuffer();
		}

		RegCloseKey(hKey);
		//int iBufferSize = 128;
		//TCHAR sz = (TCHAR*) malloc(128*sizeof(TCHAR));
	}
	return lRet;
}
long CSettings::GetDW(LPCTSTR szValueName, LPDWORD pdw)
{
	HKEY hKey = GetRootKey(KEY_READ);
	long lRet = ERROR_PATH_NOT_FOUND;

	if (hKey)
	{
		DWORD dwType, dwSize=sizeof(DWORD);
		lRet = RegQueryValueEx(hKey, szValueName, 0, &dwType, (LPBYTE)pdw, &dwSize);
		RegCloseKey(hKey);
	}
	return lRet;
}

// returns -1 on failure
long CSettings::GetB(LPCTSTR szValueName, LPBYTE pData, DWORD* pdwcb)
{
	HKEY hKey = GetRootKey(KEY_READ);
	long lRet = ERROR_PATH_NOT_FOUND;

	if (hKey)
	{
		DWORD dwType;
		lRet = RegQueryValueEx(hKey, szValueName, 0, &dwType, pData, pdwcb);
		RegCloseKey(hKey);
		
	}
	return lRet;
}

void CSettings::SetRootKeyPath(LPCTSTR szPath, BOOL bIsSubDirectory)
{
	m_szRootKeyPath;
	if (bIsSubDirectory)
		m_szRootKeyPath = (CString)IDS_ROOTKEYPATH + _T("\\");
	m_szRootKeyPath += szPath;	
}


HKEY CSettings::GetRootKey(REGSAM Sam)
{
 	HKEY hKey=0;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_szRootKeyPath
		, 0, Sam, &hKey) )
		RegCreateFullKeyPath(HKEY_LOCAL_MACHINE, m_szRootKeyPath, Sam, &hKey);
	
	return hKey;
}

long CSettings::RegCreateFullKeyPath(const HKEY hRootKey
	, LPCTSTR szPath, REGSAM samDesired, HKEY* phKeyResult)
{
	HKEY hCurKey = hRootKey;
	LPTSTR szCurSubKey = new TCHAR[_tcslen(szPath)+sizeof(TCHAR)];
	LPCTSTR pszCur=szPath;
	long lRet=0L;
	int iPos;
	DWORD dwDisp;
	while (pszCur < szPath+_tcslen(szPath) )
	{
		iPos = 0;

		// get next subkey
		while(*pszCur != '\\'&&*pszCur!=_TINT('\0'))
		{
			szCurSubKey[iPos++] = *pszCur;
			pszCur = _tcsinc(pszCur);
		}
		szCurSubKey[iPos] = _TINT('\0');
		
		lRet = RegCreateKeyEx(hCurKey, szCurSubKey, 0
			, NULL, NULL
			, samDesired
			, NULL, phKeyResult
			, &dwDisp);
		RegCloseKey(hCurKey);

		if (ERROR_SUCCESS != lRet)
		{
			*phKeyResult=0;
			break;
		}

		hCurKey = *phKeyResult;

		pszCur = _tcsinc(pszCur);
	}

	delete[] szCurSubKey;
	return lRet;
}


/*
void CSettings::EnableAutoStart(BOOL bEnable)
{
	HKEY hKey;
	if (ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE
		, IDS_RUNKEY
		, 0
		, KEY_WRITE
		, &hKey))
	{
		if (!bEnable)
			RegDeleteValue(hKey, IDS_APPTITLE);
		else
		{
			char szPath[MAX_PATH];
			if (GetModuleFileName(NULL, szPath, MAX_PATH))
			{
				strcat(szPath, " -s");
				if (ERROR_SUCCESS != RegSetValueEx(hKey, IDS_APPTITLE, 0
					, REG_SZ, (LPBYTE)szPath, strlen(szPath)))
					ERRBOX("Unable to set autostart feature - Unable to write to registry");
			}
			else
				ERRBOX("Unable to set autostart feature - File error");
		}
		RegCloseKey(hKey);
	}
	else
		ERRBOX("Unable to enable autostart feature - Registry key could not be opened");

}

BOOL CSettings::IsAutoStartEnabled()
{
	BOOL bEnabled=FALSE;
	HKEY hKey;

	// check if autostart is enabled
	if (ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE
		, IDS_RUNKEY
		, 0
		, KEY_READ
		, &hKey))
	{
		TCHAR szBuf[MAX_PATH];
		DWORD dwcb = MAX_PATH, dwType;
		bEnabled = (ERROR_SUCCESS==RegQueryValueEx(hKey, IDS_APPTITLE, 0
			, &dwType, (LPBYTE)szBuf, &dwcb));
		RegCloseKey(hKey);
	}
	return bEnabled;

}
*/
/*
void CSettings::SetEncrypted(LPCTSTR szValueName, LPCTSTR szValue)
{
	long lcb = (_tcslen(szValue)+1)*sizeof(TCHAR);
	TCHAR* pszEncVal = new TCHAR[lcb];
	_tcscpy(pszEncVal, szValue);
	
	TCHAR* pszEncValStart= pszEncVal;

	TCHAR szEncSzStart[MAX_PATH];
	TCHAR* pszEncSz=szEncSzStart;
	GetEncryptionHash(szEncSzStart);

	while (*pszEncVal)
	{
		if (0 == *pszEncSz)
			pszEncSz = szEncSzStart;

		*pszEncVal ^= *pszEncSz;

		pszEncVal = _tcsinc(pszEncVal);
		pszEncSz = _tcsinc(pszEncSz);
	}
	Set(szValueName,(LPBYTE) pszEncValStart, lcb);
	delete[] pszEncValStart;
}

long CSettings::GetEncryptedS(LPCTSTR szValueName, LPTSTR szValue)
{
	DWORD dwSize=MAX_PATH*sizeof(TCHAR);
	long lRet = GetB(szValueName, (LPBYTE)szValue, &dwSize);
	if (ERROR_SUCCESS == lRet)
	{
		szValue[dwSize]=0;

		TCHAR* pszDecVal = szValue;
		
		TCHAR szEncSzStart[MAX_PATH];
		TCHAR* pszEncSz=szEncSzStart;
		GetEncryptionHash(szEncSzStart);

		while (*pszDecVal)
		{
			if (0 == *pszEncSz)
			pszEncSz = szEncSzStart;

			*pszDecVal ^= *pszEncSz;
	
			pszEncSz = _tcsinc(pszEncSz);
			pszDecVal = _tcsinc(pszDecVal);
		}
	}
	return lRet;
}
void CSettings::GetEncryptionHash(LPTSTR sz)
{
	LPTSTR psz=sz;
	for (int j = 4 ; j < 28 ; j += 4)
		*(psz++) = (char)j;

	for (int i = 'z'+1 ; i < 255 ; i+=3)
		*(psz++) = (char)i;

	*psz=0;
}
*/




void CSettings::SetDisplayFormat(DWORD nBKBMBGB)
{
	Set(IDS_DISPLAYFORMAT, nBKBMBGB);
}

DWORD CSettings::GetDisplayFormat()
{
	DWORD dwDFmt=20;// MB
	GetDW(IDS_DISPLAYFORMAT, &dwDFmt);
	return dwDFmt;
}

CString CSettings::DisplayFormatToText(DWORD nBKBMBGB)
{
	CString szDFmt;
	switch (nBKBMBGB)
	{
		case 10:	
			szDFmt = _T("KB");
			break;

		case 20:	
			szDFmt = _T("MB");
			break;

		case 30:	
			szDFmt = _T("GB");
			break;

		default:
				ASSERT(FALSE);
	}
	
	return szDFmt;
}
