#pragma once

#include "SIRProps.h"

class MyPrintBarcodeManager
{
public:
	MyPrintBarcodeManager(LPCTSTR membership_number, const SIRProps& props);

	void PrintBarcode();
public:
	~MyPrintBarcodeManager(void);

private:
	void OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo);
	void OnPrint(CDC *pDC, CPrintInfo* pInfo);
	void OnEndPrinting(CDC *pDC, CPrintInfo* pInfo);
	bool GetAndCheckDeviceCapabilities(CDC * pDC);

private:
	// reference to the properties
	const SIRProps& m_props;

	// reference to the data to print
	CString m_membership_number;

	// pointers to barcode printer device information
    HGLOBAL m_hDevMode;
    HGLOBAL m_hDevNames;


	// page size information
	int m_width_bits;
	int m_height_bits;
	int m_width_mm;
	int m_height_mm;

	int m_hdps;
	int m_vdps;

	int m_min_label_width;
	int m_min_label_height;

	// printing resources
	CPen *m_pPrintPen, *m_pOldPrintPen;
	CFont *m_pBarcodeFont, *m_pTextFont, *m_pOldFont;
};
