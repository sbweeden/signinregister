// SignInRegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SignInRegister.h"
#include "SignInRegisterDlg.h"
#include "Msg.h"
#include "SignInRecord.h"
#include "SignInDlg.h"
#include "MyPrintManager.h"
#include "MyPrintBarcodeManager.h"
#include "ShooterDatabaseFactory.h"
#include "FinancialMembers.h"
#include "CardData.h"
#include "MyDelete.h"
#include "SIRProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SIG_ANCHOR_LEFT 615
#define SIG_ANCHOR_TOP 440

#define TIMER_TIME 2000

#define SIGNATURE_SCALE 0.43


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSignInRegisterDlg dialog




CSignInRegisterDlg::CSignInRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignInRegisterDlg::IDD, pParent),
	m_bRangeOfficerSignedIn(false),
	m_sig_anchor(SIG_ANCHOR_LEFT, SIG_ANCHOR_TOP)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	BOOL result = m_Font.CreateFont(
		20, /* height */
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

	result = m_FontLB.CreateFont(
		15, /* height */
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
		_T("Courier New") /* facename */
		);

	CTime t = CTime::GetCurrentTime();

	// set the filename to a file representing today
	m_filename = _T("c:\\signinregister\\signinregister");
	m_filename += t.Format("%Y%m%d");
	m_filename += _T(".sir");

	m_signInLB.SetParent(this);
}

CSignInRegisterDlg::~CSignInRegisterDlg()
{
	ClearAllRecords();
}


void CSignInRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_signInLB);
}

BEGIN_MESSAGE_MAP(CSignInRegisterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RANGE_OFFICER_SIGNIN, &CSignInRegisterDlg::OnBnClickedButtonRangeOfficerSignin)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSignInRegisterDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_TEST_SIGNIN, &CSignInRegisterDlg::OnBnClickedButtonTestSignin)
	ON_LBN_DBLCLK(IDC_LIST1, &CSignInRegisterDlg::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CSignInRegisterDlg::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_BARCODE, &CSignInRegisterDlg::OnBnClickedButtonPrintBarcode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CSignInRegisterDlg::OnBnClickedButtonBrowse)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CSignInRegisterDlg::OnBnClickedOk)
	ON_MESSAGE(WM_CARDSWIPED_MESSAGE, &CSignInRegisterDlg::OnCardSwipe)
END_MESSAGE_MAP()


// CSignInRegisterDlg message handlers

