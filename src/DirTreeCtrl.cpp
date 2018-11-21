// DirTreeCtrl.cpp: 
// 
// wrapped CTreeCtrl to select and or display folders and files (optional )
// 

#include "stdafx.h"

#include "DirTreeCtrl.h"
//#include "SortStringArray.h"

#define NULLSTRING _T("")
#define BACKSLASHCHAR _TINT('\\')
#define BACKSLASHSTRING _T("\\")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CSortStringArray::Sort()
{
	BOOL bNotDone = TRUE;   
	while (bNotDone)   
	{      bNotDone = FALSE;
      for(int pos = 0;pos < GetUpperBound();pos++)
        bNotDone |= CompareAndSwap(pos);   
	}
}
BOOL CSortStringArray::CompareAndSwap( int pos )
{
   CString temp;   
   int posFirst = pos;   
   int posNext = pos + 1;
   if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)   
   {
      temp = GetAt(posFirst);      
	  SetAt(posFirst, GetAt(posNext));
      SetAt(posNext, temp);      
	  return TRUE;   
   }   
   
   return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CDirTreeCtrl

CDirTreeCtrl::CDirTreeCtrl()
{
	m_strRoot = NULLSTRING;      // Auf Leer setzen
}

CDirTreeCtrl::~CDirTreeCtrl()
{
	m_imgList.Detach();
}


BEGIN_MESSAGE_MAP(CDirTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDirTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f�r Nachrichten CDirTreeCtrl 

BOOL CDirTreeCtrl::DisplayTree(LPCTSTR strRoot, BOOL bFiles)
{
	DWORD dwStyle = GetStyle();   // read the windowstyle
	if ( dwStyle & TVS_EDITLABELS ) 
	{
		// Don't allow the user to edit ItemLabels
		ModifyStyle( TVS_EDITLABELS , 0 );
	}
	
	// Display the DirTree with the Rootname e.g. C:\
	// if Rootname == NULL then Display all Drives on this PC
    // First, we need the system-ImageList
	
	DeleteAllItems();

	if ( !GetSysImgList() )
		return FALSE;
    m_bFiles = bFiles;  // if TRUE, Display Path- and Filenames 
	if ( strRoot == NULL || strRoot[0] == 0 )
	{
		if ( !DisplayDrives() )
			return FALSE;
		m_strRoot = NULLSTRING;
	}
    else
	{
		m_strRoot = strRoot;
		if ( m_strRoot.Right(1) != BACKSLASHCHAR )
			m_strRoot += BACKSLASHSTRING;
		HTREEITEM hParent = AddItem( TVI_ROOT, m_strRoot );
		DisplayPath( hParent, strRoot );
	}
	return TRUE;	
}
/////////////////////////////////////////////////
BOOL CDirTreeCtrl::GetSysImgList()
/////////////////////////////////////////////////
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;

	if ( GetImageList( TVSIL_NORMAL ) )
		m_imgList.Detach();
	
	hImgList = (HIMAGELIST)SHGetFileInfo( _T("C:\\"),
							  0,
							  &shFinfo,
							  sizeof( shFinfo ),
							  SHGFI_SYSICONINDEX | 
							  SHGFI_SMALLICON );
	if ( !hImgList )
	{
		m_strError = "Cannot retrieve the Handle of SystemImageList!";
		return FALSE;
	}

	m_imgList.m_hImageList = hImgList;    
    
	SetImageList( &m_imgList, TVSIL_NORMAL );
	return TRUE;   // OK
}

BOOL CDirTreeCtrl::DisplayDrives()
{
	//
	// Displaying the Availible Drives on this PC
	// This are the First Items in the TreeCtrl
	//
	DeleteAllItems();
	TCHAR  szDrives[128];
	TCHAR* pDrive;

	if ( !GetLogicalDriveStrings( sizeof(szDrives), szDrives ) )
	{
		m_strError = "Error Getting Logical DriveStrings!";
		return FALSE;
	}

	pDrive = szDrives;
	while( *pDrive )
	{
		HTREEITEM hParent = AddItem( TVI_ROOT, pDrive );
		if ( FindSubDir( pDrive ) )
			InsertItem( NULLSTRING, 0, 0, hParent );
		pDrive += _tcslen( pDrive ) + 1;
	}


	return TRUE;

}

