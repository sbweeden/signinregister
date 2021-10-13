// SignInDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SignInRegister.h"
#include "SignInDlg.h"
#include "Msg.h"
#include "ShooterDatabaseFactory.h"
#include "FinancialMembers.h"
#include "LookupDialog.h"
#include "FieldLimits.h"

#define SIGRECT_LEFT 40
#define SIGRECT_TOP 215

#define SIGRECT_RIGHT (SIGRECT_LEFT+SIG_PIXEL_WIDTH)
#define SIGRECT_BOTTOM (SIGRECT_TOP+SIG_PIXEL_HEIGHT)

#define EXPIRYTXT_LEFT 300
#define EXPIRYTXT_TOP 90


#define ILLEGAL_CHAR ";"

// CSignInDlg dialog

IMPLEMENT_DYNAMIC(CSignInDlg, CDialog)

CSignInDlg::CSignInDlg(
	const SIRProps& props,
	LPCTSTR title /* = NULL */,
	LPCTSTR membership_number /* = NULL */, 
	LPCTSTR shooter_name /* = NULL */,
	LPCTSTR license /* = NULL */,
	LPCTSTR club /* = NULL */,
	LPCTSTR catstring /* = NULL */,
	const Signature * pSig, /* = NULL */
	__time64_t license_expiry, /* = 0 */
	CWnd* pParent /* = NULL */)
	: CDialog(CSignInDlg::IDD, pParent),
	m_props(props),
	m_title(title),
	m_drawing(false),
	m_sigArea(SIGRECT_LEFT, SIGRECT_TOP, SIGRECT_RIGHT, SIGRECT_BOTTOM),
	m_membership_number(membership_number),
	m_shooter_name(shooter_name),
	m_license(license),
	m_club(club),
	m_cat_a(FALSE),
	m_cat_b(FALSE),
	m_cat_c(FALSE),
	m_anchor(SIGRECT_LEFT, SIGRECT_TOP)
{
	commonConstruction(catstring, pSig, license_expiry);
}

CSignInDlg::CSignInDlg( const SIRProps& props, LPCTSTR title, SignInRecord * pRecord, CWnd * pParent /* = NULL */ )
	: CDialog(CSignInDlg::IDD, pParent),
	m_props(props),
	m_title(title),
	m_drawing(false),
	m_sigArea(SIGRECT_LEFT, SIGRECT_TOP, SIGRECT_RIGHT, SIGRECT_BOTTOM),
	m_membership_number(pRecord->getMembershipNumber()),
	m_shooter_name(pRecord->getShooterName()),
	m_license(pRecord->getLicense()),
	m_club(pRecord->getClub()),
	m_cat_a(FALSE),
	m_cat_b(FALSE),
	m_cat_c(FALSE),
	m_anchor(SIGRECT_LEFT, SIGRECT_TOP)
{
	commonConstruction(pRecord->getCategory(), &pRecord->getSignature(), pRecord->getLicenseExpiry());
}

CSignInDlg::~CSignInDlg()
{
}

void CSignInDlg::commonConstruction(LPCTSTR catstring, const Signature * pSig, __time64_t license_expiry)
{
	if ( pSig != NULL )
	{
		m_sig = *pSig;
	}

	CString catstr(catstring);
	if ( catstr.Find('A') >= 0 )
	{
		m_cat_a = TRUE;
	}
	if ( catstr.Find('B') >= 0 )
	{
		m_cat_b = TRUE;
	}
	if ( catstr.Find('C') >= 0 )
	{
		m_cat_c = TRUE;
	}

	if (license_expiry > 0) 
	{
		COleDateTime dt(license_expiry);
		m_license_expiry = dt;
		showExpiryDaysOnInit = TRUE;
	} 
	else 
	{
		showExpiryDaysOnInit = FALSE;
		getStartOfToday(m_license_expiry);
	}

	BOOL result = m_FontExpiry.CreateFont(
		30, /* height */
		0, /* width */
		0, /* escapment */
		0, /* orientation */
		FW_DONTCARE, /* weight */
		FALSE, /* italic */
		FALSE, /* underline */
		FALSE, /* strikeout */
		DEFAULT_CHARSET, /* charset */
		OUT_DEFAULT_PRECIS, /* out precision */
		CLIP_DEFAULT_PRECIS, /* clip precision */
		DEFAULT_QUALITY, /* quality */
		FF_DONTCARE, /* pitch and family */
		_T("Hevetica") /* facename */
		);

}

