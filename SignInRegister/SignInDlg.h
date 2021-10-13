#pragma once
#include "afxwin.h"
#include "Signature.h"
#include "SignInRecord.h"
#include "SIRProps.h"

// CSignInDlg dialog

class CSignInDlg : public CDialog
{
	DECLARE_DYNAMIC(CSignInDlg)

public:
	CSignInDlg(
		const SIRProps& props,
		LPCTSTR title = NULL,
		LPCTSTR membership_number = NULL, 
		LPCTSTR shooter_name = NULL,
		LPCTSTR license = NULL,
		LPCTSTR club = NULL,
		LPCTSTR catstring = NULL,
		const Signature * pSig = NULL,
		__time64_t license_expiry = 0,
		CWnd* pParent = NULL);   // standard constructor

	CSignInDlg( const SIRProps& props, LPCTSTR title, SignInRecord * pRecord, CWnd * pParent = NULL );

	__time64_t getLicenseExpiry();

	virtual ~CSignInDlg();

private:
	void commonConstruction(LPCTSTR catstring, const Signature * pSig, __time64_t license_expiry);
	bool CheckForIllegalChar(LPCTSTR str, LPCTSTR fieldName);
	void AutoComplete(LPCTSTR membership_number);
	void CheckEmptyFields();
	void getStartOfToday(COleDateTime &dt);
	int getDaysTillExpiry();
	void clearExpiryDaysDisplay();
	void updateExpiryDaysDisplay();

public: 
// Dialog Data
	enum { IDD = IDD_SIGNIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS * lpwndpos);
	DECLARE_MESSAGE_MAP()

public:
	CString m_title;
	CRect m_sigArea;
	CPoint m_anchor;
	CPoint m_lastPoint;
	bool m_drawing;
	Signature m_sig;
	CString m_cat_string;

private:
	// reference to the properties
	const SIRProps& m_props;

	// used for controlling window position in middle of screen
	int m_xpos, m_ypos, m_sz_x, m_sz_y;

	// used for display of expiry days
	CFont m_FontExpiry;
	BOOL showExpiryDaysOnInit;

protected:

public:
	afx_msg void OnPaint();
public:
	CString m_membership_number;
public:
	CString m_shooter_name;
public:
	CString m_license;
public:
	CString m_club;
public:
	BOOL m_cat_a;
public:
	BOOL m_cat_b;
public:
	BOOL m_cat_c;
public:
	COleDateTime m_license_expiry;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonClearsig();
public:
	afx_msg void OnEnChangeEditMembershipNumber();
public:
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonTomorrow();
	afx_msg void OnDtnDatetimechangeLicenseExpiry(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
