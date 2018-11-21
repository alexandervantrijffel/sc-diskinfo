// EnumDir.cpp: implementation of the CEnumDir class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnumDir.h"
 
#include "stdio.h" // fopen (TEMPORARY) /******************8/


CEdirInfo::CEdirInfo()
{
	hEventAbort=0;
	bSubDirs=1;
	dwcFiles=dwcFolders=0;
	dwDFmt=20;
	icBytes.QuadPart = icBytesTotal.QuadPart = 0;
	lpfnEdirAddItem=0;
}

CEdirInfo::~CEdirInfo()
{

}















#define DIRECTORY_QUERY                 (0x0001)
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define FILE_OPEN                       0x00000001
#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_OPENED                     0x00000001
#define FILE_SUPERSEDED                 0x00000000
#define FILE_OPENED                     0x00000001
#define FILE_CREATED                    0x00000002
#define FILE_OVERWRITTEN                0x00000003
#define FILE_EXISTS                     0x00000004
#define FILE_DOES_NOT_EXIST             0x00000005
#define STATUS_NO_MORE_FILES            0x80000006L
#define FILE_SYNCHRONOUS_IO_NONALERT    0x00000020
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnumDir::CEnumDir()
{
	RtlInitUnicodeString=0;
	NtOpenFile=0;
	NtQueryDirectoryFile=0;
	NtClose=0;
	m_hNTDLL = 0;
}

CEnumDir::~CEnumDir()
{
	if (m_hNTDLL)
	{
		FreeLibrary(m_hNTDLL);
		m_hNTDLL=0;
	}
}

void CEnumDir::InitFunctionPointers()
{
	m_hNTDLL = GetModuleHandle( TEXT("ntdll.dll") );
	if (!m_hNTDLL)
		throw EDirNTDLLErr();

	if (!RtlInitUnicodeString)
	{
		RtlInitUnicodeString = ( void (__stdcall*)(PUNICODE_STRING, LPCWSTR) )
						GetProcAddress( m_hNTDLL, "RtlInitUnicodeString");
	}

	if (!NtOpenFile)
	{
		NtOpenFile = ( DWORD (__stdcall*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, 
			PIO_STATUS_BLOCK, DWORD, DWORD) ) 
						GetProcAddress( m_hNTDLL, "NtOpenFile");
	}

	if (!NtQueryDirectoryFile)
	{
		NtQueryDirectoryFile = ( DWORD (__stdcall*)(HANDLE,HANDLE,PIO_APC_ROUTINE
			, PVOID, PIO_STATUS_BLOCK, PVOID, DWORD, FILE_INFORMATION_CLASS
			, BOOLEAN, PUNICODE_STRING, BOOLEAN) )
						GetProcAddress( m_hNTDLL, "NtQueryDirectoryFile");
	}
	if (!NtClose)
	{
		NtClose = ( DWORD (__stdcall*)(HANDLE) )
						GetProcAddress( m_hNTDLL, "NtClose");
	}
	if (!RtlInitUnicodeString || !NtOpenFile || !NtQueryDirectoryFile || !NtClose)
		throw EDirNTDLLErr();
}
HANDLE CEnumDir::OpenDirectory(HANDLE hRootKey, LPCWSTR szPath)
{
	IO_STATUS_BLOCK io_status_block;
	HANDLE hDir;
	
	UNICODE_STRING ObjName;
	RtlInitUnicodeString(&ObjName, szPath);

	OBJECT_ATTRIBUTES ObjAttr;
	ObjAttr.Length=sizeof(ObjAttr);
	ObjAttr.RootDirectory=hRootKey;
	ObjAttr.ObjectName=&ObjName;
	ObjAttr.Attributes=OBJ_CASE_INSENSITIVE;
	ObjAttr.SecurityDescriptor=NULL;
	ObjAttr.SecurityQualityOfService=NULL;

	DWORD dwStatus = NtOpenFile(
		   &hDir					// OUT PHANDLE FileHandle,
		 , FILE_LIST_DIRECTORY|SYNCHRONIZE	//|FILE_READ_ATTRIBUTES|FILE_READ_EA|FILE_READ_DATA		// IN ACCESS_MASK DesiredAccess,
		 , &ObjAttr					// IN POBJECT_ATTRIBUTES ObjectAttributes,
		 , &io_status_block			// OUT PIO_STATUS_BLOCK IoStatusBlock
		 , FILE_SHARE_READ			// IN ULONG ShareAccess
		 , FILE_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_NONALERT		// IN ULONG OpenOptions
		 );

	if (dwStatus != 0)
	{
		/********************/
		MessageBoxW(NULL, szPath, L"Unable to open directory", MB_OK|MB_ICONEXCLAMATION);
		hDir=0;
	}

	return hDir;
}
void CEnumDir::GetDirectoryInformation(CEdirInfo* pInfo)
{
	InitFunctionPointers();
	m_dwcFilesTotal = m_dwcFoldersTotal = 0;

	pInfo->dwcFiles=pInfo->dwcFolders=0;
	pInfo->icBytes.QuadPart=0;

	// format native path
	WCHAR _wszPath[MAX_PATH+3];
	swprintf(_wszPath, L"\\??\\%s", pInfo->wszPath);
	
	if (EnumSubDirectory(NULL, _wszPath, pInfo, 0))
	{
		pInfo->lpfnEdirAddItem(EDIR_SEPARATOR, pInfo);
		pInfo->icBytes = pInfo->icBytesTotal;
		pInfo->dwcFiles = m_dwcFilesTotal;
		pInfo->dwcFolders = m_dwcFoldersTotal;

		pInfo->lpfnEdirAddItem(EDIR_TOTAL, pInfo);
	}
}
// App				Sleep (50)	Sleep(25)	Sleep(5)	Sleep(0)	Werkelijk
// i386				4743											4743
// program files    13364											13362
// Winnt												6581		6571
// programming								16215					16155
//								5907								5903

