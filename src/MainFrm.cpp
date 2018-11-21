// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DiskInfo.h"

#include "MainFrm.h"
#include "dirtreeview.h"
#include "DiskInfodoc.h"
#include "DiskInfoview.h"

#include "settings.h"
#include "afxpriv.h"
#include "global.h"

#include <newapis.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_KB,ID_VIEW_GB, OnCmdDisplayFormat)
	ON_COMMAND(ID_STOPSCAN, OnStopScan)
	ON_COMMAND(ID_REFRESH, OnRefresh)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR//,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nStatusBarScanCount=0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,CBRS_SIZE_DYNAMIC|CBRS_TOP|TBSTYLE_FLAT|TBSTYLE_TRANSPARENT
		,AFX_IDW_TOOLBAR) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	m_wndToolBar.SetHeight(25);
	
	if (!m_wndDlgBar.Create(this, IDD_STATUS,
		0, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	// create a logo bar, m_wndLogo is a CDialogBar object, IDR_MAINFRAME is a dialog resource
	if (!m_wndPathEntry.Create(this, IDD_PATHENTRY, 
		0, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create logobar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndPathEntry) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}		


	m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY);
	// let the rebar own the toolbar
	m_wndToolBar.SetOwner(&m_wndReBar);

	CSettings settings;
	UINT wID;
	switch (settings.GetDisplayFormat())
	{
		case 10:
			wID = ID_VIEW_KB;
			break;
		case 20:
			wID = ID_VIEW_MB;
			break;
		case 30:
			wID = ID_VIEW_GB;
			break;	
	}

	m_wndToolBar.GetToolBarCtrl().PressButton(wID);
	for (int i=0 ; i<3 ; i++)
	{
		UINT nStyle = m_wndToolBar.GetButtonStyle(i) | BS_AUTO3STATE;
		m_wndToolBar.SetButtonStyle(i, nStyle);
	}

	m_wndToolBar.GetToolBarCtrl().SetState(ID_STOPSCAN, TBSTATE_INDETERMINATE);
	m_wndToolBar.GetToolBarCtrl().SetState(ID_REFRESH, TBSTATE_INDETERMINATE);

	m_bAutoMenuEnable=FALSE;
	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// create splitter window
	if (!m_Splitter.CreateStatic(this, 1, 2))
		return FALSE;

	CRect rc;
	GetClientRect(&rc);

	int cyborder = GetSystemMetrics(SM_CYBORDER);
	int cxborder = GetSystemMetrics(SM_CXBORDER);

	if (!m_Splitter.CreateView(0, 0, RUNTIME_CLASS(CDirTreeView), CSize(rc.Width()/2-cxborder, rc.Height()-cyborder), pContext) ||
		!m_Splitter.CreateView(0, 1, RUNTIME_CLASS(CDiskInfoView), CSize(rc.Width()/2-cxborder, rc.Height()-cyborder), pContext))
	{
		m_Splitter.DestroyWindow();
		return FALSE;
	}

	CSettings settings;
	DWORD dwDFmt = settings.GetDisplayFormat();
	((CDiskInfoView*)GetRightPane())->
		SetDisplayFormat( dwDFmt, settings.DisplayFormatToText(dwDFmt));

	return TRUE;
}

CWnd* CMainFrame::GetRightPane()
{
	return m_Splitter.GetPane(0, 1);
}

CWnd* CMainFrame::GetLeftPane()
{
	return m_Splitter.GetPane(0, 0);
}

void CMainFrame::OnFileNew() 
{
	((CDirTreeView*)m_Splitter.GetPane(0,0))->DoExplorerMenu(GetSafeHwnd()
		, _T("c:\\"), CPoint(50,50));
}

void CMainFrame::OnCmdDisplayFormat(UINT wID)
{
	UINT nFormat;
	switch (wID)
	{
		case ID_VIEW_KB:
			nFormat = 10;
			break;
		case ID_VIEW_MB:
			nFormat = 20;
			break;
		case ID_VIEW_GB:
			nFormat = 30;
			break;		
	}
	CSettings settings;
	settings.SetDisplayFormat(nFormat);

	CString szDFmt = settings.DisplayFormatToText(nFormat);
	((CDiskInfoView*)GetRightPane())->SetDisplayFormat(nFormat, szDFmt);

	CString szPath = ((CDirTreeView*)GetLeftPane())->GetSelectedPath();
	if (szPath.GetLength())
		SetFreeDiskSpace(nFormat, szDFmt, szPath);
}

void CMainFrame::OnStopScan()
{
	SetScanState(FALSE);
	((CDiskInfoView*)GetRightPane())->EndEnumThread();
	SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)_T("Scan aborted"));
}

void CMainFrame::OnRefresh()
{
	CDiskInfoView * pView = DYNAMIC_DOWNCAST( CDiskInfoView
								, ((CMainFrame*)::AfxGetMainWnd())->GetRightPane());
	CDirTreeView * pTreeView = DYNAMIC_DOWNCAST( CDirTreeView, GetLeftPane());
	pView->StartEnum(pTreeView->GetSelectedPath());

}

void CMainFrame::SetScanState(BOOL bScanning, LPCTSTR szPath, DWORD dwDFmt)
{
	m_wndToolBar.GetToolBarCtrl().SetState(ID_STOPSCAN, 
		bScanning ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE);

	m_wndToolBar.GetToolBarCtrl().SetState(ID_REFRESH,
		!bScanning ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE);

	if (szPath)
	{
		CSettings settings;
		SetFreeDiskSpace(dwDFmt, settings.DisplayFormatToText(dwDFmt), szPath);

		m_nStatusBarScanCount=0;
		m_szStatusBarScanPath.Format(_T("Scanning %s"), szPath);
		SetTimer(0, 100, 0 );
	}
	if (!bScanning)
		KillTimer(0);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	m_szStatusBarScanPath += _TINT('.');
	SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)(LPCTSTR)m_szStatusBarScanPath);
	if (++m_nStatusBarScanCount == 120)
	{
		m_szStatusBarScanPath = m_szStatusBarScanPath.Left(
			m_szStatusBarScanPath.GetLength() - m_nStatusBarScanCount);
		m_nStatusBarScanCount=0;
	}

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::SetFreeDiskSpace(UINT nDFmt, LPCTSTR szDFmt, LPCTSTR szPath)
{
	ULARGE_INTEGER cbAvailableToCaller, cbTotal, cbFree;
	VERIFY(GetDiskFreeSpaceEx(szPath, &cbAvailableToCaller,&cbTotal,&cbFree));
	CString szStatus;

	TCHAR szTotal[128];
	_ultot( (long)cbFree.QuadPart >> nDFmt, szTotal, 10);
	
	szStatus.Format(_T("%s %s (%d%%)")
		, InsertDots(szTotal)
		, szDFmt
		, (UINT) ((cbFree.QuadPart >> 10) * 100) / (cbTotal.QuadPart >> 10));

	m_wndDlgBar.SetDlgItemText(IDC_STATUS_FREEDISKSPACE,szStatus);
}
