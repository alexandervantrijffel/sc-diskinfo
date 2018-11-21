// DirTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "DiskInfo.h"
#include "DirTreeView.h"

#include "mainfrm.h"
#include "DiskInfodoc.h"
#include "DiskInfoview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView

IMPLEMENT_DYNCREATE(CDirTreeView, CView)

CDirTreeView::CDirTreeView()
{
	m_nDirTreeCtrlyOffset=0;
}

CDirTreeView::~CDirTreeView()
{
}


BEGIN_MESSAGE_MAP(CDirTreeView, CView)
	//{{AFX_MSG_MAP(CDirTreeView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_DIRTREECTRL, OnSelchanged)
	ON_NOTIFY(NM_RCLICK, IDC_DIRTREECTRL, OnRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView drawing

void CDirTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView diagnostics

#ifdef _DEBUG
void CDirTreeView::AssertValid() const
{
	CView::AssertValid();
}

void CDirTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView message handlers

void CDirTreeView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if(IsWindow(m_wndHeader.GetSafeHwnd()))
	{
		HDLAYOUT hdl;
		WINDOWPOS wpos;
		CRect rc(0,0,cx,0);
		hdl.prc = &rc;
		hdl.pwpos = &wpos;

		VERIFY(m_wndHeader.SendMessage(HDM_LAYOUT, 0, (LPARAM)&hdl));
		VERIFY(m_wndHeader.SetWindowPos(&wndTop, 0, 0, wpos.x + wpos.cx
			, wpos.y+wpos.cy, SWP_SHOWWINDOW));

		HDITEM hdi;
		hdi.mask = HDI_WIDTH;
		hdi.cxy = cx;
		m_wndHeader.SetItem(0, &hdi);

		if ( m_DirTreeCtrl.m_hWnd )
			m_DirTreeCtrl.SetWindowPos( NULL, 0, 0
			, cx, cy - (m_nDirTreeCtrlyOffset), SWP_NOZORDER | SWP_NOMOVE );
	}
	
}

BOOL CDirTreeView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	if (CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		DWORD dwHdrStyle = HDS_HORZ|HDS_BUTTONS|HDS_FULLDRAG|HDS_DRAGDROP
							|CCS_TOP|WS_VISIBLE;
		
		if(m_wndHeader.Create(dwHdrStyle, CRect(0, 0, 0, 0), this, 0))
		{		

			HDITEM hdi;
			hdi.mask = HDI_TEXT|HDI_FORMAT|HDI_WIDTH;
			hdi.fmt = HDF_LEFT;
			hdi.pszText = _T("Folders");
			hdi.cchTextMax = 8;
			hdi.cxy = 100;

			m_wndHeader.InsertItem(0, &hdi);
			m_wndHeader.SetHeight((int)(GetSystemMetrics(SM_CYFIXEDFRAME)*5.7));
			
			m_nDirTreeCtrlyOffset = GetSystemMetrics(SM_CYFIXEDFRAME)*7;
			

			if (m_DirTreeCtrl.Create( 
					  WS_CHILD | TVS_LINESATROOT | TVS_HASBUTTONS 
					  | WS_VISIBLE | TVS_HASLINES  |TVS_SHOWSELALWAYS
					, CRect(0, m_nDirTreeCtrlyOffset, 0, 0), this, IDC_DIRTREECTRL ))
			{
				m_DirTreeCtrl.DisplayTree( NULL, FALSE );
				return TRUE;
			}
	
		}
	}
	
	return TRUE;
}

void CDirTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTree = (NM_TREEVIEW*)pNMHDR;
	if (pNMTree->action < 2)
	{
		CDiskInfoView * pView = DYNAMIC_DOWNCAST( CDiskInfoView
								, ((CMainFrame*)::AfxGetMainWnd())->GetRightPane());
		pView->StartEnum(m_DirTreeCtrl.GetFullPath(pNMTree->itemNew.hItem));
		
	}
	// let CPrintView process the selected directory
	//((CMainFrame*)::AfxGetMainWnd())->GetRightView()->LoadDirectory(szItemPath);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  FUNCTION:       DuplicateItem