__time64_t CSignInDlg::getLicenseExpiry()
{
	// convert COleDateTime to time
	__time64_t result = 0;
	SYSTEMTIME systime;
	if (m_license_expiry.GetAsSystemTime(systime)) {
		CTime ct(systime);
		result = ct.GetTime();
	}
	return result;
}

bool CSignInDlg::CheckForIllegalChar(LPCTSTR str, LPCTSTR fieldName)
{
	CString mystr(str);
	bool result = (mystr.Find(_T(ILLEGAL_CHAR)) < 0);
	if ( !result )
	{
		MessageBox(CFMsg(IDS_ERR_ILLEGAL_CHAR, _T(ILLEGAL_CHAR), fieldName), CMsg(IDS_MB_ERROR), MB_OK);
	}
	return result;
}


void CSignInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MEMBERSHIP_NUMBER, m_membership_number);
	DDX_Text(pDX, IDC_EDIT_SHOOTER_NAME, m_shooter_name);
	DDX_Text(pDX, IDC_EDIT_LICENSE, m_license);
	DDX_Text(pDX, IDC_EDIT_CLUB, m_club);
	DDX_Check(pDX, IDC_CHECK_A, m_cat_a);
	DDX_Check(pDX, IDC_CHECK_B, m_cat_b);
	DDX_Check(pDX, IDC_CHECK_C, m_cat_c);
	DDX_DateTimeCtrl(pDX, IDC_LICENSE_EXPIRY, m_license_expiry);
}


BEGIN_MESSAGE_MAP(CSignInDlg, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CSignInDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSIG, &CSignInDlg::OnBnClickedButtonClearsig)
	ON_EN_CHANGE(IDC_EDIT_MEMBERSHIP_NUMBER, &CSignInDlg::OnEnChangeEditMembershipNumber)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSignInDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_TOMORROW, &CSignInDlg::OnBnClickedButtonTomorrow)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_LICENSE_EXPIRY, &CSignInDlg::OnDtnDatetimechangeLicenseExpiry)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CSignInDlg::OnInitDialog()
{
	// center window in screen
	CRect pos;
	GetWindowRect(&pos);
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	m_sz_x = pos.right - pos.left;
	m_sz_y = pos.bottom - pos.top;
	m_xpos = (x-m_sz_x)/2;
	m_ypos = (y-m_sz_y)/2;
	SetWindowPos(&wndTopMost, m_xpos, m_ypos, m_sz_x, m_sz_y, SWP_SHOWWINDOW );


	CEdit * pMembershipEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_MEMBERSHIP_NUMBER);
	pMembershipEdit->SetLimitText(MAX_MEMBERSHIP_LEN);

	CEdit * pShooterNameEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_SHOOTER_NAME);
	pShooterNameEdit->SetLimitText(MAX_NAME_LEN);

	CEdit * pLicenseEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_LICENSE);
	pLicenseEdit->SetLimitText(MAX_LICENSE_LEN);

	CEdit * pClubEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_CLUB);
	pClubEdit->SetLimitText(MAX_CLUB_LEN);

	/* Don't need category b for shotgun shooting */
	CButton * pB = (CButton *) this->GetDlgItem(IDC_CHECK_B);
	pB->EnableWindow(FALSE);

	// Set the days till expiry as static text
	CStatic * pStaticExpiryDays = (CStatic *) this->GetDlgItem(IDC_TXT_EXPIRYDAYS);
	pStaticExpiryDays->SetFont(&m_FontExpiry);
	if (showExpiryDaysOnInit) {
		updateExpiryDaysDisplay();
	} else {
		clearExpiryDaysDisplay();
	}

	// if the membership number was supplied but not the name, try an auto-complete
	if ( m_membership_number.GetLength() > 0 && m_shooter_name.GetLength() == 0 )
	{
		AutoComplete(m_membership_number);
	}

	if ( m_title.GetLength() > 0 )
	{
		this->SetWindowText(m_title);
	}

	// if any of the fields are now just "0", then they should be empty
	CheckEmptyFields();

	// Set focus to first empty entry field
	if ( m_membership_number.GetLength() == 0 )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_MEMBERSHIP_NUMBER);
		pEdit->SetFocus();
	}
	else if ( m_shooter_name.GetLength() == 0 )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_SHOOTER_NAME);
		pEdit->SetFocus();
	}
	else if ( m_license.GetLength() == 0 )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_LICENSE);
		pEdit->SetFocus();
	}
	else if ( m_club.GetLength() == 0 )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_CLUB);
		pEdit->SetFocus();
	}
	else if ( m_cat_a == FALSE /* && m_cat_b == FALSE */ && m_cat_c == FALSE )
	{
		CButton * pButton = (CButton *) this->GetDlgItem(IDC_CHECK_A);
		pButton->SetFocus();
	}
	else 
	{
		// just set focus to the OK button.
		CButton * pButton = (CButton *) this->GetDlgItem(IDOK);
		pButton->SetFocus();
	}

	return FALSE; // we set the focus
}

