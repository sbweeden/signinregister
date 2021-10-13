#include "StdAfx.h"
#include "SIRProps.h"

// properties file entries
#define SIR_PROP_BARCODE_ENABLED _T("sir.enablebarcode")
#define SIR_PROP_BARCODE_PRINTER _T("sir.barcode.printername")
#define SIR_PROP_BARCODE_FONT _T("sir.barcode.font")
#define SIR_PROP_DEFAULT_CLUB _T("sir.default.club")
#define SIR_PROP_EXPIRY_WARNING_DAYS _T("sir.expiry.warning.days")

SIRProps::SIRProps()
: m_props(_T("c:\\signinregister\\signinregister.properties"))
{
	CString enabledStr = m_props.getProperty(SIR_PROP_BARCODE_ENABLED);
	enabledStr = enabledStr.MakeLower();
	m_barcodeEnabled = (enabledStr.CompareNoCase(_T("true"))==0);
	m_barcodePrinterName = m_props.getProperty(SIR_PROP_BARCODE_PRINTER);
	m_barcodeFontName = m_props.getProperty(SIR_PROP_BARCODE_FONT);
	m_defaultClub = m_props.getProperty(SIR_PROP_DEFAULT_CLUB);
	if (m_defaultClub.GetLength() <= 0) {
		m_defaultClub = _T("Gold Coast");
	}
	CString dStr = m_props.getProperty(SIR_PROP_EXPIRY_WARNING_DAYS);
	if (dStr.GetLength() > 0) {
		m_expiryWarningDays = _wtoi(dStr);
	} else {
		// default to 90 days
		m_expiryWarningDays = 90;
	}
}

SIRProps::~SIRProps(void)
{
}