//
//  DESCRIPTION:    Makes a copy of the next item in an item ID list.
//
//  INPUT:          pMalloc = Pointer to an IMalloc interface.
//                  pidl    = Pointer to an item ID list.
//
//  RETURNS:        Pointer to an ITEMIDLIST containing the copied item ID.
//
//  NOTES:          It is the caller's responsibility to free the memory
//                  allocated by this function when the item ID is no longer
//                  needed. Example:
//
//                    pidlItem = DuplicateItem (pMalloc, pidl);
//                      .
//                      .
//                      .
//                    pMalloc->lpVtbl->Free (pMalloc, pidlItem);
//
//                  Failure to free the ITEMIDLIST will result in memory
//                  leaks.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LPITEMIDLIST DuplicateItem (LPMALLOC pMalloc, LPITEMIDLIST pidl)
{
    USHORT nLen;
    LPITEMIDLIST pidlNew;

    nLen = pidl->mkid.cb;
    if (nLen == 0)
        return NULL;

    pidlNew = (LPITEMIDLIST) pMalloc->Alloc (nLen + sizeof (USHORT));
    if (pidlNew == NULL)
        return NULL;

    CopyMemory (pidlNew, pidl, nLen);
    *((USHORT*) (((LPBYTE) pidlNew) + nLen)) = 0;

    return pidlNew;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  FUNCTION:       GetNextItem
//
//  DESCRIPTION:    Finds the next item in an item ID list.
//
//  INPUT:          pidl = Pointer to an item ID list.
//
//  RETURNS:        Pointer to the next item.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LPITEMIDLIST GetNextItem (LPITEMIDLIST pidl)
{
    USHORT nLen;

    if ((nLen = pidl->mkid.cb) == 0)
        return NULL;
    
    return (LPITEMIDLIST) (((LPBYTE) pidl) + nLen);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  FUNCTION:       GetItemCount
//
//  DESCRIPTION:    Computes the number of item IDs in an item ID list.
//
//  INPUT:          pidl = Pointer to an item ID list.
//
//  RETURNS:        Number of item IDs in the list.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

UINT CDirTreeView::GetItemCount (LPITEMIDLIST pidl)
{
    USHORT nLen;
    UINT nCount;

    nCount = 0;
    while ((nLen = pidl->mkid.cb) != 0) {
        pidl = GetNextItem (pidl);
        nCount++;
    }
    return nCount;
}





BOOL CDirTreeView::DoExplorerMenu (HWND hwnd, LPCTSTR pszPath, POINT point)
{
    LPMALLOC pMalloc;
    LPSHELLFOLDER psfFolder, psfNextFolder;
    LPITEMIDLIST pidlMain, pidlItem, pidlNextItem, *ppidl;
    LPCONTEXTMENU pContextMenu;
    CMINVOKECOMMANDINFO ici;
    ULONG ulCount, ulAttr;
    TCHAR tchPath[MAX_PATH];
    WCHAR wchPath[MAX_PATH];    
    UINT nCount, nCmd;
    BOOL bResult;
    HMENU hMenu;

    //
    // Make sure the file name is fully qualified and in Unicode format.
    //
    GetFullPathName (pszPath, sizeof (tchPath) / sizeof (TCHAR), tchPath, NULL);

//    if (IsTextUnicode (tchPath, lstrlen (tchPath), NULL))
#ifdef _UNICODE
        lstrcpy (wchPath, tchPath);
//    else
#else
        MultiByteToWideChar (CP_ACP, 0, pszPath, -1, wchPath,
                             sizeof (wchPath) / sizeof (WCHAR));
#endif

    //
    // Get pointers to the shell's IMalloc interface and the desktop's
    // IShellFolder interface.
    //
    bResult = FALSE;

    if (!SUCCEEDED (SHGetMalloc (&pMalloc)))
        return bResult;

    if (!SUCCEEDED (SHGetDesktopFolder (&psfFolder))) {
        pMalloc->Release();
        return bResult;
    }

    //
    // Convert the path name into a pointer to an item ID list (pidl).
    //
    if (SUCCEEDED (psfFolder->ParseDisplayName (hwnd,
        NULL, wchPath, &ulCount, &pidlMain, &ulAttr)) && (pidlMain != NULL)) {

        if (nCount = GetItemCount (pidlMain)) { // nCount must be > 0
            //
            // Initialize psfFolder with a pointer to the IShellFolder
            // interface of the folder that contains the item whose context
            // menu we're after, and initialize pidlItem with a pointer to
            // the item's item ID. If nCount > 1, this requires us to walk
            // the list of item IDs stored in pidlMain and bind to each
            // subfolder referenced in the list.
            //
            pidlItem = pidlMain;

            while (--nCount) {
                //
                // Create a 1-item item ID list for the next item in pidlMain.
                //
                pidlNextItem = DuplicateItem (pMalloc, pidlItem);
                if (pidlNextItem == NULL) {
                    pMalloc->Free (pidlMain);
                    psfFolder->Release ();
                    pMalloc->Release ();
                    return bResult;
                }

                //
                // Bind to the folder specified in the new item ID list.
                //
                if (!SUCCEEDED (psfFolder->BindToObject (
                    pidlNextItem, NULL, IID_IShellFolder, (void**)&psfNextFolder))) {
                    pMalloc->Free (pidlNextItem);
                    pMalloc->Free (pidlMain);
                    psfFolder->Release ();
                    pMalloc->Release ();
                    return bResult;
                }

                //
                // Release the IShellFolder pointer to the parent folder
                // and set psfFolder equal to the IShellFolder pointer for
                // the current folder.
                //
                psfFolder->Release ();
                psfFolder = psfNextFolder;

                //
                // Release the storage for the 1-item item ID list we created
                // just a moment ago and initialize pidlItem so that it points
                // to the next item in pidlMain.
                //
                pMalloc->Free (pidlNextItem);
                pidlItem = GetNextItem (pidlItem);
            }

            //
            // Get a pointer to the item's IContextMenu interface and call
            // IContextMenu::QueryContextMenu to initialize a context menu.
            //
            ppidl = &pidlItem;
            if (SUCCEEDED (psfFolder->GetUIObjectOf (
                hwnd, 1, (const ITEMIDLIST**)ppidl, IID_IContextMenu, NULL, (void**)&pContextMenu))) {

                hMenu = CreatePopupMenu ();
                if (SUCCEEDED (pContextMenu->QueryContextMenu (
                    hMenu, 0, 1, 0x7FFF, CMF_EXPLORE))) {

                    ::ClientToScreen (hwnd, &point);

                    //
                    // Display the context menu.
                    //
                    nCmd = TrackPopupMenu (hMenu, TPM_LEFTALIGN |
                        TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
                        point.x, point.y, 0, hwnd, NULL);		

                    //
                    // If a command was selected from the menu, execute it.
                    //
                    if (nCmd) {
                        ici.cbSize          = sizeof (CMINVOKECOMMANDINFO);
                        ici.fMask           = 0;
                        ici.hwnd            = hwnd;
						ici.lpVerb = (LPCSTR)nCmd-1;
                        ici.lpParameters    = NULL;
                        ici.lpDirectory     = NULL;
                        ici.nShow           = SW_SHOWNORMAL;
                        ici.dwHotKey        = 0;
                        ici.hIcon           = NULL;

                        if (SUCCEEDED (
                            pContextMenu->InvokeCommand (&ici)))
                            bResult = TRUE;
                    }
                }
                DestroyMenu (hMenu);
                pContextMenu->Release ();
            }
        }
        pMalloc->Free (pidlMain);
    }

    //
    // Clean up and return.
    //
    psfFolder->Release ();
    pMalloc->Release ();

    return bResult;
}

void CDirTreeView::OnRClick(WPARAM wParam, LPARAM lpNMHDR)
{
	CPoint point(GetMessagePos());
	ScreenToClient(&point);

	UINT nFlags;
	HTREEITEM hItem = m_DirTreeCtrl.HitTest(point, &nFlags);
	if ( hItem && nFlags & TVHT_ONITEM)
	{
		HTREEITEM hPrev = m_DirTreeCtrl.GetNextItem(hItem, TVGN_PREVIOUSVISIBLE);
		if (hPrev)
			DoExplorerMenu(GetSafeHwnd(), m_DirTreeCtrl.GetFullPath(hPrev), point);
	}
	else
		::MessageBox(0,0,0,0);
}

BOOL CDirTreeView::SelectPath(LPCTSTR szPath)
{
	return m_DirTreeCtrl.SetSelPath(szPath);
}

CString CDirTreeView::GetSelectedPath()
{
	return m_DirTreeCtrl.GetFullPath(m_DirTreeCtrl.GetSelectedItem());
}
