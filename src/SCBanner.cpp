// SCBanner.cpp : implementation file
//

#include "stdafx.h"
#include "DiskInfo.h"
#include "SCBanner.h"

#include "hyperlink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCBanner

CSCBanner::CSCBanner()
{
}

CSCBanner::~CSCBanner()
{
}


BEGIN_MESSAGE_MAP(CSCBanner, CStatic)
	//{{AFX_MSG_MAP(CSCBanner)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCBanner message handlers

BOOL CSCBanner::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(LoadCursor(0, IDC_HAND));
	return 1;
}

void CSCBanner::PreSubclassWindow() 
{
	// we want to receive mouse clicks via STN_CLICKED
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetStyle() | SS_NOTIFY);

	m_ToolTip.Create(this);
	m_ToolTip.AddTool(this, ID_SCBANNER);
	m_ToolTip.Activate(TRUE);

	CStatic::PreSubclassWindow();
}


void CSCBanner::OnClicked()
{
	CHyperLink hlink;
	CString szURL;
	szURL.LoadString(IDS_WEB_SC_HOME);
	hlink.GotoURL(szURL, SW_SHOW);
}

BOOL CSCBanner::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);	
	return CStatic::PreTranslateMessage(pMsg);
}
