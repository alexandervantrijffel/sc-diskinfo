// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__21043857_037E_4CE6_9449_003916F52C27__INCLUDED_)
#define AFX_MAINFRM_H__21043857_037E_4CE6_9449_003916F52C27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include  "pathentrybar.h"
#include	  "freediskspacebar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetScanState(BOOL bScanning, LPCTSTR szPath=0, DWORD dwDFmt=0);
	CWnd* GetLeftPane();
	CWnd* GetRightPane();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CFreeDiskSpaceBar      m_wndDlgBar;
	CSplitterWnd m_Splitter;

	CString m_szStatusBarScanPath;
	UINT m_nStatusBarScanCount;
// Generated message map functions
protected:
	void OnRefresh();
	void SetFreeDiskSpace(UINT nDFmt, LPCTSTR szDFmt, LPCTSTR szPath);
	void OnStopScan();
	afx_msg void OnCmdDisplayFormat(UINT wID);
	CPathEntryBar m_wndPathEntry;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNew();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__21043857_037E_4CE6_9449_003916F52C27__INCLUDED_)