BOOL CEnumDir::EnumSubDirectory(HANDLE hRootKey, LPCWSTR wszSubDir, CEdirInfo* pInfo, int iSubDirDepth)
{
	if ((iSubDirDepth++ % 3) == 0)
	{
		DWORD dwRet = WaitForSingleObject(pInfo->hEventAbort, 0);
		if (WAIT_OBJECT_0 == dwRet)
			throw EDirAbortErr();
	}


 	HANDLE hDir = OpenDirectory(hRootKey, wszSubDir);
	if (!hDir)
		return FALSE;

	IO_STATUS_BLOCK io_status_block;
	FILE_QUERY_DIRECTORY fqd, *pfqd_loop;
	ZeroMemory(&fqd, sizeof(fqd));

	WCHAR wcszDir[MAX_PATH];
	UINT iPos=0xFFFF;
	DWORD dwStatus;

	// voor win9x:
//	WIN32_FIND_DATA
//	nFileSizeHigh 
//		Specifies the high-order DWORD value of the file size, in bytes. This value is zero unless the file size is greater than MAXDWORD. The size of the file is equal to (nFileSizeHigh * MAXDWORD) + nFileSizeLow. 
//	nFileSizeLow 
//		Specifies the low-order DWORD value of the file size, in bytes. 

	while (TRUE)
	{
		WaitForSingleObject(hDir, INFINITE);
		dwStatus=NtQueryDirectoryFile(
			  hDir				// IN HANDLE FileHandle 
			, 0					// IN HANDLE EventHandle OPTIONAL
			, 0					// IN PIO_APC_ROUTINE ApcRoutine OPTIONAL
			, 0					// IN PVOID ApcContext OPTIONAL
			, &io_status_block // OUT PIO_STATUS_BLOCK IoStatusBlock
			, &fqd				// OUT PVOID Buffer
			, sizeof(fqd)				// IN ULONG Bufferlength
			, FileBothDirectoryInformation //IN FILE_INFORMATION_CLASS InformationClass, // information type
			, 0					// IN BOOLEAN ReturnByOne
			, 0					// IN PUNICODE_STRING FileTemplate OPTIONAL
			, 0					// IN BOOLEAN Reset
			);

		if (dwStatus != NO_ERROR)
		{
			if (dwStatus == STATUS_NO_MORE_FILES)
				break;
			else if (dwStatus & 0x103)//STATUS_PENDING
			{
				if (io_status_block.Status == STATUS_NO_MORE_FILES)
					break;
			}
		}

		pfqd_loop = &fqd;

		do
		{
			// no dots?
			if (wcslen(pfqd_loop->FileBothDirectoryInformationClass.FileName)
				&& *pfqd_loop->FileBothDirectoryInformationClass.FileName != '.')
			{	
				// different?
//				if ( wcsncmp(wcszDir
//					, pfqd_loop->FileBothDirectoryInformationClass.FileName
//					, max(pfqd_loop->FileNameLength/sizeof(WCHAR), iPos)))

					// copy file name to buffer
					iPos=pfqd_loop->FileNameLength/sizeof(WCHAR);
					wcsncpy(wcszDir
						, pfqd_loop->FileBothDirectoryInformationClass.FileName
					, iPos);
					wcszDir[iPos]=0;
			
					if (pfqd_loop->FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (pInfo->bSubDirs)
						{
							if (1 == iSubDirDepth)
							{
								m_dwcFilesTotal += pInfo->dwcFiles;
								m_dwcFoldersTotal += (pInfo->dwcFolders)
									? pInfo->dwcFolders - 1
									: 0;
								pInfo->icBytesTotal.QuadPart += pInfo->icBytes.QuadPart;

								pInfo->dwcFiles = 0;
								pInfo->dwcFolders = 0;
								pInfo->icBytes.QuadPart = 0;
							}

							EnumSubDirectory(
									  hDir
									, wcszDir
									, pInfo
									, iSubDirDepth);
	
							if (1 == iSubDirDepth)
								pInfo->lpfnEdirAddItem(wcszDir, pInfo);

							pInfo->dwcFolders++;
						}
					}
					else
					{
						(pInfo->dwcFiles)++;
						pInfo->icBytes.QuadPart+=pfqd_loop->EndOfFile.QuadPart;
					}
			}
	
			// last one?
			if (0 == pfqd_loop->NextEntryOffset)
				break;

			// go to next entry
			pfqd_loop = (PFILE_QUERY_DIRECTORY)((DWORD)pfqd_loop+pfqd_loop->NextEntryOffset);

		}while(TRUE);
	}
	
	NtClose(hDir);

	if (1 == iSubDirDepth)
	{	
		m_dwcFilesTotal += pInfo->dwcFiles;
		m_dwcFoldersTotal += (pInfo->dwcFolders)
			? pInfo->dwcFolders - 1
			: 0;
		pInfo->icBytesTotal.QuadPart += pInfo->icBytes.QuadPart;
	}

//	else if (iSubDirDepth==2)

	return TRUE;
}

