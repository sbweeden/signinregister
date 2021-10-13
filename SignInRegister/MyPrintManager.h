#pragma once

class MyPrintManager
{
public:
	MyPrintManager(CPtrArray &records, LPCTSTR filename);

	void PrintRecords();
public:
	~MyPrintManager(void);

private:
	void OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo);
	void OnPrint(CDC *pDC, CPrintInfo* pInfo);
	void OnEndPrinting(CDC *pDC, CPrintInfo* pInfo);
	bool GetAndCheckDeviceCapabilities(CDC * pDC);

private:
	// reference to the data to print
	CPtrArray &m_records;
	CString m_filename;

	// page size information
	int m_width_bits;
	int m_height_bits;
	int m_width_mm;
	int m_height_mm;

	int m_hdps;
	int m_vdps;

	int m_a4_width;
	int m_a4_height;

	// printing resources
	CPen *m_pPrintPen, *m_pOldPrintPen;
};