BOOL CSignInRegisterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_signInLB.SetFont(&m_FontLB);

	CStatic * pStaticRO = (CStatic *) this->GetDlgItem(IDC_TXT_RANGE_OFFICER);
	CStatic * pStaticStatus = (CStatic *) this->GetDlgItem(IDC_TXT_STATUS);
	pStaticRO->SetFont(&m_Font);
	pStaticStatus->SetFont(&m_Font);

	// Range officer not logged in
	SetRangeOfficerText(_T(""));
	SetStatusText(CMsg(IDS_STATUS_RO_LOGIN));
	CButton * pButton = (CButton *) this->GetDlgItem(IDC_BUTTON_TEST_SIGNIN);
	pButton->EnableWindow(FALSE);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// load default records
	Load(m_filename);

	// force load of shooter databasse and list of financial members
	ShooterDatabase * pDB = ShooterDatabaseFactory::GetShooterDatabase();
	FinancialMembers * pFM = FinancialMembers::getFinancialMembers();

	CButton * pPrintButton = (CButton *) this->GetDlgItem(IDC_BUTTON_PRINT);
	pPrintButton->EnableWindow(m_records.GetCount()>0);

	CButton * pPrintBarcodeButton = (CButton *) this->GetDlgItem(IDC_BUTTON_PRINT_BARCODE);
	// enable barcode if some records, and properties indicate we are supporting barcode printer
	pPrintBarcodeButton->EnableWindow(m_props.isBarcodeEnabled() && (m_records.GetCount()>0));

	CEdit * pFilenameEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_FILENAME);
	pFilenameEdit->SetReadOnly();

	/* 
	* set a timer to regularly return control back to the listbox so that we 
	* can receive mag stripe input
	*/
	m_signInLB.SetFocus();
	SetTimer(1, TIMER_TIME, NULL);

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CSignInRegisterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSignInRegisterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		//CDialog::OnPaint();
		CPaintDC dc(this);
		int index = m_signInLB.GetCurSel();
		if ( index != LB_ERR )
		{
			SignInRecord * pRecord = (SignInRecord *) m_signInLB.GetItemDataPtr(index);
			pRecord->getSignature().Draw(&dc, m_sig_anchor, 1, true, SIGNATURE_SCALE);
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSignInRegisterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSignInRegisterDlg::OnBnClickedButtonRangeOfficerSignin()
{
	DoRangeOfficerSignIn();
}

void CSignInRegisterDlg::SetRangeOfficerText(LPCTSTR str)
{
		CStatic * pStaticRO = (CStatic *) this->GetDlgItem(IDC_TXT_RANGE_OFFICER);
		pStaticRO->SetWindowText(str);
}

void CSignInRegisterDlg::SetStatusText(LPCTSTR str)
{
		CStatic * pStaticStatus = (CStatic *) this->GetDlgItem(IDC_TXT_STATUS);
		pStaticStatus->SetWindowText(str);
}
void CSignInRegisterDlg::OnLbnSelchangeList1()
{
	int index = m_signInLB.GetCurSel();
	if ( index != LB_ERR )
	{
		SignInRecord * pRecord = (SignInRecord *) m_signInLB.GetItemDataPtr(index);
		CDC * pDC = this->GetDC();
		pRecord->getSignature().Draw(pDC, m_sig_anchor, 1, true, SIGNATURE_SCALE);

		// enable the label print if we are using it
		if (m_props.isBarcodeEnabled()) 
		{
			CButton * pPrintBarcodeButton = (CButton *) this->GetDlgItem(IDC_BUTTON_PRINT_BARCODE);
			pPrintBarcodeButton->EnableWindow(TRUE);
		}

	}
}

void CSignInRegisterDlg::OnBnClickedButtonTestSignin()
{
	DoShooterSignIn();
}

void CSignInRegisterDlg::AddRecordToLB( 
	SignInRecord * pRecord, 
	int replacingIndex /* = -1 */,
	bool bSave /* = true */ )
{
	CString tstr = pRecord->getTimestamp().Format("%Y-%m-%d %H:%M:%S");
	CString lbstr;
	lbstr.Format( 
		_T("%s  %-25s %-20s %-20s %-5s %-25s"), 
		tstr,
		pRecord->getShooterName(),
		pRecord->getLicense(),
		pRecord->getClub(),
		pRecord->getCategory(),
		pRecord->getRangeOfficer());

	int focusIndex = -1;

	if ( replacingIndex >= 0 )
	{
		SignInRecord * pOldRecordLB = (SignInRecord *) m_signInLB.GetItemDataPtr(replacingIndex);
		m_signInLB.DeleteString(replacingIndex);
		m_signInLB.InsertString(replacingIndex, lbstr);
		m_signInLB.SetItemDataPtr(replacingIndex, pRecord);
		SignInRecord * pOldRecordRecords = (SignInRecord *) m_records.GetAt(replacingIndex);
		ASSERT(pOldRecordLB == pOldRecordRecords);
		m_records.RemoveAt(replacingIndex);
		m_records.InsertAt(replacingIndex, pRecord);
		if ( m_pCurrentRangeOfficer == pOldRecordLB )
		{
			m_pCurrentRangeOfficer = pRecord;
		}
		delete pOldRecordLB;
		focusIndex = replacingIndex;
	}
	else
	{
		int index = m_signInLB.AddString(lbstr);
		m_records.Add((void *)pRecord);
		m_signInLB.SetItemDataPtr(index, pRecord);
		focusIndex = index;
	}

	if( focusIndex >= 0 )
	{
		m_signInLB.SetCurSel(focusIndex);
		m_signInLB.SetFocus();
		OnLbnSelchangeList1();
	}

	if ( bSave)
	{
		Save();

		// add or update this shooters details in the shooter database
		ShooterDatabase * pDB = ShooterDatabaseFactory::GetShooterDatabase();
		ShooterRecord sr(
			pRecord->getMembershipNumber(),
			pRecord->getShooterName(),
			pRecord->getLicense(),
			pRecord->getClub(),
			pRecord->getCategory(),
			pRecord->getLicenseExpiry());
		pDB->Store(sr);
	}


	// if the print button was disabled, enable it now
	CButton * pPrintButton = (CButton *) this->GetDlgItem(IDC_BUTTON_PRINT);
	pPrintButton->EnableWindow(TRUE);

}

void CSignInRegisterDlg::OnLbnDblclkList1()
{
	// Figure out the current selection, and open it
		int index = m_signInLB.GetCurSel();
		if ( index != LB_ERR )
		{
			SignInRecord * pOrigRecord = (SignInRecord *) m_signInLB.GetItemDataPtr(index);
			CSignInDlg dlg(m_props, CMsg(IDS_TITLE_EDIT_RECORD), pOrigRecord);

			if ( dlg.DoModal() == IDOK )
			{
				// first manage the sign in record
				SignInRecord * pNewRecord = new SignInRecord(
					dlg.m_membership_number,
					dlg.m_shooter_name,
					dlg.m_license,
					dlg.m_club,
					dlg.m_cat_string,
					&dlg.m_sig,
					pOrigRecord->getRangeOfficer(),
					dlg.getLicenseExpiry());

				// update the record with the new data
				AddRecordToLB(pNewRecord, index, true);
			}
		}
}

void CSignInRegisterDlg::Load(LPCTSTR filename)
{
	CFile f;
	CStringA alldata;
	CStringA line;
	int curPos = 0;
	BYTE buffer[4096];
	DWORD dwRead;

	CWaitCursor wait;
	
	if (f.Open(filename, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareExclusive))
	{
		// remove current records
		ClearListBoxAndRecords();
		m_filename = filename;
		CEdit * pFilenameEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_FILENAME);
		pFilenameEdit->SetWindowText(m_filename);

		do
		{
			dwRead = f.Read(buffer, 4096);
			if (dwRead > 0)
			{
				alldata.Append((const char *) buffer, dwRead);
			}
		}
		while (dwRead > 0);

		if ( alldata.GetLength() > 0 )
		{
			// tokenize into lines and add a record for each line
			do
			{
				line = alldata.Tokenize("\r\n", curPos);
				if ( curPos >= 0 )
				{
					SignInRecord * pRecord = new SignInRecord();
					pRecord->FromAsciiString(line);
					AddRecordToLB(pRecord, -1, false);
				}
			} while ( curPos >= 0 );
		}
		f.Close();
	}
	this->RedrawWindow();
}

