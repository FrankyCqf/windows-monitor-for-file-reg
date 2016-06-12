// RegMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegMonitor.h"
#include "RegMonitorDlg.h"


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegMonitorDlg dialog

CRegMonitorDlg::CRegMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegMonitorDlg)
	m_mainK = _T("");
	m_subK = _T("");
	m_path = _T("");
	m_subFile = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CRegMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegMonitorDlg)
	DDX_Control(pDX, IDC_LIST1, m_showbox);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_CBString(pDX, IDC_COMBO2, m_mainK);
	DDX_Text(pDX, IDC_EDIT3, m_subK);
	DDX_Text(pDX, IDC_EDIT1, m_path);
	DDX_Check(pDX, IDC_CHECK1, m_subFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CRegMonitorDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_CBN_EDITCHANGE(IDC_COMBO2, OnEditchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegMonitorDlg message handlers

BOOL CRegMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRegMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRegMonitorDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString p = m_path;
	if (p == "") m_showbox.AddString("Must not be empty!");
	else {
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		doMonFile(p);
	}
}

DWORD WINAPI ThreadProc(LPVOID lpParam){
	HANDLE hDir; 
	char notify[1024]; 
	DWORD cbBytes; 
	CString file;
	FILE_NOTIFY_INFORMATION *pnotify=(FILE_NOTIFY_INFORMATION *)notify; 
	FILE_NOTIFY_INFORMATION *tmp; 
	CRegMonitorDlg* lpP = (CRegMonitorDlg*) lpParam;
	hDir = CreateFile(lpP->path, FILE_LIST_DIRECTORY,FILE_SHARE_READ|FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL); 
	if (hDir == INVALID_HANDLE_VALUE) {
		lpP->m_showbox.AddString(TEXT("Error!Invalid Path...\n"));
		return 0;
	}
	while (TRUE) {
		BOOL result = FALSE;
		int count = lpP->m_showbox.GetCount();
		lpP->m_showbox.SetCurSel(count - 1);
		if(ReadDirectoryChangesW(hDir, &notify, sizeof(notify),lpP->m_subFile, FILE_NOTIFY_CHANGE_FILE_NAME| FILE_NOTIFY_CHANGE_LAST_WRITE, &cbBytes, NULL, NULL)) {
			tmp = pnotify; 
			char file_name[1024];
			if (lpP->interrupt == TRUE){
			return 0;
			}
			memset(file_name,0,strlen(file_name));
            WideCharToMultiByte(CP_ACP,
                                0,
                                pnotify->FileName,
                                pnotify->FileNameLength / 2,
                                file_name,
                                99,
                                NULL,
                                NULL);
			switch(tmp->Action) 
            { 
            case FILE_ACTION_ADDED: 
				file += "File Added: ";
				file += file_name;
				file += "\n";
				lpP->m_showbox.AddString(file);
				file = "";
                break; 
            case FILE_ACTION_REMOVED:
				file += "File Removed: ";
				file += file_name;
				file += "\n";
				lpP->m_showbox.AddString(file);
				file = "";
                break; 
            case FILE_ACTION_MODIFIED: 
                file += "File Modified: ";
				file += file_name;
				file += "\n";
				lpP->m_showbox.AddString(file);
				file = "";
                break; 
            case FILE_ACTION_RENAMED_OLD_NAME: 
				file += "File Renamed Old Name: ";
				file += file_name;
				file += "\n";
				lpP->m_showbox.AddString(file);
				file = "";
                break; 
            case FILE_ACTION_RENAMED_NEW_NAME: 
                file += "File Renamed New Name: ";
				file += file_name;
				file += "\n";
				lpP->m_showbox.AddString(file);
				file = "";
                break; 
            default: 
                break; 
            }
		}
	}
	CloseHandle(hDir);
}


DWORD CRegMonitorDlg::doMonFile(CString path)
{
	CRegMonitorDlg::path = path;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, 0);
    CString tmp = "start monitor file: " + path +" (";
	//m_showbox.AddString("start monitor file: " + path +" (");
	if (m_subFile == TRUE) tmp += "monitoring its subfolder)\n";
	if (m_subFile == FALSE) tmp += "not monitoring its subfolder)\n";
	m_showbox.AddString(tmp);
	CRegMonitorDlg::interrupt = FALSE;
	while(TRUE)  
    {  
        DWORD result ;   
        MSG msg ;   
		if (CRegMonitorDlg::interrupt == TRUE){
			return 0;
		}
        result = MsgWaitForMultipleObjects(1, &hThread,   
            FALSE, INFINITE, QS_ALLINPUT);   
        if (result == (WAIT_OBJECT_0))  
        {  
            break;  
        }   
        else   
        {   
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);  
            DispatchMessage(&msg);   
        }   
		
    }  
	return 0;
}



