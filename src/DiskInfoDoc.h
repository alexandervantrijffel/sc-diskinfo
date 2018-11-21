// DiskInfoDoc.h : interface of the CDiskInfoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DiskInfoDOC_H__7F18DFFA_D4EE_49FF_8B6E_70AC0D19308D__INCLUDED_)
#define AFX_DiskInfoDOC_H__7F18DFFA_D4EE_49FF_8B6E_70AC0D19308D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDiskInfoDoc : public CDocument
{
protected: // create from serialization only
	CDiskInfoDoc();
	DECLARE_DYNCREATE(CDiskInfoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiskInfoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDiskInfoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDiskInfoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DiskInfoDOC_H__7F18DFFA_D4EE_49FF_8B6E_70AC0D19308D__INCLUDED_)
