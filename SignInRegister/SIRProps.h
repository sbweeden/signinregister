#pragma once

#include "PropFile.h"

class SIRProps
{
public:
	SIRProps();

	bool isBarcodeEnabled() const { return m_barcodeEnabled; }
	CString getBarcodePrinterName() const { return m_barcodePrinterName; }
	CString getBarcodeFontName() const { return m_barcodeFontName; }
	CString getDefaultClub() const { return m_defaultClub; }
	int getExpiryWarningDays() const { return m_expiryWarningDays; }

public:
	virtual ~SIRProps(void);

private:
	PropFile m_props;
	bool m_barcodeEnabled;
	CString m_barcodePrinterName;
	CString m_barcodeFontName;
	CString m_defaultClub;
	int m_expiryWarningDays;
};