//void main(int argc, char *argv[])
DWORD CRegMonitorDlg::doMonReg(CString mainK,CString subK)
{
   DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
                     REG_NOTIFY_CHANGE_ATTRIBUTES |
                     REG_NOTIFY_CHANGE_LAST_SET |
                     REG_NOTIFY_CHANGE_SECURITY; 

   HANDLE hEvent;
   HKEY   hMainKey;
   HKEY   hKey;
   LONG   lErrorCode;

   // Convert parameters to appropriate handles.
   if (_tcscmp(TEXT("HKEY_LOCAL_MACHINE"), mainK) == 0) hMainKey=HKEY_LOCAL_MACHINE;
   else if(_tcscmp(TEXT("HKEY_USERS"), mainK) == 0) hMainKey=HKEY_USERS;
   else if(_tcscmp(TEXT("HKEY_CURRENT_USER"), mainK) == 0) hMainKey=HKEY_CURRENT_USER;
   else if(_tcscmp(TEXT("HKEY_CLASSES_ROOT"), mainK) == 0) hMainKey=HKEY_CLASSES_ROOT;
   else if(_tcscmp(TEXT("HKEY_CURRENT_CONFIG"), mainK) == 0) hMainKey=HKEY_CURRENT_CONFIG;
   else 
   {
      m_showbox.AddString(TEXT("Error!Invalid Key...\n"));
      return 0;
   }

   // Open a key.
   lErrorCode = RegOpenKeyEx(hMainKey, subK, 0, KEY_NOTIFY, &hKey);
   if (lErrorCode != ERROR_SUCCESS)
   {
      m_showbox.AddString(TEXT("Error!Invalid Key...\n"));
      return 0;
   }

   // Create an event.
   hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (hEvent == NULL)
   {
      m_showbox.AddString(TEXT("Error in CreateEvent...\n"));
      return 0;
   }

   // Watch the registry key for a change of value.
   lErrorCode = RegNotifyChangeKeyValue(hKey, 
                                        TRUE, 
                                        dwFilter, 
                                        hEvent, 
                                        TRUE);
   if (lErrorCode != ERROR_SUCCESS)
   {
      m_showbox.AddString(TEXT("Error in RegNotifyChangeKeyValue...\n"));
      return 0;
   }

   // Wait for an event to occur.
   m_showbox.AddString(TEXT("Waiting for a change in the specified key...\n"));
   
   while(TRUE)  
   {  
		
       DWORD result ;   
       MSG msg ;   
	
       result = MsgWaitForMultipleObjects(1, &hEvent,   
           FALSE, INFINITE, QS_ALLINPUT);   
       if (result == (WAIT_OBJECT_0))  
       {  
           break;  
       }   
       else   
       {   
           PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);  
           DispatchMessage(&msg);   
       }   
   }  
   
   
   m_showbox.AddString(TEXT("\nChange has occurred...\n"));

   /**
	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
   {
      _tprintf(TEXT("Error in WaitForSingleObject (%d).\n"), GetLastError());
      return 0;
   }
   else m_showbox.AddString(TEXT("\nChange has occurred.\n"));
   **/
   // Close the key.
   lErrorCode = RegCloseKey(hKey);
   if (lErrorCode != ERROR_SUCCESS)
   {
      m_showbox.AddString(TEXT("Error in RegCloseKey...\n"));
      return 0;
   }
   
   // Close the handle.
   if (!CloseHandle(hEvent))
   {
      m_showbox.AddString(TEXT("Error in CloseHandle...\n"));
      return 0;
   }
}

void CRegMonitorDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CRegMonitorDlg::interrupt = TRUE;
	CDialog::OnCancel();
}

void CRegMonitorDlg::OnButton1() 
{
	if(m_mainK == "" || m_subK == "") m_showbox.AddString("Must not be empty!");
	else {
		CString tmp;
		tmp += "Start monitoring Reg: ";
		tmp += m_mainK;
		tmp += "\\";
		tmp += m_subK;
		m_showbox.AddString(tmp);
		doMonReg(m_mainK,m_subK);
	}
}

void CRegMonitorDlg::OnChangeEdit3() 
{
	UpdateData(TRUE);   
	UpdateData(FALSE);  
}

void CRegMonitorDlg::OnEditchangeCombo2() 
{
	UpdateData(TRUE);   
	UpdateData(FALSE);
}

void CRegMonitorDlg::OnChangeEdit1() 
{
	UpdateData(TRUE);   
	UpdateData(FALSE);
	
}

void CRegMonitorDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	CRegMonitorDlg::interrupt = TRUE;
	m_showbox.AddString("Stopped Monitoring...");
	GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	int count = m_showbox.GetCount();
	m_showbox.SetCurSel(count - 1);
}



void CRegMonitorDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);   
	UpdateData(FALSE);
}
