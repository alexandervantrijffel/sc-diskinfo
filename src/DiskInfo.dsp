# Microsoft Developer Studio Project File - Name="DiskInfo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DiskInfo - Win32 Win32 UNICODE Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DiskInfo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DiskInfo.mak" CFG="DiskInfo - Win32 Win32 UNICODE Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DiskInfo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DiskInfo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "DiskInfo - Win32 Win32 UNICODE Debug" (based on "Win32 (x86) Application")
!MESSAGE "DiskInfo - Win32 Win32 UNICODE Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DiskInfo - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "out/Release"
# PROP Intermediate_Dir "out/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "DiskInfo - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out/Debug"
# PROP Intermediate_Dir "out/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "DiskInfo - Win32 Win32 UNICODE Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DiskInfo___Win32_Win32_UNICODE_Debug"
# PROP BASE Intermediate_Dir "DiskInfo___Win32_Win32_UNICODE_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out/i386ud"
# PROP Intermediate_Dir "out/i386ud"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MSIMG32.LIB /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "DiskInfo - Win32 Win32 UNICODE Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DiskInfo___Win32_Win32_UNICODE_Release"
# PROP BASE Intermediate_Dir "DiskInfo___Win32_Win32_UNICODE_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "out/i386ur"
# PROP Intermediate_Dir "out/i386ur"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 MSIMG32.LIB /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"out/i386ur/scdixpstd.exe"

!ENDIF 

# Begin Target

# Name "DiskInfo - Win32 Release"
# Name "DiskInfo - Win32 Debug"
# Name "DiskInfo - Win32 Win32 UNICODE Debug"
# Name "DiskInfo - Win32 Win32 UNICODE Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ComboHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\DirTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DirTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DiskInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DiskInfo.rc

!IF  "$(CFG)" == "DiskInfo - Win32 Release"

!ELSEIF  "$(CFG)" == "DiskInfo - Win32 Debug"

!ELSEIF  "$(CFG)" == "DiskInfo - Win32 Win32 UNICODE Debug"

!ELSEIF  "$(CFG)" == "DiskInfo - Win32 Win32 UNICODE Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DiskInfoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DiskInfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumDir.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeDiskSpaceBar.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PathEntryBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SCBanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ComboHistory.h
# End Source File
# Begin Source File

SOURCE=.\DirTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DirTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DiskInfo.h
# End Source File
# Begin Source File

SOURCE=.\DiskInfoDoc.h
# End Source File
# Begin Source File

SOURCE=.\DiskInfoView.h
# End Source File
# Begin Source File

SOURCE=.\EdirInfo.h
# End Source File
# Begin Source File

SOURCE=.\EnumDir.h
# End Source File
# Begin Source File

SOURCE=.\FlatHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FreeDiskSpaceBar.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PathEntryBar.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SCBanner.h
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DiskInfo.ico
# End Source File
# Begin Source File

SOURCE=.\res\DiskInfo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DiskInfoDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\go.bmp
# End Source File
# Begin Source File

SOURCE=.\res\godsbld.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\logosc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logoscsmall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\scbanner.bmp
# End Source File
# Begin Source File

SOURCE=.\res\scbanner2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclogo.bmp
# End Source File
# Begin Source File

SOURCE=".\res\STOP-UNINSTALL.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
