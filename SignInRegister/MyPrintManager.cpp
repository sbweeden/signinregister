#include "StdAfx.h"
#include "MyPrintManager.h"
#include "SignInRegister.h"
#include "Msg.h"
#include "Resource.h"
#include "SignInRecord.h"

#define RECORDS_PER_PAGE 14

#define PRINTER_SIGNATURE_SCALE 1.016

MyPrintManager::MyPrintManager(CPtrArray& records, LPCTSTR filename)
	: m_records(records),
	m_filename(filename)
{
}

MyPrintManager::~MyPrintManager(void)
{
}

void MyPrintManager::PrintRecords()
{
	CDC dc;
	CPrintDialog printDlg(FALSE);

	theApp.SetPrinterOptions();

    if (theApp.DoPrintDialog(&printDlg) == IDCANCEL)     // Get printer settings from user
        return;

    dc.Attach(printDlg.GetPrinterDC());     // Get and attach a printer DC

	// determine if we can print on this page
	if ( !GetAndCheckDeviceCapabilities(&dc))
	{
		dc.DeleteDC();
		return;
	}


    dc.m_bPrinting = TRUE;

    CString strTitle;                       // Get the application title
    strTitle.LoadString(IDS_APP_TITLE);

    DOCINFO di;                             // Initialise print document details
    ::ZeroMemory (&di, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);    // Begin a new print job

    // Get the printing extents and store in the m_rectDraw field of a 
    // CPrintInfo object
    CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, 
                            dc.GetDeviceCaps(HORZRES), 
                            dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);            // Call your "Init printing" function
    for (UINT page = Info.GetMinPage(); 
         page <= Info.GetMaxPage() && bPrintingOK; 
         page++)
    {
        dc.StartPage();                     // begin new page
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);                // Call your "Print page" function
        bPrintingOK = (dc.EndPage() > 0);   // end page
    }
    OnEndPrinting(&dc, &Info);              // Call your "Clean up" function

    if (bPrintingOK)
        dc.EndDoc();                        // end a print job
    else
        dc.AbortDoc();                      // abort job.

    dc.DeleteDC();                          // delete the printer DC
}

void MyPrintManager::OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo)
{
    // maybe pre cache some pens or brushes, or modify the
    // properties of the DC
	m_pPrintPen = new CPen(PS_SOLID, 8, RGB(0,0,0));
	m_pOldPrintPen = pDC->SelectObject(m_pPrintPen);

	// maximum number of pages
	pInfo->SetMaxPage((int)((m_records.GetCount()+RECORDS_PER_PAGE-1)/RECORDS_PER_PAGE));
}

