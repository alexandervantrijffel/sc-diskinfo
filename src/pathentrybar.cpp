// PathSelectBar.cpp : implementation file
//

#include "stdafx.h"
#include "DiskInfo.h"
#include "PathEntryBar.h"

#include "Mainfrm.h"
#include "dirtreeview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathEntryBar dialog


CPathEntryBar::CPathEntryBar()
{
	//{{AFX_DATA_INIT(CPathEntryBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CPathEntryBar::~CPathEntryBar()
{
}
		

void CPathEntryBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathEntryBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PATHENTRY, m_PathEntry);
	DDX_Control(pDX, IDC_PATHENTRY_GO, m_BtnGo);
}


BEGIN_MESSAGE_MAP(CPathEntryBar, CDialogBar)
	//{{AFX_MSG_MAP(CPathEntryBar)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	ON_UPDATE_COMMAND_UI(IDC_PATHENTRY_GO,OnUpdateGo)
	ON_BN_CLICKED(IDC_PATHENTRY_GO, OnGo)
	ON_CBN_SELCHANGE(IDC_PATHENTRY, OnComboSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathEntryBar message handlers

LRESULT CPathEntryBar::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);

	// Calculate a color effect for hilighting the button
	COLORREF Color = ::GetSysColor(COLOR_BTNFACE);

	m_BtnGo.LoadBitmaps(IDB_GO,0,0,IDB_GODSBLD);
	
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(&m_BtnGo, ID_SETPATH_GO);
	m_ToolTip.Activate(TRUE);
	
	m_PathEntry.AutoSelectOnLoad(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPathEntryBar::OnUpdateGo(CCmdUI *pCmdUI)
{
	TCHAR szWndText[MAX_PATH];
	m_PathEntry.GetWindowText(szWndText, MAX_PATH);
	pCmdUI->Enable(-1 != GetFileAttributes(szWndText));
}

void CPathEntryBar::OnGo()
{
	CString szPath; 
	m_PathEntry.GetWindowText(szPath);

	VERIFY( ((CDirTreeView*)
		((CMainFrame*)::AfxGetMainWnd())->GetLeftPane())->SelectPath(szPath) );
	
	m_PathEntry.SaveData();
}

BOOL CPathEntryBar::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		if (GetDlgItem(IDC_PATHENTRY_GO)->IsWindowEnabled())
			OnGo();

	m_ToolTip.RelayEvent(pMsg);

	return 0;
}

void CPathEntryBar::OnComboSelChange()
{
	CString szPath;
	m_PathEntry.GetLBText(m_PathEntry.GetCurSel(), szPath);

	if (!((CDirTreeView*)
		((CMainFrame*)::AfxGetMainWnd())->GetLeftPane())->SelectPath(szPath) )
		AfxMessageBox(_T("Path not found!"), MB_ICONSTOP);

}
