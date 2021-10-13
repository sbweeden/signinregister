#pragma once
#include "afxwin.h"
#include "MySearchListBox.h"
#include "Resource.h"
#include "MySearchEdit.h"
#include "ShooterRecord.h"


// CLookupDialog dialog

class CLookupDialog : public CDialog
{
	DECLARE_DYNAMIC(CLookupDialog)

public:
	CLookupDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLookupDialog();

// Dialog Data
	enum { IDD = IDD_LOOKUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
public:
	CString m_search_text;
public:
	CFont m_FontLB;
	MySearchListBox m_searchLB;
	ShooterRecord * m_selectedRecord;
public:
	MySearchEdit m_searchEdit;
public:
	afx_msg void OnLbnSelchangeListSearchRecords();
public:
	afx_msg void OnLbnDblclkListSearchRecords();
public:
	afx_msg void OnBnClickedOk();

};