void MyPrintManager::OnPrint(CDC *pDC, CPrintInfo* pInfo)
{
    // Do your drawing/printing exactly as you would in a
    // CView::OnDraw function. The CPrintInfo structure
    // will give you the bounds and the current page number
	/*
	CString msg;
	msg.Format(_T("Left: %d Top: %d Right: %d Bottom: %d Width(mm): %d  Height(mm): %d Width(res): %d Height(res): %d"), 
		pInfo->m_rectDraw.left, 
		pInfo->m_rectDraw.top, 
		pInfo->m_rectDraw.right, 
		pInfo->m_rectDraw.bottom,
		pDC->GetDeviceCaps(HORZSIZE),
		pDC->GetDeviceCaps(VERTSIZE),
		pDC->GetDeviceCaps(HORZRES),
		pDC->GetDeviceCaps(VERTRES)
		);
	MessageBox(NULL, msg, _T("Debug"), MB_OK);
	*/

	// all these numbers are in mm - we convert them into device units at draw time
	const int BORDER_LEFT = (m_width_mm - m_a4_width) / 2;
	const int BORDER_TOP = (m_height_mm - m_a4_height) / 2 + 20;
	const int BORDER_RIGHT = BORDER_LEFT + m_a4_width;
	const int BORDER_BOTTOM = BORDER_TOP + m_a4_height - 21;

	// overall border
	pDC->Rectangle(
		BORDER_LEFT*m_hdps,
		BORDER_TOP*m_vdps, 
		BORDER_RIGHT*m_hdps, 
		BORDER_BOTTOM*m_vdps);

	// column lines
	const int TIMESTAMP_LINE_X = BORDER_LEFT+26;
	const int NAME_LINE_X = TIMESTAMP_LINE_X+63;
	const int LICENSE_LINE_X = NAME_LINE_X + 37;
	const int CLUB_LINE_X = LICENSE_LINE_X + 51;
	const int CAT_LINE_X = CLUB_LINE_X + 10;
	const int SIG_LINE_X = CAT_LINE_X + 30;

	pDC->MoveTo(TIMESTAMP_LINE_X*m_hdps, BORDER_TOP*m_vdps);
	pDC->LineTo(TIMESTAMP_LINE_X*m_hdps, BORDER_BOTTOM*m_vdps);
	pDC->MoveTo(NAME_LINE_X*m_hdps, BORDER_TOP*m_vdps);
	pDC->LineTo(NAME_LINE_X*m_hdps, BORDER_BOTTOM*m_vdps);
	pDC->MoveTo(LICENSE_LINE_X*m_hdps, BORDER_TOP*m_vdps);
	pDC->LineTo(LICENSE_LINE_X*m_hdps, BORDER_BOTTOM*m_vdps);
	pDC->MoveTo(CLUB_LINE_X*m_hdps, BORDER_TOP*m_vdps);
	pDC->LineTo(CLUB_LINE_X*m_hdps, BORDER_BOTTOM*m_vdps);
	pDC->MoveTo(CAT_LINE_X*m_hdps, BORDER_TOP*m_vdps);
	pDC->LineTo(CAT_LINE_X*m_hdps, BORDER_BOTTOM*m_vdps);
	pDC->MoveTo(SIG_LINE_X*m_hdps, BORDER_TOP*m_vdps);
	pDC->LineTo(SIG_LINE_X*m_hdps, BORDER_BOTTOM*m_vdps);

	// first row underline
	const int TITLE_LINE_Y = BORDER_TOP + 10;
	pDC->MoveTo(BORDER_LEFT*m_hdps, TITLE_LINE_Y*m_vdps);
	pDC->LineTo(BORDER_RIGHT*m_hdps, TITLE_LINE_Y*m_vdps);
	pDC->MoveTo(BORDER_LEFT*m_hdps, (TITLE_LINE_Y-1)*m_vdps);
	pDC->LineTo(BORDER_RIGHT*m_hdps, (TITLE_LINE_Y-1)*m_vdps);

	// filename and page number
	const int META_Y = BORDER_TOP - 5;
	CString metastr;
	metastr.Format(_T("Filename: %s   Page %d of %d"), m_filename, pInfo->m_nCurPage, pInfo->GetMaxPage());
	pDC->TextOut((BORDER_LEFT+2)*m_hdps, META_Y*m_vdps, metastr);

	// text for column headings
	const int TITLES_Y = BORDER_TOP + 2;

	pDC->TextOut((BORDER_LEFT+2)*m_hdps, TITLES_Y*m_vdps, CString(_T("Timestamp")));
	pDC->TextOut((TIMESTAMP_LINE_X+2)*m_hdps, TITLES_Y*m_vdps, CString(_T("Name")));
	pDC->TextOut((NAME_LINE_X+2)*m_hdps, TITLES_Y*m_vdps, CString(_T("License")));
	pDC->TextOut((LICENSE_LINE_X+2)*m_hdps, TITLES_Y*m_vdps, CString(_T("Club")));
	pDC->TextOut((CLUB_LINE_X+2)*m_hdps, TITLES_Y*m_vdps, CString(_T("Cat")));
	pDC->TextOut((CAT_LINE_X+2)*m_hdps, TITLES_Y*m_vdps, CString(_T("Signature")));
	pDC->TextOut((SIG_LINE_X+1)*m_hdps, TITLES_Y*m_vdps, CString(_T("Range Officer (* R.O. login)")));

	// print this page's records
	int start_record = (int) (RECORDS_PER_PAGE * (pInfo->m_nCurPage-1));
	int end_record = (int) min((start_record+RECORDS_PER_PAGE), m_records.GetCount());
	for ( int i = start_record; i < end_record; i++ )
	{
		const int RECORD_HEIGHT = 12;
		const int RECORD_Y = (TITLE_LINE_Y+1) + ((i-start_record)*RECORD_HEIGHT);

		SignInRecord * pRecord = (SignInRecord *) m_records.GetAt(i);

		// print the timestamp
		CString tstr1 = pRecord->getTimestamp().Format("%Y-%m-%d");
		CString tstr2 = pRecord->getTimestamp().Format("%H:%M:%S");
		pDC->TextOut((BORDER_LEFT+1)*m_hdps, RECORD_Y*m_vdps, tstr1);
		pDC->TextOut((BORDER_LEFT+1)*m_hdps, (RECORD_Y+6)*m_vdps, tstr2);

		// print the name
		pDC->TextOut((TIMESTAMP_LINE_X+1)*m_hdps, RECORD_Y*m_vdps, pRecord->getShooterName());

		// print the license
		pDC->TextOut((NAME_LINE_X+1)*m_hdps, RECORD_Y*m_vdps, pRecord->getLicense());

		// print the club
		pDC->TextOut((LICENSE_LINE_X+1)*m_hdps, RECORD_Y*m_vdps, pRecord->getClub());

		// print the category
		pDC->TextOut((CLUB_LINE_X+1)*m_hdps, RECORD_Y*m_vdps, pRecord->getCategory());

		// print the signature
		//pRecord->getSignature().Draw(pDC, CPoint((CAT_LINE_X+1)*m_hdps, RECORD_Y*m_vdps), 4, false, PRINTER_SIGNATURE_SCALE);
		pRecord->getSignature().Draw(pDC, CPoint((CAT_LINE_X)*m_hdps, (RECORD_Y-1)*m_vdps), 4, false, PRINTER_SIGNATURE_SCALE);

		// print the range office
		pDC->TextOut((SIG_LINE_X+1)*m_hdps, RECORD_Y*m_vdps, pRecord->getRangeOfficer());

		// underline if it's not the last for this page
		pDC->MoveTo(BORDER_LEFT*m_hdps, (RECORD_Y+RECORD_HEIGHT-1)*m_vdps);
		pDC->LineTo(BORDER_RIGHT*m_hdps, (RECORD_Y+RECORD_HEIGHT-1)*m_vdps);
	}
}

