#if !defined(AFX_COMBOHISTORY_H__054307EE_9EAA_401C_A6BF_012B7B259B24__INCLUDED_)
#define AFX_COMBOHISTORY_H__054307EE_9EAA_401C_A6BF_012B7B259B24__INCLUDED_

#include "Settings.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboHistory window

class CComboHistory : public CComboBox
{
// Construction
public:
	CComboHistory();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboHistory)
	public:
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void AutoSelectOnLoad(BOOL bTrue);
	void SetMaxMRU(UINT nMax);
	void SaveData();
	virtual ~CComboHistory();
	
	
	// Generated message map functions
protected:
	CString GetKeyName();
	void LoadData();

	UINT m_nMaxMRU;
	BOOL m_bAutoSelectOnLoad;


	//{{AFX_MSG(CComboHistory)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOHISTORY_H__054307EE_9EAA_401C_A6BF_012B7B259B24__INCLUDED_)