void CDirTreeCtrl::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
{
	//
	// Displaying the Path in the TreeCtrl
	//
	CFileFind find;
	CString   strPathFiles = strPath;
	BOOL      bFind;
	CSortStringArray strDirArray;
	CSortStringArray strFileArray;
	
	if ( strPathFiles.Right(1) != BACKSLASHSTRING )
		strPathFiles += BACKSLASHSTRING;
	strPathFiles += "*.*";

	bFind = find.FindFile( strPathFiles );

	while ( bFind )
	{
		bFind = find.FindNextFile();
		if ( find.IsDirectory() && !find.IsDots() )
		{		
			strDirArray.Add( find.GetFilePath() );
		}
		if ( !find.IsDirectory() && m_bFiles )
			strFileArray.Add( find.GetFilePath() );

	}
    
	strDirArray.Sort();
	SetRedraw( FALSE );
	CWaitCursor wait;
    
	for ( int i = 0; i < strDirArray.GetSize(); i++ )
	{
			HTREEITEM hItem = AddItem( hParent, strDirArray.GetAt(i) );
			if ( FindSubDir( strDirArray.GetAt(i) ) )
				InsertItem( NULLSTRING, 0, 0, hItem );
	}

	if ( m_bFiles )
	{
		strFileArray.Sort();
		for ( i = 0; i < strFileArray.GetSize(); i++ )
			HTREEITEM hItem = AddItem( hParent, strFileArray.GetAt(i) );
	}
    
	SetRedraw( TRUE );
}

