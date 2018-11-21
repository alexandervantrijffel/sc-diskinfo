// Settings.h: interface for the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGS_H__36949CB0_4783_45E6_BE1C_A9273CDED155__INCLUDED_)
#define AFX_SETTINGS_H__36949CB0_4783_45E6_BE1C_A9273CDED155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IDS_SETTING_CUSTOMSTEALTHNAME _T("CustomStealthName")
#define IDS_SETTING_STEALTHNAME _T("StealthName")

class CSettings  
{
public:
	CString DisplayFormatToText(DWORD nBKBMBGB);
	DWORD GetDisplayFormat();
	void SetDisplayFormat(DWORD nBKBMBGB);
	long DeleteKey(LPCTSTR szName);
	CSettings();
//	CSettings(CSettings &r);
//	CSettings& CSettings::operator=(CSettings &r);
	virtual ~CSettings();

	void SetRootKeyPath(LPCTSTR szPath, BOOL bIsSubDirectory);
	long GetS(LPCTSTR szValueName, CString& szValue);
/*	long GetEncryptedS(LPCTSTR szValueName, LPTSTR szValue);
	void SetEncrypted(LPCTSTR szValueName, LPCTSTR szValue);
	BOOL IsAutoStartEnabled();
	void  EnableAutoStart(BOOL bEnable);
*/	long  GetDW(LPCTSTR szValueName, LPDWORD pdw);
	long  GetB(LPCTSTR szValueName, LPBYTE pData, DWORD* pdwcb);
	long  GetS(LPCTSTR szValueName, LPTSTR szValue, LPDWORD pdwSize);
	void  Set(LPCTSTR szValueName, DWORD dwValue);
	void  Set(LPCTSTR szValueName, LPCTSTR szValue);
	BOOL  Set(LPCTSTR szValueName, BYTE* pData, DWORD dwcbData, const HKEY=0);
	HKEY  GetRootKey(REGSAM Sam);

protected:
	//void GetEncryptionHash(LPTSTR sz);
	long RegCreateFullKeyPath(const HKEY hRootKey, LPCTSTR szPath, REGSAM samDesired, HKEY* phKeyResult);
	CString m_szRootKeyPath;
};

#endif // !defined(AFX_SETTINGS_H__36949CB0_4783_45E6_BE1C_A9273CDED155__INCLUDED_)