afx_msg void CSignInDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// if the left mouse button is not down, we are not drawing
	if ( !(nFlags & MK_LBUTTON ) )
	{
		m_drawing = false;
	}
	else 
	{
		// if we are in the drawing area, either start, or continue drawing
		if ( m_sigArea.PtInRect(point) ) 
		{
			if ( m_drawing )
			{
				m_sig.AddStrokeAndDraw(this->GetDC(), m_anchor, m_lastPoint - m_anchor, point - m_anchor );
			}
			else
			{
				m_drawing = true;
			}

			// remember the last point
			m_lastPoint.SetPoint(point.x, point.y);
		}
		else
		{
			// stop drawing when we leave the drawing area
			m_drawing = false;
		}
	}
}

afx_msg void CSignInDlg::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
	// don't allow window to be moved
	lpwndpos->x = m_xpos;
	lpwndpos->y = m_ypos;
	lpwndpos->flags |= SWP_SHOWWINDOW;
}

void CSignInDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CPoint anchor(SIGRECT_LEFT, SIGRECT_TOP);
	m_sig.Draw(&dc, anchor);

	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CSignInDlg::OnBnClickedOk()
{
	// validate fields
	UpdateData(TRUE);
	boolean valid = true;

	m_cat_string = "";
	if ( m_cat_a )
	{
		m_cat_string += "A";
	}
	if ( m_cat_b )
	{
		m_cat_string += "B";
	}
	if ( m_cat_c )
	{
		m_cat_string += "C";
	}

	if ( valid && m_membership_number.GetLength() == 0 )
	{
		valid = false;
		MessageBox( CMsg(IDS_ERR_MEMBERSHIP_NUMBER), CMsg(IDS_MB_VALIDATION), MB_ICONERROR | MB_OK );
	}
	if ( valid )
	{
		valid = CheckForIllegalChar(m_membership_number, _T("Membership Number"));
	}
	if ( !valid )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_MEMBERSHIP_NUMBER);
		pEdit->SetFocus();
		return;
	}

	// check if the shooter is financial
	FinancialMembers * pFM = FinancialMembers::getFinancialMembers();
	if ( !pFM->IsFinancial(m_membership_number) )
	{
		int mbResult = MessageBox(CFMsg(IDS_NOT_FINANCIAL, m_membership_number), CMsg(IDS_MB_WARNING), MB_YESNOCANCEL | MB_ICONWARNING);
		if ( mbResult != IDYES && mbResult != IDNO )
		{
			CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_MEMBERSHIP_NUMBER);
			pEdit->SetFocus();
			return;
		}

		if (mbResult == IDYES) {
			pFM->SetFinancial(m_membership_number, true);
		}
	}

	if ( valid && m_shooter_name.GetLength() == 0 )
	{
		valid = false;
		MessageBox( CMsg(IDS_ERR_NAME), CMsg(IDS_MB_VALIDATION), MB_ICONERROR | MB_OK );
	}
	if ( valid )
	{
		valid = CheckForIllegalChar(m_shooter_name, _T("Shooter Name"));
	}
	if ( !valid )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_SHOOTER_NAME);
		pEdit->SetFocus();
		return;
	}

	if ( valid && m_license.GetLength() == 0 )
	{
		valid = false;
		MessageBox( CMsg(IDS_ERR_LICENSE), CMsg(IDS_MB_VALIDATION), MB_ICONERROR | MB_OK );
	}
	if ( valid )
	{
		valid = CheckForIllegalChar(m_license, _T("License"));
	}
	if ( !valid )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_LICENSE);
		pEdit->SetFocus();
		return;
	}

	if ( valid && m_club.GetLength() == 0 )
	{
		valid = false;
		MessageBox( CMsg(IDS_ERR_CLUB), CMsg(IDS_MB_VALIDATION), MB_ICONERROR | MB_OK );
	}
	if ( valid )
	{
		valid = CheckForIllegalChar(m_club, _T("Club"));
	}
	if ( !valid )
	{
		CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_CLUB);
		pEdit->SetFocus();
		return;
	}

	if ( valid && m_cat_string.GetLength() == 0 )
	{
		valid = false;
		MessageBox( CMsg(IDS_ERR_CATEGORY), CMsg(IDS_MB_VALIDATION), MB_ICONERROR | MB_OK );
		CButton * pButton = (CButton *) this->GetDlgItem(IDC_CHECK_A);
		pButton->SetFocus();
		return;
	}

	if ( valid && m_sig.NumStrokes() == 0 )
	{
		valid = false;
		MessageBox( CMsg(IDS_ERR_SIGNATURE), CMsg(IDS_MB_VALIDATION), MB_ICONERROR | MB_OK );
	}

	if (valid)
	{
		// check expiry date is > today
		int days_till_expiry = getDaysTillExpiry();
		if (days_till_expiry < 1) 
		{
			valid = false;
			MessageBox( CMsg(IDS_ERR_LICENSE_EXPIRY), CMsg(IDS_MB_VALIDATION), MB_ICONERROR | MB_OK );
		}

	}

	if ( valid )
	{
		OnOK();
	}
}

