// DiskInfoView.cpp : implementation of the CDiskInfoView class
//

#include "stdafx.h"
#include "DiskInfo.h"

#include "mainfrm.h"
#include "DiskInfoDoc.h"
#include "DiskInfoView.h"
#include "settings.h"
#include "enumdir.h"
#include "global.h"

#include <afxpriv.h> // WM_SETMESSAGESTRING

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ULONG WINAPI EnumThreadProc(PVOID lpParam)
{
	CEdirInfo* pInfo = (CEdirInfo*)lpParam;
	CEnumDir EnumDir;
	try
	{
		EnumDir.GetDirectoryInformation(pInfo);
	}
	catch(EDirNTDLLErr)
	{
		AfxMessageBox(_T("Operation failed"), MB_ICONSTOP);
	}
	catch(EDirAbortErr)
	{
	}

	delete pInfo;
	return 0;
}

BOOL CDiskInfoView::m_bScanning=0;
CReportCtrl CDiskInfoView::m_ResultListCtrl;
/////////////////////////////////////////////////////////////////////////////
// CDiskInfoView

IMPLEMENT_DYNCREATE(CDiskInfoView, CView)

BEGIN_MESSAGE_MAP(CDiskInfoView, CView)
	//{{AFX_MSG_MAP(CDiskInfoView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoView construction/destruction

CDiskInfoView::CDiskInfoView()
{
	m_hEnumThread=0;
	m_hEventEndEdir=0;
}

CDiskInfoView::~CDiskInfoView()
{
	EndEnumThread();

	if (m_hEventEndEdir)
		CloseHandle(m_hEventEndEdir);
	Sleep(1000);
}

BOOL CDiskInfoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoView drawing

void CDiskInfoView::OnDraw(CDC* pDC)
{
	CDiskInfoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoView diagnostics

#ifdef _DEBUG
void CDiskInfoView::AssertValid() const
{
	CView::AssertValid();
}

void CDiskInfoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDiskInfoDoc* CDiskInfoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiskInfoDoc)));
	return (CDiskInfoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDiskInfoView message handlers

BOOL CDiskInfoView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	if (CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		// create listcontrol
		if (m_ResultListCtrl.Create(
				/*LVS_ALIGNLEFT | LVS_REPORT | TVS_SHOWSELALWAYS |*/ WS_VISIBLE, 
				CRect(0,0,0,0), 
				this, 
				IDC_RESULTLISTCTRL) )
		{

			RVSUBITEM rvs;
			rvs.lpszText = _T("Directory");
			rvs.iWidth = 100;
			
			m_ResultListCtrl.DefineSubItem(0, &rvs);

			rvs.lpszText = _T("SubDirs");
			rvs.iWidth = 50;
			m_ResultListCtrl.DefineSubItem(1, &rvs);
			
			rvs.lpszText = _T("Files");
			rvs.iWidth = 50;
			m_ResultListCtrl.DefineSubItem(2, &rvs);

			rvs.lpszText = _T("Size (MB)");
			rvs.iWidth = 90;
			rvs.nFormat = RVCF_RIGHT|HDF_RIGHT|RVCF_TEXT;
			m_ResultListCtrl.DefineSubItem(3, &rvs);

			rvs.lpszText = _T("Proportion");
			rvs.iWidth = 300;
			rvs.nFormat = RVCF_TEXT;
			m_ResultListCtrl.DefineSubItem(4, &rvs);

			m_ResultListCtrl.ActivateSubItem(0, 0);
			m_ResultListCtrl.ActivateSubItem(1, 1);
			m_ResultListCtrl.ActivateSubItem(2, 2);
			m_ResultListCtrl.ActivateSubItem(3, 3);
			m_ResultListCtrl.ActivateSubItem(4, 4);
			
/*			// set extended styles
			m_ResultListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_TWOCLICKACTIVATE);
*/
			return TRUE;
		}
	}	
	
	return FALSE;
}

void CDiskInfoView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (IsWindow(m_ResultListCtrl.GetSafeHwnd()))
		m_ResultListCtrl.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE );
}

void CDiskInfoView::StartEnum(CString szPath)
{
	EndEnumThread();

	if (!m_hEventEndEdir)
	{
		m_hEventEndEdir = CreateEvent(0, 0, 0, 0);
		WaitForSingleObject(m_hEventEndEdir, 0);
	}
	else
		ResetEvent(m_hEventEndEdir);

	m_ResultListCtrl.DeleteAllItems();
	
	CSettings settings;
	DWORD dwDFmt = settings.GetDisplayFormat();

	CEdirInfo* peInfo = new CEdirInfo;
	peInfo->bSubDirs = TRUE;
	peInfo->wszPath = szPath;
	peInfo->lpfnEdirAddItem = EdirAddItem;
	peInfo->dwDFmt = dwDFmt;
	peInfo->hEventAbort = m_hEventEndEdir;

	m_bScanning=1;

	DWORD dwID;
	m_hEnumThread = CreateThread( 0, 0, &EnumThreadProc, peInfo, 0, &dwID);	
	ASSERT(m_hEnumThread);
	
	((CMainFrame*)::AfxGetMainWnd())->SetScanState(TRUE, szPath, dwDFmt);
}

