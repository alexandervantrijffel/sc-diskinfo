// EnumDir.h: interface for the CEnumDir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENUMDIR_H__D8D8D9A3_D727_49D9_957E_3CD088D270AE__INCLUDED_)
#define AFX_ENUMDIR_H__D8D8D9A3_D727_49D9_957E_3CD088D270AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define EDIR_SEPARATOR L"ннн"
#define EDIR_TOTAL	   L"Total"

//typedef BOOL (*FNEDIRADDITEM)(void*);

#define EDIR_FAILURE	0x10
#define EDIR_ABORTED	0x20

class EDirAbortErr {};
class EDirNTDLLErr	{};


class CEdirInfo  
{
public:
	CEdirInfo();
	virtual ~CEdirInfo();

	HANDLE hEventAbort;
	CString wszPath; //IN
	DWORD dwcFiles;	//OUT
	DWORD dwcFolders;
	LARGE_INTEGER icBytes; //OUT
	BOOL bSubDirs; //IN
	DWORD dwDFmt; //intermediate var
	LARGE_INTEGER icBytesTotal;
	BOOL (* lpfnEdirAddItem)(LPCTSTR pszFolderName, void* pEdirInfo);
};


class CEnumDir  
{
	typedef ULONG ACCESS_MASK;
	typedef ACCESS_MASK *PACCESS_MASK;
	typedef const WCHAR* LPWCSTR;

	typedef struct _UNICODE_STRING 
	{	
		USHORT Length;
	    USHORT MaximumLength;
		PWSTR  Buffer;
	} UNICODE_STRING;
	typedef UNICODE_STRING *PUNICODE_STRING;
	#define UNICODE_NULL ((WCHAR)0) // winnt

	typedef struct _OBJECT_ATTRIBUTES {
		ULONG Length;
		HANDLE RootDirectory;
		PUNICODE_STRING ObjectName;
		ULONG Attributes;
		PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
		PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
	} OBJECT_ATTRIBUTES;
	typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

	typedef struct _io_status_block
	{
		DWORD Status;
		DWORD Information;
	} IO_STATUS_BLOCK;
	typedef IO_STATUS_BLOCK* PIO_STATUS_BLOCK;

	typedef 
	VOID (*PIO_APC_ROUTINE) (
	    IN PVOID ApcContext,
		IN PIO_STATUS_BLOCK IoStatusBlock,
		IN ULONG Reserved
		);
	typedef enum _FILE_INFORMATION_CLASS {
	    FileDirectoryInformation = 1,
		FileFullDirectoryInformation,
		FileBothDirectoryInformation,
		FileBasicInformation,
		FileStandardInformation,
		FileInternalInformation,
		FileEaInformation,
		FileAccessInformation,
		FileNameInformation,
		FileRenameInformation,
		FileLinkInformation,
		FileNamesInformation,
		FileDispositionInformation,
		FilePositionInformation,
		FileFullEaInformation,
		FileModeInformation,
		FileAlignmentInformation,
		FileAllInformation,
		FileAllocationInformation,
		FileEndOfFileInformation,
		FileAlternateNameInformation,
		FileStreamInformation,
		FilePipeInformation,
		FilePipeLocalInformation,
		FilePipeRemoteInformation,
		FileMailslotQueryInformation,
		FileMailslotSetInformation,
		FileCompressionInformation,
		FileCopyOnWriteInformation,
		FileCompletionInformation,
		FileMoveClusterInformation,
		FileOleClassIdInformation,
		FileOleStateBitsInformation,
		FileNetworkOpenInformation,
		FileObjectIdInformation,
		FileOleAllInformation,
		FileOleDirectoryInformation,
		FileContentIndexInformation,
		FileInheritContentIndexInformation,
		FileOleInformation,
		FileMaximumInformation
	} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

	typedef struct {
	    ULONG NextEntryOffset;
	    ULONG FileIndex;
	    LARGE_INTEGER CreationTime;
	    LARGE_INTEGER LastAccessTime;
	    LARGE_INTEGER LastWriteTime;
	    LARGE_INTEGER ChangeTime;
	    LARGE_INTEGER EndOfFile;
	    LARGE_INTEGER AllocationSize;
	    ULONG FileAttributes;
		ULONG FileNameLength;
		union {
	        struct {
		        WCHAR FileName[1];	// [1]
	        } FileDirectoryInformationClass;

			struct {
				DWORD dwUknown1;
	            WCHAR FileName[1];	// [1]
		    } FileFullDirectoryInformationClass;

	        struct {
		        DWORD dwUknown2;
			    USHORT AltFileNameLen;
		        WCHAR AltFileName[12];		// [12]
				WCHAR FileName[MAX_PATH];	// [1]
			} FileBothDirectoryInformationClass;
		};
	} FILE_QUERY_DIRECTORY, *PFILE_QUERY_DIRECTORY;


public:
	HANDLE OpenDirectory(HANDLE hRootKey, LPCWSTR szPath);
	void GetDirectoryInformation(CEdirInfo* pInfo);
	
	CEnumDir();
	virtual ~CEnumDir();

protected:
	DWORD m_dwcFilesTotal;
	DWORD m_dwcFoldersTotal;
	HMODULE m_hNTDLL;

	BOOL CEnumDir::EnumSubDirectory(HANDLE hRootKey, LPCWSTR wszSubDir, CEdirInfo* pInfo, int iSubDirDepth);
	void InitFunctionPointers();

	//  funtion pointers
	void (__stdcall *RtlInitUnicodeString)(PUNICODE_STRING, LPWCSTR);

	DWORD (__stdcall *NtOpenFile)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, 
		PIO_STATUS_BLOCK, DWORD, DWORD);

	DWORD (__stdcall *NtQueryDirectoryFile)(HANDLE,HANDLE,PIO_APC_ROUTINE
		, PVOID, PIO_STATUS_BLOCK, PVOID, DWORD, FILE_INFORMATION_CLASS,
		BOOLEAN, PUNICODE_STRING, BOOLEAN);

	DWORD (__stdcall *NtClose)(HANDLE);
};

#endif // !defined(AFX_ENUMDIR_H__D8D8D9A3_D727_49D9_957E_3CD088D270AE__INCLUDED_)