void MyPrintManager::OnEndPrinting(CDC *pDC, CPrintInfo* pInfo)
{
    // Clean up pens or brushes, or restore the DC
	pDC->SelectObject(m_pOldPrintPen);
	delete m_pPrintPen;
}

bool MyPrintManager::GetAndCheckDeviceCapabilities(CDC * pDC)
{
	m_width_bits = pDC->GetDeviceCaps(HORZRES);
	m_height_bits = pDC->GetDeviceCaps(VERTRES);
	m_width_mm = pDC->GetDeviceCaps(HORZSIZE);
	m_height_mm = pDC->GetDeviceCaps(VERTSIZE);

	m_hdps = m_width_bits/m_width_mm;
	m_vdps = m_height_bits/m_height_mm;

	// an A4 page in landscape mode is 297mm x 210 mm, however many printers can't use that whole area. 
	// we require 280mm x 200mm
	m_a4_width = 280; // 297;
	m_a4_height = 200; //210;
	if ( m_width_mm < m_a4_width || m_height_mm < m_a4_height )
	{
		CString widthstr;
		CString heightstr;
		widthstr.Format(_T("%d"), m_width_mm);
		heightstr.Format(_T("%d"), m_height_mm);
		MessageBox(NULL, CFMsg(IDS_ERR_PAGE_RESOLUTION, widthstr, heightstr), CMsg(IDS_MB_ERROR), MB_OK | MB_SYSTEMMODAL);
		return false;
	}
	return true;
}