void CSignInRegisterDlg::Save()
{
	CWaitCursor wait;
	CFile f;

	if ( f.Open(
		m_filename, 
		CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite) )
	{
		for ( int i = 0; i < m_records.GetCount(); i++ )
		{
			SignInRecord * pRecord = (SignInRecord *) m_records.GetAt(i);
			CStringA line = pRecord->ToAsciiString();
			line += "\r\n";
			f.Write(line, line.GetLength());
		}
		f.Close();
	}
}
void CSignInRegisterDlg::OnBnClickedButtonPrint()
{
	MyPrintManager pm(m_records, m_filename);
	pm.PrintRecords();
}

void CSignInRegisterDlg::OnBnClickedButtonPrintBarcode()
{
	// determine membership number of currently selected record
	int index = m_signInLB.GetCurSel();
	if ( index != LB_ERR )
	{
		SignInRecord * pOrigRecord = (SignInRecord *) m_signInLB.GetItemDataPtr(index);
		MyPrintBarcodeManager pm(pOrigRecord->getMembershipNumber(), m_props);
		pm.PrintBarcode();
	}
}

void CSignInRegisterDlg::ClearListBoxAndRecords()
{
	// first remove all from listbox
	m_signInLB.ResetContent();
	ClearAllRecords();
}

