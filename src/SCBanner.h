#if !defined(AFX_SCBANNER_H__79636A92_B08E_4C4C_93E6_F6110836D758__INCLUDED_)
#define AFX_SCBANNER_H__79636A92_B08E_4C4C_93E6_F6110836D758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCBanner.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSCBanner window
class CSCBanner : public CStatic
{
// Construction
public:
	CSCBanner();

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCBanner)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnClicked();
	virtual ~CSCBanner();

	// Generated message map functions
protected:
	CToolTipCtrl m_ToolTip;
	//{{AFX_MSG(CSCBanner)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCBANNER_H__79636A92_B08E_4C4C_93E6_F6110836D758__INCLUDED_)
