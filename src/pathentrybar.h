#if !defined(AFX_PATHSELECTBAR_H__60B8ADE3_5784_471C_B426_BE9AA9E4D1E3__INCLUDED_)
#define AFX_PATHSELECTBAR_H__60B8ADE3_5784_471C_B426_BE9AA9E4D1E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathSelectBar.h : header file
//

#include "combohistory.h"
/////////////////////////////////////////////////////////////////////////////
// CPathSelectBar dialog

class CPathEntryBar : public CDialogBar
{
// Construction
public:
	CPathEntryBar();   // standard constructor
	~CPathEntryBar();
// Dialog Data
	//{{AFX_DATA(CPathSelectBar)
	enum { IDD = IDD_PATHENTRY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathSelectBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnComboSelChange();
	CComboHistory m_PathEntry;
	CBitmapButton m_BtnGo;
	CToolTipCtrl	  m_ToolTip;
	// Generated message map functions
	//{{AFX_MSG(CPathEntryBar)
	//}}AFX_MSG
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnUpdateGo(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHSELECTBAR_H__60B8ADE3_5784_471C_B426_BE9AA9E4D1E3__INCLUDED_)
