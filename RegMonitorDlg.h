// RegMonitorDlg.h : header file
//

#if !defined(AFX_REGMONITORDLG_H__A6B61493_CAAC_4866_8DB4_3160944C6475__INCLUDED_)
#define AFX_REGMONITORDLG_H__A6B61493_CAAC_4866_8DB4_3160944C6475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRegMonitorDlg dialog

class CRegMonitorDlg : public CDialog
{
// Construction
public:
	CRegMonitorDlg(CWnd* pParent = NULL);	// standard constructor
	DWORD doMonReg(CString mainK,CString subK);
	DWORD doMonFile(CString path);
	CString path;
	BOOL interrupt;
// Dialog Data
	//{{AFX_DATA(CRegMonitorDlg)
	enum { IDD = IDD_REGMONITOR_DIALOG };
	CListBox	m_showbox;
	CButton	m_ok;
	CString	m_mainK;
	CString	m_subK;
	CString	m_path;
	BOOL	m_subFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRegMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton1();
	afx_msg void OnChangeEdit3();
	afx_msg void OnEditchangeCombo2();
	afx_msg void OnChangeEdit1();
	afx_msg void OnButton4();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGMONITORDLG_H__A6B61493_CAAC_4866_8DB4_3160944C6475__INCLUDED_)
