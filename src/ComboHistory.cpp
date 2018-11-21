// ComboHistory.cpp : implementation file
//

#include "stdafx.h"
#include "ComboHistory.h"

#define IDS_SETTING_SELECTITEM _T("sel")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboHistory

CComboHistory::CComboHistory()
{
	m_nMaxMRU=-1;
	m_bAutoSelectOnLoad=TRUE;
}

CComboHistory::~CComboHistory()
{
}


BEGIN_MESSAGE_MAP(CComboHistory, CComboBox)
	//{{AFX_MSG_MAP(CComboHistory)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboHistory message handlers

void CComboHistory::PreSubclassWindow() 
{
	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);
	rc.bottom = rc.top + 150;
	MoveWindow(&rc);

	LoadData();

	CComboBox::PreSubclassWindow();
}

CString CComboHistory::GetKeyName()
{
	int ID = GetDlgCtrlID();
	ASSERT(ID);

	TCHAR szID[6];
	return (CString)_T("MRU\\") + _itot(ID, szID, 10);
}

void CComboHistory::LoadData()
{
	CSettings settings;
	settings.SetRootKeyPath(GetKeyName(), TRUE);

	if (HKEY hk = settings.GetRootKey(KEY_READ))
	{
		DWORD dwIndex=0,dwcbName=MAX_PATH,dwcbValue=MAX_PATH;
		TCHAR szName[MAX_PATH],szValue[MAX_PATH];

		while (ERROR_SUCCESS == 
					RegEnumValue( hk, dwIndex++, szName, &dwcbName, 0, 0
						, (LPBYTE)szValue, &dwcbValue))
		{
			dwcbName=dwcbValue=MAX_PATH;

			if (_tcslen(szValue) && _tcscmp(szName,IDS_SETTING_SELECTITEM))
				AddString(szValue);
		}

		if (m_bAutoSelectOnLoad)
		{
//			DWORD dwSel;
//			if (ERROR_SUCCESS != settings.GetDW(IDS_SETTING_SELECTITEM, &dwSel))
//				dwSel=0;
	
//			SetCurSel(dwSel);
		}

		RegCloseKey(hk);
	}
}

void CComboHistory::SaveData()
{
	CSettings settings;
	
	CString szKeyName = GetKeyName();
	settings.DeleteKey(szKeyName);
	
	settings.SetRootKeyPath(szKeyName, TRUE);

	DWORD dwSel = GetCurSel();
	if (dwSel == CB_ERR)
		dwSel=0;

	settings.Set(IDS_SETTING_SELECTITEM, dwSel);

	CString szItem;
	GetWindowText(szItem);
	if ( szItem.GetLength() && CB_ERR == FindString(-1, szItem))
		settings.Set(_T("00"), szItem);
	
	TCHAR szID[6];
	for (UINT iCur = 0 ; iCur<(UINT)GetCount()
		&&iCur<m_nMaxMRU; iCur++)
	{
		GetLBText(iCur, szItem);

		_stprintf(szID, _T("%.2d"), iCur+1);
		settings.Set(szID, szItem);
	}	
}

void CComboHistory::OnDestroy() 
{
	SaveData();
	CComboBox::OnDestroy();
}

// set to -1 fro no maximum
void CComboHistory::SetMaxMRU(UINT nMax)
{
	m_nMaxMRU = nMax;
}

void CComboHistory::AutoSelectOnLoad(BOOL bTrue)
{
	m_bAutoSelectOnLoad=bTrue;
}
