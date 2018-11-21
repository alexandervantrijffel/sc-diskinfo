#include "global.h"
#include "stdafx.h"

TCHAR* InsertDots(LPTSTR szNumber)
{	
	UINT iCount=0, iRealCount = 1;
	TCHAR szDotted[MAX_PATH];
	TCHAR* pszCurrent = szNumber + _tcslen(szNumber) - 1;
	memset(szDotted, 0, MAX_PATH);
	
	while (pszCurrent >= szNumber)	
	{		
		szDotted[iCount] = *pszCurrent;

		if ( (iRealCount++ % 3) == NULL && pszCurrent != szNumber)
			szDotted[++iCount] = _TINT('.');
		
		if (*pszCurrent==_TINT('.')||*pszCurrent==_TINT(','))
		{	iCount--;
			iRealCount--;
		}

		pszCurrent--;
		iCount++;
	}
	_tcscpy(szNumber, _tcsrev(szDotted) );
	return szNumber;
}