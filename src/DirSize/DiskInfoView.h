// DiskInfoView.h : interface of the CDiskInfoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DiskInfoVIEW_H__7DF732C0_B5CF_4EC3_90AC_7651A210C1BA__INCLUDED_)
#define AFX_DiskInfoVIEW_H__7DF732C0_B5CF_4EC3_90AC_7651A210C1BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resultlistctrl.h"
#include "enumdir.h"
	
	
class CDiskInfoView : public CView
{
protected: // create from serialization only
	CDiskInfoView();
	DECLARE_DYNCREATE(CDiskInfoView)

// Attributes
public:
	CDiskInfoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiskInfoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	static BOOL EdirAddItem(LPCTSTR pszFolderPath, void* pEdirInfo);
	void StartEnum(CString szPath);
	virtual ~CDiskInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	static CResultListCtrl m_ResultListCtrl;
	CEnumDir m_EnumDir;
// Generated message map functions
protected:
	//{{AFX_MSG(CDiskInfoView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DiskInfoView.cpp
inline CDiskInfoDoc* CDiskInfoView::GetDocument()
   { return (CDiskInfoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DiskInfoVIEW_H__7DF732C0_B5CF_4EC3_90AC_7651A210C1BA__INCLUDED_)