/*
BOOL CEnumDir::EnumSubDirectory(HANDLE hRootKey, LPCWSTR wszSubDir, LPDWORD pdwcFiles, PLARGE_INTEGER picBytes, BOOL bSubDirs)
{
	HANDLE hDir = OpenDirectory(hRootKey, wszSubDir);
	if (!hDir)
		return FALSE;

	IO_STATUS_BLOCK io_status_block;
	FILE_QUERY_DIRECTORY fqd, *pfqd_loop;
	ZeroMemory(&fqd, sizeof(fqd));

	WCHAR wcszDir[MAX_PATH];
	UINT iPos=0xFFFF;
	DWORD dwStatus;

	// voor win9x:
//	WIN32_FIND_DATA
//	nFileSizeHigh 
//		Specifies the high-order DWORD value of the file size, in bytes. This value is zero unless the file size is greater than MAXDWORD. The size of the file is equal to (nFileSizeHigh * MAXDWORD) + nFileSizeLow. 
//	nFileSizeLow 
//		Specifies the low-order DWORD value of the file size, in bytes. 

	while (TRUE)
	{
		WaitForSingleObject(hDir, INFINITE);
		dwStatus=NtQueryDirectoryFile(
			  hDir				// IN HANDLE FileHandle 
			, 0					// IN HANDLE EventHandle OPTIONAL
			, 0					// IN PIO_APC_ROUTINE ApcRoutine OPTIONAL
			, 0					// IN PVOID ApcContext OPTIONAL
			, &io_status_block // OUT PIO_STATUS_BLOCK IoStatusBlock
			, &fqd				// OUT PVOID Buffer
			, sizeof(fqd)				// IN ULONG Bufferlength
			, FileBothDirectoryInformation //IN FILE_INFORMATION_CLASS InformationClass, // information type
			, 0					// IN BOOLEAN ReturnByOne
			, 0					// IN PUNICODE_STRING FileTemplate OPTIONAL
			, 0					// IN BOOLEAN Reset
			);

		if (dwStatus != NO_ERROR)
		{
			if (dwStatus == STATUS_NO_MORE_FILES)
				break;
			else if (dwStatus & 0x103)//STATUS_PENDING
			{
				if (io_status_block.Status == STATUS_NO_MORE_FILES)
					break;
			}
		}

		pfqd_loop = &fqd;

		do
		{
			// no dots?
			if (wcslen(pfqd_loop->FileBothDirectoryInformationClass.FileName)
				&& *pfqd_loop->FileBothDirectoryInformationClass.FileName != '.')
			{	
				// different?
//				if ( wcsncmp(wcszDir
//					, pfqd_loop->FileBothDirectoryInformationClass.FileName
//					, max(pfqd_loop->FileNameLength/sizeof(WCHAR), iPos)))

					// copy file name to buffer
					iPos=pfqd_loop->FileNameLength/sizeof(WCHAR);
					wcsncpy(wcszDir
						, pfqd_loop->FileBothDirectoryInformationClass.FileName
						, iPos);
					wcszDir[iPos]=0;
			
					if (pfqd_loop->FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (bSubDirs)
						{
								EnumSubDirectory(
									  hDir
									, wcszDir
									, pdwcFiles
									, picBytes
									, bSubDirs);
						}
					}
					else
					{
						(*pdwcFiles)++;
						picBytes->QuadPart+=pfqd_loop->EndOfFile.QuadPart;
					}
			}
	
			// last one?
			if (0 == pfqd_loop->NextEntryOffset)
				break;

			// go to next entry
			pfqd_loop = (PFILE_QUERY_DIRECTORY)((DWORD)pfqd_loop+pfqd_loop->NextEntryOffset);

		}while(TRUE);
	}
	
	NtClose(hDir);
	return TRUE;
}
*/

