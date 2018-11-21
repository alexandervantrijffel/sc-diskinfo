; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DiskInfo.h"
LastPage=0

ClassCount=12
Class1=CDiskInfoApp
Class2=CDiskInfoDoc
Class3=CDiskInfoView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_MAINFRAME (English (U.S.))
Class5=CAboutDlg
Class6=CDirTreeView
Class7=CResultListCtrl
Resource4=IDD_ABOUTBOX (English (U.S.))
Class8=CPathEntryBar
Class9=CStdToolBar
Class10=CStdBar
Resource5=IDD_STATUS (English (U.S.))
Class11=CFreeDiskSpaceBar
Class12=CSCBanner
Resource6=IDD_PATHENTRY

[CLS:CDiskInfoApp]
Type=0
HeaderFile=DiskInfo.h
ImplementationFile=DiskInfo.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CDiskInfoDoc]
Type=0
HeaderFile=DiskInfoDoc.h
ImplementationFile=DiskInfoDoc.cpp
Filter=N

[CLS:CDiskInfoView]
Type=0
HeaderFile=DiskInfoView.h
ImplementationFile=DiskInfoView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CDiskInfoView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=DiskInfo.cpp
ImplementationFile=DiskInfo.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_VIEW_KB
Command2=ID_VIEW_MB
Command3=ID_VIEW_GB
Command4=ID_STOPSCAN
Command5=ID_REFRESH
Command6=ID_APP_ABOUT
CommandCount=6

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_VIEW_TOOLBAR
Command3=ID_VIEW_STATUS_BAR
Command4=ID_APP_ABOUT
CommandCount=4

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=11
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_WEB_HOME,static,1342308352
Control8=IDC_WEB_SUPPORT,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_SCBANNER,static,1342177294
Control11=IDC_STATIC,static,1342177283

[CLS:CDirTreeView]
Type=0
HeaderFile=DirTreeView.h
ImplementationFile=DirTreeView.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=CDirTreeView

[CLS:CResultListCtrl]
Type=0
HeaderFile=ResultListCtrl.h
ImplementationFile=ResultListCtrl.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CResultListCtrl
VirtualFilter=FWC

[DLG:IDD_PATHENTRY]
Type=1
Class=CPathEntryBar
ControlCount=3
Control1=IDC_PATHENTRY,combobox,1344340290
Control2=IDC_PATHENTRY_GO,button,1342242827
Control3=IDC_STATIC,static,1342308352

[CLS:CPathEntryBar]
Type=0
HeaderFile=pathentrybar.h
ImplementationFile=pathentrybar.cpp
BaseClass=CDialogBar
Filter=D
VirtualFilter=dWC
LastObject=CPathEntryBar

[CLS:CStdToolBar]
Type=0
HeaderFile=StdToolBar.h
ImplementationFile=StdToolBar.cpp
BaseClass=CToolBarCtrl
Filter=W

[CLS:CStdBar]
Type=0
HeaderFile=StdBar.h
ImplementationFile=StdBar.cpp
BaseClass=CToolBar
Filter=W
LastObject=CStdBar

[DLG:IDD_STATUS (English (U.S.))]
Type=1
Class=CFreeDiskSpaceBar
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATUS_FREEDISKSPACE,static,1342308352
Control3=IDC_SCBANNER,static,1342177294

[CLS:CFreeDiskSpaceBar]
Type=0
HeaderFile=FreeDiskSpaceBar.h
ImplementationFile=FreeDiskSpaceBar.cpp
BaseClass=CDialogBar
Filter=D
LastObject=IDC_HLINK_TEMP
VirtualFilter=dWC

[CLS:CSCBanner]
Type=0
HeaderFile=SCBanner.h
ImplementationFile=SCBanner.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CSCBanner

