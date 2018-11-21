// DiskInfoDoc.cpp : implementation of the CDiskInfoDoc class
//

#include "stdafx.h"
#include "DiskInfo.h"

#include "DiskInfoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoDoc

IMPLEMENT_DYNCREATE(CDiskInfoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDiskInfoDoc, CDocument)
	//{{AFX_MSG_MAP(CDiskInfoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoDoc construction/destruction

CDiskInfoDoc::CDiskInfoDoc()
{
	// TODO: add one-time construction code here

}

CDiskInfoDoc::~CDiskInfoDoc()
{
}

BOOL CDiskInfoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T(""));
	//GetDocTemplate()->SetDefaultTitle(this);
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDiskInfoDoc serialization

void CDiskInfoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoDoc diagnostics

#ifdef _DEBUG
void CDiskInfoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDiskInfoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoDoc commands
