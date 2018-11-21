#if !defined(AFX_FREEDISKSPACEBAR_H__7AB986DB_C37E_4AC5_900E_CE1827266FA7__INCLUDED_)
#define AFX_FREEDISKSPACEBAR_H__7AB986DB_C37E_4AC5_900E_CE1827266FA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreeDiskSpaceBar.h : header file
//

#include "scbanner.h"
/////////////////////////////////////////////////////////////////////////////
// CFreeDiskSpaceBar dialog

class CFreeDiskSpaceBar : public CDialogBar
{
// Construction
public:
	CFreeDiskSpaceBar();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFreeDiskSpaceBar)
	enum { IDD = IDD_STATUS };
	CSCBanner	m_SCBanner;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreeDiskSpaceBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
	//{{AFX_MSG(CFreeDiskSpaceBar)
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREEDISKSPACEBAR_H__7AB986DB_C37E_4AC5_900E_CE1827266FA7__INCLUDED_)