// NT CreateFile:
/*
	DWORD (__stdcall *NtCreateFile)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES
		, PIO_STATUS_BLOCK, PLARGE_INTEGER, DWORD, DWORD, DWORD, DWORD, PVOID, DWORD);
	
	NtCreateFile = ( DWORD (__stdcall*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES
		, PIO_STATUS_BLOCK, PLARGE_INTEGER, DWORD, DWORD, DWORD, DWORD, PVOID, DWORD) )
				GetProcAddress( GetModuleHandle( TEXT("ntdll.dll") ), "NtCreateFile");

	if (NtCreateFile == 0)
		return 0;

	HANDLE hDir=0;
	IO_STATUS_BLOCK io_status_block;
	DWORD stat = NtCreateFile( &hDir
		, FILE_READ_ATTRIBUTES // | DELETE | FILE_READ_EA | READ_CONTROL // FILE_LIST_DIRECTORY
		, &ObjAttr
		, &io_status_block
		, 0
		, 0 //FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM
		, 0
		, FILE_OPEN 
		, FILE_DIRECTORY_FILE
		, NULL
		, 0);

	if (stat!=0)
		return 0;

	DWORD (__stdcall *NtQueryDirectoryFile)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID
		, PIO_STATUS_BLOCK, PVOID, DWORD, DWORD, DWORD, PCWSTRING, DWORD);

	NtQueryDirectoryFile = ( DWORD (__stdcall*)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID
		, PIO_STATUS_BLOCK, PVOID, DWORD, DWORD, DWORD, PCWSTRING, DWORD) )
						GetProcAddress( GetModuleHandle( TEXT("ntdll.dll") ), 
							"NtQueryDirectoryFile");
	if (NtQueryDirectoryFile)
	{



	}


	DWORD (__stdcall *NtClose)(HANDLE);

	NtClose = ( DWORD (__stdcall*)(HANDLE) )
		GetProcAddress( GetModuleHandle( TEXT("ntdll.dll") ), "NtClose");

	if ( NtClose == NULL )
		return 0;

	DWORD status = NtClose(hDir);


	if (status != 0)
		return 0;

	return 0;

		
	
		FILE_STANDARD_INFORMATION fsi;
		IO_STATUS_BLOCK io_status_block;

	//	// QueryDirectory / QueryEa
	//

	#define SL_RESTART_SCAN                 0x01
	#define SL_RETURN_SINGLE_ENTRY          0x02
	#define SL_INDEX_SPECIFIED              0x04

		TCHAR sz[0x268];
		DWORD dwStat=NtQueryDirectoryFile( hDir	// Directory handle
			, (HANDLE)0						// Event
			, 0								// ApcRoutine
			, 0								// ApcContext
			, &io_status_block				// STATUS
			, (PVOID)sz//&fsi				// BUFFER 
			, 0x268 //sizeof(fsi)			// SIZE OF BUFFER
			, FileBothDirectoryInformation	// CLASS
			, 1								// return single entry
			, L"nbtscan"					// FileName
			, 0								// RESTART SCAN
			);
		
		if (!dwStat)
			ERRBOX(_T("YES"));
	}

	//	FileBothDirectoryInformation

	DWORD (__stdcall *NtQueryInformationFile)( HANDLE, PIO_STATUS_BLOCK, PVOID, DWORD, DWORD );
	NtQueryInformationFile = ( DWORD (__stdcall *)( HANDLE, PIO_STATUS_BLOCK, PVOID, DWORD, DWORD) )
                               GetProcAddress( GetModuleHandle( TEXT( "ntdll.dll" ) ),
                              "NtQueryInformationFile" );

	if (NtQueryInformationFile)
	{
		BY_HANDLE_FILE_INFORMATION FileInfo;

		if (GetFileInformationByHandle(hDir, &FileInfo))
		{
			DWORD dw=0;
		
		}

		FILE_STANDARD_INFORMATION fsi;
		FILE_BASIC_INFORMATION fbi;
		FILE_FULL_EA_INFORMATION ffei;
		TCHAR szBuf[4096];
		for (int i=1 ; i<30 ; i++)
		{
		ULONG dwStatus = NtQueryInformationFile(hDir
			, &io_status_block
			, szBuf
			, 4096
			, i); // filebothdirectoryinformation	
		if (!dwStatus)
		{
			DWORD dw=1;
			ERRBOX( _T("gotcha!") );
		}
		else
			DWORD dw2=1;
		}
	}

HANDLE hFile =CreateFile( _T("C:\\durect")//szPath
		, FILE_READ_ATTRIBUTES | FILE_READ_EA | FILE_LIST_DIRECTORY
		, FILE_SHARE_READ
		, NULL
		, CREATE_NEW
		, 0
		, 0); 
	if (INVALID_HANDLE_VALUE == hFile)
		DWORD dwLast = GetLastError();

HANDLE hDir;
DWORD (__stdcall *ZwCreateDirectoryObject)( PHANDLE, DWORD, POBJECT_ATTRIBUTES);
ZwCreateDirectoryObject = ( DWORD (__stdcall*)(PHANDLE, DWORD, POBJECT_ATTRIBUTES) )
								GetProcAddress( GetModuleHandle( TEXT( "ntdll.dll" ) ),
									"NtCreateDirectoryObject" );

if ( ZwCreateDirectoryObject == NULL )
{
   return( FALSE );
}

DWORD status = ZwCreateDirectoryObject(&hDir
								 , DIRECTORY_QUERY
								 , &ObjAttr);
if (status != 0)
	return 0; 



	NTSYSAPI
NTSTATUS
NTAPI
NtQueryDirectoryFile (
  IN HANDLE FileHandle, // handle to the file
  IN HANDLE EventHandle OPTIONAL,
  IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
  IN PVOID ApcContext OPTIONAL,
  OUT PIO_STATUS_BLOCK IoStatusBlock,
  OUT PVOID Buffer, // pointer to the buffer to receive the result
  IN ULONG BufferLength, // length of Buffer
  IN FILE_INFORMATION_CLASS InformationClass, // information type
  IN BOOLEAN ReturnByOne, // each call returns info for only one file
  IN PUNICODE_STRING FileTemplate OPTIONAL, // template for search
  IN BOOLEAN Reset, // restart search
); 

*/
/*


  Structures:


  typedef struct _FILE_BASIC_INFORMATION {                    
    LARGE_INTEGER CreationTime;                             
    LARGE_INTEGER LastAccessTime;                           
    LARGE_INTEGER LastWriteTime;                            
    LARGE_INTEGER ChangeTime;                               
    ULONG FileAttributes;                                   
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;         
                                                            
typedef struct _FILE_STANDARD_INFORMATION {                 
    LARGE_INTEGER AllocationSize;                           
    LARGE_INTEGER EndOfFile;                                
    ULONG NumberOfLinks;                                    
    BOOLEAN DeletePending;                                  
    BOOLEAN Directory;                                      
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;   

typedef struct _FILE_FULL_EA_INFORMATION {
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[1];
} FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;





  */