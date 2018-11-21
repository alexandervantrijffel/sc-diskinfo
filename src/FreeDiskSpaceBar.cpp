// FreeDiskSpaceBar.cpp : implementation file
//

#include "stdafx.h"
#include "DiskInfo.h"
#include "FreeDiskSpaceBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreeDiskSpaceBar dialog


CFreeDiskSpaceBar::CFreeDiskSpaceBar()
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CFreeDiskSpaceBar)
	//}}AFX_DATA_INIT
}


void CFreeDiskSpaceBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFreeDiskSpaceBar)
	DDX_Control(pDX, IDC_SCBANNER, m_SCBanner);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFreeDiskSpaceBar, CDialogBar)
	//{{AFX_MSG_MAP(CFreeDiskSpaceBar)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreeDiskSpaceBar message handlers

LRESULT CFreeDiskSpaceBar::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	// als onderstaande procedure niet wordt uitgevoerd valt de hele functionaliteit
	// van m_SCBanner in de soep!
	UpdateData(FALSE);
	return 0;
}