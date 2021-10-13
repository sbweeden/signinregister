// SignInRegisterDlg.h : header file
//

#pragma once

#include "SignInRecord.h"
#include "MyListBox.h"
#include "ShooterRecord.h"
#include "SIRProps.h"

// CSignInRegisterDlg dialog
class CSignInRegisterDlg : public CDialog
{
// Construction
public:
	CSignInRegisterDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSignInRegisterDlg();

// Dialog Data
	enum { IDD = IDD_SIGNINREGISTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRangeOfficerSignin();

private:
	void SetRangeOfficerText(LPCTSTR str);
	void SetStatusText(LPCTSTR str);
	void Load(LPCTSTR filename);
	void Save();

	void ClearListBoxAndRecords();
	void ClearAllRecords();
	void OnOK();
	void OnCancel();
	bool CloseUpShop();
	void DoRangeOfficerSignIn(const ShooterRecord * pRecord = NULL);
	void DoShooterSignIn(const ShooterRecord * pRecord = NULL);

private:
	CFont m_Font;
	CFont m_FontLB;
	CPoint m_sig_anchor;

	bool m_bRangeOfficerSignedIn;
	SignInRecord * m_pCurrentRangeOfficer;
	MyListBox m_signInLB;
	CPtrArray m_records;

	CString m_filename;
	SIRProps m_props;

private:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonTestSignin();
	void AddRecordToLB( SignInRecord * pRecord, int replacingIndex = -1, bool bSave = true);
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonPrintBarcode();
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnCardSwipe(WPARAM wParam, LPARAM lParam);
};
