#include "StdAfx.h"
#include "MyPrintBarcodeManager.h"
#include "SignInRegister.h"
#include "Msg.h"
#include "Resource.h"
#include "SignInRecord.h"

#define BARCODE_FONT_HEIGHT_MM 10
#define TEXT_FONT_HEIGHT_MM 4
#define TEXT_GAP_MM 1

// this is because the printer is reporting 44mm width instead if 51mm which is the actual label.
#define BARCODE_DRAG_LEFT_OFFSET_MM 3

MyPrintBarcodeManager::MyPrintBarcodeManager(LPCTSTR membership_number, const SIRProps& props)
	: m_membership_number(membership_number),
	m_props(props),
	m_hDevMode(NULL),
	m_hDevNames(NULL)
{
}

MyPrintBarcodeManager::~MyPrintBarcodeManager(void)
{
	if (m_hDevMode != NULL) 
	{
		GlobalFree(m_hDevMode);
		m_hDevMode = NULL;
	}

	if (m_hDevNames != NULL)
	{
		GlobalFree(m_hDevNames);
		m_hDevNames = NULL;
	}
}

void MyPrintBarcodeManager::PrintBarcode()
{
	// set to non-default printer without changing default app printer
	CString printerName = m_props.getBarcodePrinterName();
	LPTSTR lpszData = new TCHAR[printerName.GetLength()+1];
	wcscpy_s(lpszData, printerName.GetLength()+1, printerName);
	BOOL printerDeviceOk = theApp.GetPrinterDevice(lpszData, &m_hDevNames, &m_hDevMode);
	delete[] lpszData;

	if (!printerDeviceOk)
	{
		CString msg;
		msg.Format(_T("GetPrinterDevice called failed for printer: %s"), 
			printerName);
		MessageBox(NULL, msg, _T("Debug"), MB_OK | MB_SYSTEMMODAL);
		return;
	}

	CPrintDialog printDlg(FALSE);
	printDlg.m_pd.hDevMode = m_hDevMode;
	printDlg.m_pd.hDevNames = m_hDevNames;

	// set to landscape
	DEVMODE * pDevMode = printDlg.GetDevMode();
	pDevMode->dmFields |= DM_ORIENTATION;
	pDevMode->dmOrientation = DMORIENT_LANDSCAPE;

	CPrintInfo printInfo;
	printInfo.m_pPD = &printDlg;

	CDC dc;
    dc.Attach(printInfo.m_pPD->CreatePrinterDC());     // Create and attach a printer DC

	// determine if we can print on this page
	if (!GetAndCheckDeviceCapabilities(&dc))
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

void MyPrintBarcodeManager::OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo)
{
    // maybe pre cache some pens or brushes, or modify the
    // properties of the DC
	m_pPrintPen = new CPen(PS_SOLID, 8, RGB(0,0,0));
	m_pOldPrintPen = pDC->SelectObject(m_pPrintPen);

	m_pOldFont = NULL;

	// font for the barcode
	m_pBarcodeFont = new CFont();
	if (m_pBarcodeFont->CreateFont(
		BARCODE_FONT_HEIGHT_MM*m_vdps, /* height */
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
		m_props.getBarcodeFontName() /* facename */
		)) 
	{
		// select barcode font as we'll use it first
		m_pOldFont = pDC->SelectObject(m_pBarcodeFont);
	}

	// font for the membership number
	m_pTextFont = new CFont();
	if (!m_pTextFont->CreateFont(
		TEXT_FONT_HEIGHT_MM*m_vdps, /* height */
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
		))
	{
		// if cannot create, use default font
		delete m_pTextFont;
		m_pTextFont = NULL;
	}

	// maximum number of pages
	pInfo->SetMaxPage(1);
}

void MyPrintBarcodeManager::OnPrint(CDC *pDC, CPrintInfo* pInfo)
{
	CString barcodeString("*");
	barcodeString += m_membership_number;
	barcodeString += "*";

	const int OVERALL_PRINTING_HEIGHT_MM = BARCODE_FONT_HEIGHT_MM + TEXT_GAP_MM + TEXT_FONT_HEIGHT_MM;
	const int TOP_BORDER_MM = (m_height_mm - OVERALL_PRINTING_HEIGHT_MM) / 2;

	CSize barcodeSZ = pDC->GetOutputTextExtent(barcodeString);

	int bc_x = ((m_width_bits - barcodeSZ.cx) / 2) - (BARCODE_DRAG_LEFT_OFFSET_MM * m_hdps);
	if (bc_x < 0) 
	{
		bc_x = 0;
	}
	int bc_y = TOP_BORDER_MM*m_vdps;

	pDC->TextOut(bc_x, bc_y, barcodeString);


	pDC->SelectObject(m_pTextFont);
	CSize textSZ = pDC->GetTextExtent(m_membership_number);
	int txt_x = ((m_width_bits - textSZ.cx) / 2) - (BARCODE_DRAG_LEFT_OFFSET_MM * m_hdps);
	if (txt_x < 0)
	{
		txt_x = 0;
	}
	int txt_y = bc_y + barcodeSZ.cy + (TEXT_GAP_MM * m_vdps);
	if (txt_y + textSZ.cy < m_height_bits)
	{
		pDC->TextOut(txt_x, txt_y, m_membership_number);
	}


}

void MyPrintBarcodeManager::OnEndPrinting(CDC *pDC, CPrintInfo* pInfo)
{
    // Clean up pens or brushes, or restore the DC
	pDC->SelectObject(m_pOldPrintPen);
	delete m_pPrintPen;
	m_pPrintPen = NULL;

	if (m_pOldFont != NULL)
	{
		pDC->SelectObject(m_pOldFont);
	}

	if (m_pBarcodeFont != NULL)
	{
		delete m_pBarcodeFont;
		m_pBarcodeFont = NULL;
	}

	if (m_pTextFont != NULL)
	{
		delete m_pTextFont;
		m_pTextFont = NULL;
	}
}

bool MyPrintBarcodeManager::GetAndCheckDeviceCapabilities(CDC * pDC)
{
	m_width_bits = pDC->GetDeviceCaps(HORZRES);
	m_height_bits = pDC->GetDeviceCaps(VERTRES);
	m_width_mm = pDC->GetDeviceCaps(HORZSIZE);
	m_height_mm = pDC->GetDeviceCaps(VERTSIZE);

	m_hdps = m_width_bits/m_width_mm;
	m_vdps = m_height_bits/m_height_mm;

	// The minimum label width and height in mm that we will support
	m_min_label_width = 40;
	m_min_label_height = BARCODE_FONT_HEIGHT_MM + TEXT_GAP_MM + TEXT_FONT_HEIGHT_MM + 2;
	if ( m_width_mm < m_min_label_width || m_height_mm < m_min_label_height )
	{
		CString widthstr;
		CString heightstr;
		CString min_widthstr;
		CString min_heightstr;
		widthstr.Format(_T("%d"), m_width_mm);
		heightstr.Format(_T("%d"), m_height_mm);
		min_widthstr.Format(_T("%d"), m_min_label_width);
		min_heightstr.Format(_T("%d"), m_min_label_height);
		MessageBox(NULL, CFMsg(IDS_ERR_LABEL_RESOLUTION, widthstr, heightstr, min_widthstr, min_heightstr), CMsg(IDS_MB_ERROR), MB_OK | MB_SYSTEMMODAL);
		return false;
	}
	return true;
}