void CSignInDlg::OnBnClickedButtonClearsig()
{
	m_sig.ClearAllStrokes();
	m_sig.Draw(this->GetDC(), m_anchor);
}


void CSignInDlg::OnEnChangeEditMembershipNumber()
{
	CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_MEMBERSHIP_NUMBER);
	CString str;
	pEdit->GetWindowText(str);
	AutoComplete(str);
}

void CSignInDlg::AutoComplete(LPCTSTR membership_number)
{
	m_membership_number = membership_number;
	ShooterDatabase * pDB = ShooterDatabaseFactory::GetShooterDatabase();
	const ShooterRecord * pRecord = pDB->Lookup(membership_number);
	if ( pRecord != NULL )
	{
		m_shooter_name = pRecord->getShooterName();
		m_license = pRecord->getLicense();
		m_club = pRecord->getClub();
		CString catstr = pRecord->getCategory();
		if ( catstr.Find('A') >= 0 )
		{
			m_cat_a = TRUE;
		}
		if ( catstr.Find('B') >= 0 )
		{
			m_cat_b = TRUE;
		}
		if ( catstr.Find('C') >= 0 )
		{
			m_cat_c = TRUE;
		}
		COleDateTime dt(pRecord->getLicenseExpiryTime());
		m_license_expiry = dt;
		updateExpiryDaysDisplay();
	}
	else
	{
		m_shooter_name.Empty();
		m_license.Empty();
		m_club.Empty();
		m_cat_a = FALSE;
		m_cat_b = FALSE;
		m_cat_c = FALSE;
		m_license_expiry = COleDateTime::GetCurrentTime();
		clearExpiryDaysDisplay();
	}
	CheckEmptyFields();
}

