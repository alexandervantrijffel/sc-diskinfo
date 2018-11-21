#if !defined(AFX_DIRTREEVIEW_H__E33BF010_6B9A_40B8_99CB_578CD5D38399__INCLUDED_)
#define AFX_DIRTREEVIEW_H__E33BF010_6B9A_40B8_99CB_578CD5D38399__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirTreeView.h : header file
//

#include "dirtreectrl.h"
#include "flatheaderctrl.h"	
/////////////////////////////////////////////////////////////////////////////
// CDirTreeView view

class CDirTreeView : public CView
{
protected:
	CDirTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDirTreeView)

// Attributes
public:
		BOOL DoExplorerMenu (HWND hwnd, LPCTSTR pszPath, POINT point);
// Operations
public:
	CString GetSelectedPath();
	BOOL SelectPath(LPCTSTR szPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirTreeView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDirTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	// Generated message map functions
protected:
	void OnRClick(WPARAM wParam, LPARAM lpNMHDR);
	CDirTreeCtrl m_DirTreeCtrl;
	CFlatHeaderCtrl m_wndHeader;
	UINT m_nDirTreeCtrlyOffset;
	UINT GetItemCount (LPITEMIDLIST pidl);
	void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//{{AFX_MSG(CDirTreeView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRTREEVIEW_H__E33BF010_6B9A_40B8_99CB_578CD5D38399__INCLUDED_)
