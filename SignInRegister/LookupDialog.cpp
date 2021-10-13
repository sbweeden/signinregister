// LookupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SignInRegister.h"
#include "LookupDialog.h"
#include "ShooterDatabaseFactory.h"
#include "Msg.h"

// CLookupDialog dialog

IMPLEMENT_DYNAMIC(CLookupDialog, CDialog)

CLookupDialog::CLookupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLookupDialog::IDD, pParent)
	, m_search_text(_T("")),
	m_selectedRecord(NULL)
{
	m_searchEdit.SetParent(this);
	BOOL result = m_FontLB.CreateFont(
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
}

CLookupDialog::~CLookupDialog()
{
}

void CLookupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCH_TEXT, m_search_text);
	DDX_Control(pDX, IDC_LIST_SEARCH_RECORDS, m_searchLB);
	DDX_Control(pDX, IDC_EDIT_SEARCH_TEXT, m_searchEdit);
}


BEGIN_MESSAGE_MAP(CLookupDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CLookupDialog::OnBnClickedButtonSearch)
	ON_LBN_SELCHANGE(IDC_LIST_SEARCH_RECORDS, &CLookupDialog::OnLbnSelchangeListSearchRecords)
	ON_LBN_DBLCLK(IDC_LIST_SEARCH_RECORDS, &CLookupDialog::OnLbnDblclkListSearchRecords)
	ON_BN_CLICKED(IDOK, &CLookupDialog::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CLookupDialog::OnInitDialog()
{
	CEdit * pEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_SEARCH_TEXT);
	pEdit->SetFocus();

	CButton * pButton = (CButton *) this->GetDlgItem(IDOK);
	pButton->EnableWindow(FALSE);

	UpdateData(FALSE);

	m_searchLB.SetFont(&m_FontLB);

	return FALSE;
}

// CLookupDialog message handlers

void CLookupDialog::OnBnClickedButtonSearch()
{
	CEdit * pSearchEdit = (CEdit *) this->GetDlgItem(IDC_EDIT_SEARCH_TEXT);
	pSearchEdit->GetWindowText(m_search_text);
	if ( m_search_text.GetLength() < 3 )
	{
		MessageBox(CMsg(IDS_ERR_SEARCH_TEXT_TOO_SHORT), CMsg(IDS_MB_ERROR), MB_ICONERROR | MB_OK );
		pSearchEdit->SetFocus();
		return;
	}

	// clear existing search records
	m_searchLB.ResetContent();

	CPtrArray records;
	ShooterDatabase * pDB = ShooterDatabaseFactory::GetShooterDatabase();
	pDB->SearchNames(m_search_text, records);
	if ( records.GetCount() == 0 )
	{
		MessageBox(CMsg(IDS_ERR_NO_RECORDS_FOUND), CMsg(IDS_MB_WARNING), MB_OK );
		return;
	}

	for ( int i = 0; i < records.GetCount(); i++ )
	{
		ShooterRecord * pRecord = (ShooterRecord *) records.GetAt(i);
		int index = m_searchLB.AddString(pRecord->ToSearchResultString());
		if ( index >= 0 )
		{
			m_searchLB.SetItemDataPtr(index, pRecord);
		}
	}
}

void CLookupDialog::OnLbnSelchangeListSearchRecords()
{
	// enable the OK button if an item is selected in the list box
	UpdateData(TRUE);
	CButton * pButton = (CButton *) this->GetDlgItem(IDOK);
	pButton->EnableWindow(m_searchLB.GetCurSel() != LB_ERR);
}

void CLookupDialog::OnLbnDblclkListSearchRecords()
{
	OnBnClickedOk();
}

void CLookupDialog::OnBnClickedOk()
{
	// not much to do really - just check we have data
	UpdateData(TRUE);
	if ( m_searchLB.GetCurSel() == LB_ERR )
	{
		MessageBox(CMsg(IDS_ERR_SEARCH_NOTHING_SELECTED), CMsg(IDS_MB_ERROR), MB_OK );
		m_searchLB.SetFocus();
		return;
	}

	m_selectedRecord = (ShooterRecord *) m_searchLB.GetItemDataPtr(m_searchLB.GetCurSel());

	OnOK();
}