void CSignInDlg::CheckEmptyFields()
{
	if ( m_membership_number == CString(_T("0")))
	{
		m_membership_number.Empty();
	}
	if ( m_shooter_name == CString(_T("0")))
	{
		m_shooter_name.Empty();
	}
	if ( m_license == CString(_T("0")))
	{
		m_license.Empty();
	}
	if ( m_club == CString(_T("0")))
	{
		m_club.Empty();
	}
	UpdateData(FALSE);
}

void CSignInDlg::OnBnClickedButtonSearch()
{
	CLookupDialog ld;
	if ( ld.DoModal() == IDOK )
	{
		AutoComplete(ld.m_selectedRecord->getMembershipNumber());
	}
}

void CSignInDlg::getStartOfToday(COleDateTime & dt)
{
	COleDateTime now = COleDateTime::GetCurrentTime();
	dt.SetTime(0,0,0);
	dt.SetDate(now.GetYear(), now.GetMonth(), now.GetDay());
}

int CSignInDlg::getDaysTillExpiry()
{
	COleDateTime today;
	getStartOfToday(today);
	COleDateTimeSpan span = m_license_expiry - today;
	return ((int) span.GetTotalDays());
}

void CSignInDlg::OnBnClickedButtonTomorrow()
{
	// set the expiry time to today's date plus one day
	COleDateTime today;
	getStartOfToday(today);
	COleDateTimeSpan span;
	span.SetDateTimeSpan(1, 0, 0, 0);
	COleDateTime tomorrow(today);
	tomorrow += span;
	m_license_expiry = tomorrow;

	// change the date field on the dialog
	CDateTimeCtrl * pExpiry = (CDateTimeCtrl *) this->GetDlgItem(IDC_LICENSE_EXPIRY);
	pExpiry->SetTime(m_license_expiry);

	// update the display of days till expiry
	updateExpiryDaysDisplay();
}


void CSignInDlg::OnDtnDatetimechangeLicenseExpiry(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	// update days till expiry
	COleDateTime dt(pDTChange->st);
	m_license_expiry = dt;
	int days_till_expiry = getDaysTillExpiry();
	updateExpiryDaysDisplay();
	*pResult = 0;
}

void CSignInDlg::clearExpiryDaysDisplay() 
{
	CStatic * pStaticExpiryDays = (CStatic *) this->GetDlgItem(IDC_TXT_EXPIRYDAYS);
	pStaticExpiryDays->SetWindowText(_T(""));
}

void CSignInDlg::updateExpiryDaysDisplay() 
{
	CStatic * pStaticExpiryDays = (CStatic *) this->GetDlgItem(IDC_TXT_EXPIRYDAYS);
	CString str(_T(" EXPIRED "));
	int days = getDaysTillExpiry();
	if (days > 0) {
		str.Format(_T(" %d "), days);
	}
	pStaticExpiryDays->SetWindowText(str);
}

HBRUSH CSignInDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Set the color of the brush for the expiry static control depending on the number of days till expiry
	if ((*pWnd) == (*(this->GetDlgItem(IDC_TXT_EXPIRYDAYS)))) {
		int days_till_expiry = getDaysTillExpiry();
		if (days_till_expiry < 1) {
			// black on red - expired
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(RGB(255,0,0));
		} else if (days_till_expiry <= m_props.getExpiryWarningDays()) {
			// black on orange - warning period 
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(RGB(255,165,0));
		} else {
			// black on green - ok
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(RGB(173,255,47));
		}
	} 

	return hbr;
}