void CSignInRegisterDlg::ClearAllRecords()
{
	while ( m_records.GetSize() > 0 )
	{
		SignInRecord * pRecord = (SignInRecord *) m_records.GetAt(0);
		m_records.RemoveAt(0);
		delete pRecord;
	}
}
void CSignInRegisterDlg::OnBnClickedButtonBrowse()
{
	// logout the current range officer if logged in
	if ( m_bRangeOfficerSignedIn )
	{
		OnBnClickedButtonRangeOfficerSignin();
	}

	if ( !m_bRangeOfficerSignedIn )
	{
		CString szFilters("SignIn Registers (*.sir)|*.sir|All Files (*.*)|*.*||");
		CFileDialog fd(
			TRUE, 
			_T("sir"), 
			m_filename,
			OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, 
			szFilters,
			this,
			0);

		if ( fd.DoModal() == IDOK )
		{
			CString filename = fd.GetPathName();

			Load(filename);
		}
	}
}

void CSignInRegisterDlg::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent == 1 )
	{
		CWnd * pWnd = GetFocus();
		if ( pWnd != &m_signInLB)
		{
			m_signInLB.SetFocus();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CSignInRegisterDlg::OnBnClickedOk()
{
	OnOK();
}

void CSignInRegisterDlg::OnOK()
{
	if ( CloseUpShop() )
	{
		CDialog::OnOK();
	}
}

void CSignInRegisterDlg::OnCancel()
{
	if ( CloseUpShop() )
	{
		CDialog::OnCancel();
	}
}

bool CSignInRegisterDlg::CloseUpShop()
{
	KillTimer(1);
	if ( MessageBox( 
		CMsg(IDS_REALLY_EXIT),
		CMsg(IDS_MB_WARNING), 
		MB_YESNO | MB_ICONWARNING ) != IDYES ) 
	{
		SetTimer(1, TIMER_TIME, NULL);
		return false;
	}
	return true;
}

LRESULT CSignInRegisterDlg::OnCardSwipe(WPARAM wParam, LPARAM lParam)
{
	CStringA * pData = (CStringA *) lParam;
	CardData cd(*pData);
	MyDelete md(pData);

	// now determine if we are going to use this card data
	if ( cd.isValid() )
	{
		// do we know who this is?
		ShooterDatabase * pDB = ShooterDatabaseFactory::GetShooterDatabase();
		CString acctnum;
		acctnum += cd.getAccountNumber(); // CStringA to CString
		const ShooterRecord * pRecord = pDB->Lookup(acctnum);

		ShooterRecord dummyRecord;
		dummyRecord.setMembershipNumber(acctnum);
		dummyRecord.setShooterName(cd.getDisplayName());
		dummyRecord.setClub(m_props.getDefaultClub());

		if ( pRecord == NULL )
		{
			// shooter not known to us - check if they want to proceed
			if ( MessageBox( 
				CFMsg(IDS_SHOOTER_UNKNOWN, acctnum, cd.getDisplayName()), 
				CMsg(IDS_MB_WARNING), 
				MB_YESNO | MB_ICONEXCLAMATION) != IDYES )
			{
				return 0;
			}

			// carry details from card to dialog box 
			pRecord = &dummyRecord;
		}

		// check if this shooter is financial
		FinancialMembers * pFM = FinancialMembers::getFinancialMembers();
		if ( !pFM->IsFinancial(pRecord->getMembershipNumber()) )
		{
			if ( MessageBox(CFMsg(IDS_NOT_FINANCIAL, pRecord->getMembershipNumber()), CMsg(IDS_MB_WARNING), MB_YESNO | MB_ICONWARNING) != IDYES )
			{
				return 0;
			}
			pFM->SetFinancial(pRecord->getMembershipNumber(), true);
		}

		// if the range officer is not signed in, offer it to this person
		if ( !m_bRangeOfficerSignedIn )
		{
			if ( MessageBox( 
				CMsg(IDS_SIGNIN_AS_RANGE_OFFICER), 
				CMsg(IDS_MB_WARNING), 
				MB_YESNO | MB_ICONEXCLAMATION) != IDYES )
			{
				return 0;
			}

			// do range officer signin
			DoRangeOfficerSignIn(pRecord);
		}
		else
		{
			// this is a regular shooter signin
			DoShooterSignIn(pRecord);
		}
	}
	else
	{
		MessageBox(CMsg(IDS_INVALID_CARD), CMsg(IDS_MB_ERROR), MB_OK | MB_ICONERROR);
	}

	return 0;
}

void CSignInRegisterDlg::DoRangeOfficerSignIn(const ShooterRecord * pRecord /* = NULL */)
{
	// If there is currently a range officer signed in, this will sign them out, so pop up a warning
	CButton * pROButton = (CButton *) this->GetDlgItem(IDC_BUTTON_RANGE_OFFICER_SIGNIN);
	CButton * pShooterSigninButton = (CButton *) this->GetDlgItem(IDC_BUTTON_TEST_SIGNIN);
	if ( m_bRangeOfficerSignedIn )
	{
		if ( MessageBox( 
			CFMsg(IDS_MB_LOGOUT, m_pCurrentRangeOfficer->getShooterName()),
			CMsg(IDS_MB_WARNING), 
			MB_YESNO | MB_ICONWARNING ) != IDYES ) {
			return;
		}

		m_bRangeOfficerSignedIn = false;
		pROButton->SetWindowText(CMsg(IDS_BTN_LOGIN));
		SetRangeOfficerText(_T(""));
		SetStatusText(CMsg(IDS_STATUS_RO_LOGIN));
		pShooterSigninButton->EnableWindow(FALSE);

	} else {
		CSignInDlg * pDlg = NULL;
		MyDelete md(pDlg);
		if ( pRecord != NULL )
		{
			pDlg = new CSignInDlg(
				m_props,
				CMsg(IDS_TITLE_RO_SIGNIN),
				pRecord->getMembershipNumber(),
				pRecord->getShooterName(),
				pRecord->getLicense(),
				pRecord->getClub(),
				pRecord->getCategory(),
				NULL,
				pRecord->getLicenseExpiryTime());
		}
		else
		{
			pDlg = new CSignInDlg(m_props, CMsg(IDS_TITLE_RO_SIGNIN), NULL, NULL, NULL, m_props.getDefaultClub(), NULL);
		}

		if ( pDlg->DoModal() == IDOK )
		{
			// first manage the sign in record

			// add a * to the beginning of the range officer name to signal that this person signed in as 
			// the range officer
			CString rangeOfficer(_T("*"));
			rangeOfficer += pDlg->m_shooter_name;
			SignInRecord * pRecord = new SignInRecord(
				pDlg->m_membership_number,
				pDlg->m_shooter_name,
				pDlg->m_license,
				pDlg->m_club,
				pDlg->m_cat_string,
				&pDlg->m_sig,
				rangeOfficer,
				pDlg->getLicenseExpiry());

			// now add to list box
			AddRecordToLB( pRecord, -1, true );

			// now deal with range office signin logic
			m_bRangeOfficerSignedIn = true;
			m_pCurrentRangeOfficer = pRecord;
			SetRangeOfficerText(pRecord->getShooterName());
			pROButton->SetWindowText(CMsg(IDS_BTN_LOGOUT));
			SetStatusText(CMsg(IDS_STATUS_SIGNIN));
			pShooterSigninButton->EnableWindow(TRUE);
		}
	}
}

void CSignInRegisterDlg::DoShooterSignIn(const ShooterRecord * pRecord /* = NULL */)
{
	CSignInDlg * pDlg = NULL;
	MyDelete md(pDlg);

	if ( pRecord != NULL )
	{
		pDlg = new CSignInDlg(
			m_props,
			CMsg(IDS_TITLE_SHOOTER_SIGNIN),
			pRecord->getMembershipNumber(),
			pRecord->getShooterName(),
			pRecord->getLicense(),
			pRecord->getClub(),
			pRecord->getCategory(),
			NULL,
			pRecord->getLicenseExpiryTime());
	}
	else
	{
		pDlg = new CSignInDlg(m_props, CMsg(IDS_TITLE_SHOOTER_SIGNIN), NULL, NULL, NULL, m_props.getDefaultClub(), NULL);
	}

	if ( pDlg->DoModal() == IDOK )
	{
		// first manage the sign in record
		SignInRecord * pRecord = new SignInRecord(
			pDlg->m_membership_number,
			pDlg->m_shooter_name,
			pDlg->m_license,
			pDlg->m_club,
			pDlg->m_cat_string,
			&pDlg->m_sig,
			m_pCurrentRangeOfficer->getShooterName(),
			pDlg->getLicenseExpiry());

		// now add to list box
		AddRecordToLB(pRecord, -1, true);
	}
}