BOOL CDiskInfoView::EdirAddItem(LPCTSTR szFolderPath, void *pEdirInfo)
{
	if (!m_bScanning)
		return 0;

	CEdirInfo* pInfo = (CEdirInfo*)pEdirInfo;
	
	m_ResultListCtrl.SetProgressTotal((DWORD)
		(pInfo->icBytesTotal.QuadPart/((float)(1<<10)))); // KBytes	

	
	TCHAR szFolderName[MAX_PATH];
	TCHAR* pszLastBS = _tcsrchr(szFolderPath, _TINT('\\'));
	_tcscpy(szFolderName, (pszLastBS) ? pszLastBS+1 : szFolderPath);

	int iItem = m_ResultListCtrl.GetItemCount();
	iItem = m_ResultListCtrl.InsertItem(iItem,szFolderName);
	
	BOOL bSeparator = 0 == _tcscmp(EDIR_SEPARATOR, szFolderName);

	TCHAR szItem[MAX_PATH]=_T("");

	RVITEM rvi;
	rvi.nMask = RVIM_TEXT;
	rvi.lpszText = szItem;
	rvi.iItem = iItem;
	rvi.iSubItem=1;

	if (!bSeparator)
		_ltot(pInfo->dwcFolders, szItem, 10);
	
	m_ResultListCtrl.SetItem(&rvi);

	if (!bSeparator)
		_ltot(pInfo->dwcFiles, szItem, 10);
	
	rvi.iSubItem++;
	m_ResultListCtrl.SetItem(&rvi);

	if (!bSeparator)
		_stprintf(szItem, _T("%.3f")
			, (float) pInfo->icBytes.QuadPart / (float) (1<<pInfo->dwDFmt) ); 

	InsertDots(szItem);

	rvi.iSubItem++;
	m_ResultListCtrl.SetItem(&rvi);

	rvi.nMask = RVIM_PROGRESS|RVIM_LPARAM;
	
		
	if (bSeparator)
		rvi.lParam = ID_LPARAM_SEPARATOR;
	else
	{
		if (!_tcscmp(EDIR_TOTAL, szFolderName)) // last item?
		{
			rvi.lParam = ID_LPARAM_TOTAL;

			CMainFrame* pMainWnd = (CMainFrame*)::AfxGetMainWnd();
			
			pMainWnd->SetScanState(FALSE);
			pMainWnd->SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)_T("Scan finished"));
			m_bScanning=FALSE;
		}
		else
			rvi.lParam = ID_LPARAM_STANDARDITEM;
		
		rvi.lProgress = (int)(pInfo->icBytes.QuadPart/((float)(1<<10)));// 1024 -> KBytes
	}
	
	rvi.iSubItem++;
	m_ResultListCtrl.SetItem(&rvi);
	
	return TRUE; // continue
}



void CDiskInfoView::SetDisplayFormat(UINT nDFmt, LPCTSTR szDFmt)
{
	if (int iItems = m_ResultListCtrl.GetItemCount())
	{
		int iProgressColumn = m_ResultListCtrl.GetProgressColumn();
		ASSERT(-1 != iProgressColumn);		
		
		TCHAR szItem[MAX_PATH]=_T("");

		RVITEM rvi;
		rvi.nMask=RVIM_PROGRESS;
		rvi.lpszText = szItem;
		rvi.iSubItem = iProgressColumn;

		for (rvi.iItem = 0 ; rvi.iItem != iItems ; rvi.iItem++)
		{
			rvi.nMask=RVIM_PROGRESS|RVIM_LPARAM;
			VERIFY(m_ResultListCtrl.GetItem(&rvi));
			
			if (rvi.lParam != ID_LPARAM_SEPARATOR)
			{	
				_stprintf(szItem, _T("%.3f")
					, nDFmt == 10 ? (float)rvi.lProgress
					: (float) rvi.lProgress / (float) (1<<(nDFmt-10)) ); 
				
				rvi.nMask=RVIM_TEXT;
				rvi.lpszText = InsertDots(szItem);
				rvi.iSubItem -= 1;
				VERIFY(m_ResultListCtrl.SetItem(&rvi));
				rvi.iSubItem += 1;
			}
		}
	}

	TCHAR szHdrTxt[128];
	_stprintf(szHdrTxt, _T("Size (%s)"), szDFmt);

	HDITEM hdi;
	hdi.mask = HDI_TEXT;
	hdi.pszText = szHdrTxt;
	hdi.cchTextMax = 0;

	m_ResultListCtrl.GetHeaderCtrl()->SetItem(m_ResultListCtrl.GetColumnFromSubItem(3)
		, &hdi);
}

void CDiskInfoView::EndEnumThread()
{
	m_bScanning=FALSE;

	if (m_hEnumThread)
	{
		if (m_hEventEndEdir)
			SetEvent(m_hEventEndEdir);

		// the thread might be in the middle of EdirAddItem()
		MSG msg;	
		do
		{
			if (PeekMessage(&msg, NULL, 0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}while (WaitForSingleObject(m_hEnumThread, 500) != WAIT_OBJECT_0);
		CloseHandle(m_hEnumThread);
		m_hEnumThread=0;
	}
}