HTREEITEM CDirTreeCtrl::AddItem(HTREEITEM hParent, LPCTSTR strPath)
{
	// Adding the Item to the TreeCtrl with the current Icons
	SHFILEINFO shFinfo;
	int iIcon, iIconSel;
    CString    strTemp = strPath;
    
	if ( strTemp.Right(1) != BACKSLASHCHAR )
		 strTemp += BACKSLASHSTRING;
	if ( !SHGetFileInfo( strTemp,
						0,
						&shFinfo,
						sizeof( shFinfo ),
						SHGFI_ICON | 
					    SHGFI_SMALLICON ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return NULL;
	}

	iIcon = shFinfo.iIcon;

	// we only need the index from the system image list

	DestroyIcon( shFinfo.hIcon );

	if ( !SHGetFileInfo( strTemp,
						0,
						&shFinfo,
						sizeof( shFinfo ),
						SHGFI_ICON | SHGFI_OPENICON |
					    SHGFI_SMALLICON ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return NULL;
	}

	iIconSel = shFinfo.iIcon;

	// we only need the index of the system image list

	DestroyIcon( shFinfo.hIcon );

//	if ( strTemp.Right(1) == BACKSLASHSTRING )
//		strTemp.SetAt( strTemp.GetLength() - 1, 0 );
	
	if ( hParent == TVI_ROOT )
		return InsertItem( strTemp, iIcon, iIconSel, hParent );
	
	return InsertItem( GetSubPath( strTemp ), iIcon, iIconSel, hParent );
}

LPCTSTR CDirTreeCtrl::GetSubPath(LPCTSTR strPath)
{
	//
	// getting the last SubPath from a PathString
	// e.g. C:\temp\readme.txt
	// the result = readme.txt
	static CString strTemp;
	int     iPos;

	strTemp = strPath;
	if ( strTemp.Right(1) == BACKSLASHCHAR )
		 strTemp.SetAt( strTemp.GetLength() - 1, 0 );
	iPos = strTemp.ReverseFind( BACKSLASHCHAR );
	if ( iPos != -1 )
	    strTemp = strTemp.Mid( iPos + 1);

	return (LPCTSTR)strTemp;
}

BOOL CDirTreeCtrl::FindSubDir( LPCTSTR strPath)
{
	//
	// Are there subDirs ?
	//
	CFileFind find;
	CString   strTemp = strPath;
	BOOL      bFind;

	if ( strTemp[strTemp.GetLength()-1] == BACKSLASHCHAR )
		strTemp += "*.*";
	else
		strTemp += "\\*.*";
		
	bFind = find.FindFile( strTemp );
	
	
	while ( bFind )
	{
		bFind = find.FindNextFile();

		if ( find.IsDirectory() && !find.IsDots() )
		{
			return TRUE;
		}
		if ( !find.IsDirectory() && m_bFiles && !find.IsHidden() )
			return TRUE;
		
	}

	return FALSE;

}

void CDirTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString strPath;
	 
	if ( pNMTreeView->itemNew.state & TVIS_EXPANDED )
	{
		//UINT uTest = TVIS_EXPANDEDONCE;
		ExpandItem( pNMTreeView->itemNew.hItem, TVE_EXPAND );
	    /*
		//
		// Delete All items
		// And display the subpath
		//
		HTREEITEM hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		}
        
		strPath = GetFullPath( pNMTreeView->itemNew.hItem );
		DisplayPath( pNMTreeView->itemNew.hItem, strPath );
		*/
	}
	else
	{
		//
		// Delete the Items, but leave one there, for 
		// expanding the item next time
		//
		HTREEITEM hChild = GetChildItem( pNMTreeView->itemNew.hItem );
				
		while ( hChild ) 
		{
			DeleteItem( hChild );
			hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		}
		InsertItem( NULLSTRING, pNMTreeView->itemNew.hItem );
	}

	*pResult = 0;
}

CString CDirTreeCtrl::GetFullPath(HTREEITEM hItem)
{
	// get the Full Path of the item
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;

	strReturn = NULLSTRING;

	while ( hParent )
	{
		strTemp  = GetItemText( hParent );
		if (strTemp.GetAt(strTemp.GetLength()-1) != BACKSLASHCHAR)
			strTemp += BACKSLASHSTRING;
		strReturn = strTemp + strReturn;
		hParent = GetParentItem( hParent );
	}
    
//	strReturn.TrimRight( BACKSLASHCHAR );

    return strReturn;
}

BOOL CDirTreeCtrl::SetSelPath(LPCTSTR strPath)
{
	// Setting the Selection in the Tree
	HTREEITEM hParent = TVI_ROOT;
	int iLen = _tcslen(strPath) + 4;
	BOOL bRet = FALSE; 
    
	if ( !IsValidPath( strPath ) )
		return FALSE;

	TCHAR* pszPath = new TCHAR[iLen];
	
	for (int iPos=0 ; strPath[iPos],strPath[iPos] != _TINT(':') ; iPos++)
		pszPath[iPos] = strPath[iPos];
	
	_tcscpy(&pszPath[iPos++], _T(":\\\\"));

	if (_tcslen(strPath)>2)
		_tcscpy(&pszPath[iPos+2], &strPath[iPos+1]);
	
	_tcsupr( pszPath );
	
	if ( pszPath[_tcslen(pszPath)-1] != BACKSLASHCHAR )
		_tcscat( pszPath, BACKSLASHSTRING );
    
	int iLen2 = _tcslen( pszPath );
	TCHAR* pPath = pszPath;

	for (WORD i = 0; i < iLen2; i++ )
	{
		if ( pszPath[i] == BACKSLASHCHAR && pszPath[i+1] != BACKSLASHCHAR)
		{
			SetRedraw( FALSE );
			
			pszPath[i] = 0;
			if ( !(hParent = SearchSiblingItem( hParent, pPath )) )  
				break;
			else
			{				
				// Info:
				// the notification OnItemExpanded 
				// will not called every time 
				// after the call Expand. 
				// You must call Expand with TVE_COLLAPSE | TVE_COLLAPSERESET
				// to Reset the TVIS_EXPANDEDONCE Flag
				
				UINT uState;
				uState = GetItemState( hParent, TVIS_EXPANDEDONCE );
				if ( uState )
				{
					Expand( hParent, TVE_EXPAND );
					Expand( hParent, TVE_COLLAPSE | TVE_COLLAPSERESET );
					InsertItem(NULLSTRING, hParent ); // insert a blank child-item
					Expand( hParent, TVE_EXPAND ); // now, expand send a notification
				}
				else
					Expand( hParent, TVE_EXPAND );
			}
			pPath += _tcslen(pPath)+1;
		}
	}

	delete [] pszPath;
	
	if ( hParent ) // Ok the last subpath was found
	{		
		SelectItem( hParent ); // select the last expanded item
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	
	SetRedraw( TRUE );

    return bRet;
}

HTREEITEM CDirTreeCtrl::SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText)
{
	HTREEITEM hFound = GetChildItem( hItem );
	CString   strTemp;

	while ( hFound )
	{
		strTemp = GetItemText( hFound );
        strTemp.MakeUpper();
		if ( strTemp == strText )
			return hFound;
		hFound = GetNextItem( hFound, TVGN_NEXT );
	}

	return NULL;
}


void CDirTreeCtrl::ExpandItem(HTREEITEM hItem, UINT nCode)
{	
	CString strPath;
	
	if ( nCode == TVE_EXPAND )
	{
		HTREEITEM hChild = GetChildItem( hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( hItem );
		}
        
		strPath = GetFullPath( hItem );
		DisplayPath( hItem, strPath );
	}
}

BOOL CDirTreeCtrl::IsValidPath(LPCTSTR strPath)
{
	// This function check the Pathname
	return -1 != GetFileAttributes(strPath);
	/*
	HTREEITEM hChild;
	CString   strItem;
	CString   strTempPath = strPath;
	BOOL      bFound = FALSE;
	CFileFind find;

	hChild = GetChildItem( TVI_ROOT );
	strTempPath.MakeUpper();
	strTempPath.TrimRight(BACKSLASHCHAR);

	while ( hChild )
	{
		strItem = GetItemText( hChild );
		strItem.MakeUpper();
		if ( strItem == strTempPath.Mid( 0, strItem.GetLength() ) )
		{
			bFound = TRUE;
			break;
		}
		hChild = GetNextItem( hChild, TVGN_NEXT );
	}
    
	if ( !bFound )
		return FALSE;

	strTempPath += "\\nul";
	if ( find.FindFile( strTempPath ) )
		return TRUE;
     
	return FALSE;*/
}
