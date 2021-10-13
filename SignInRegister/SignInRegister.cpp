// SignInRegister.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <winspool.h>
#include "SignInRegister.h"
#include "SignInRegisterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSignInRegisterApp

BEGIN_MESSAGE_MAP(CSignInRegisterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSignInRegisterApp construction

CSignInRegisterApp::CSignInRegisterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CSignInRegisterApp::~CSignInRegisterApp()
{
}

// The one and only CSignInRegisterApp object

CSignInRegisterApp theApp;


// CSignInRegisterApp initialization

BOOL CSignInRegisterApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CSignInRegisterDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CSignInRegisterApp::SetPrinterOptions()
{
   PRINTDLG pd;
   pd.lStructSize=(DWORD)sizeof(PRINTDLG);
   BOOL bRet=GetPrinterDeviceDefaults(&pd);
   if(bRet)
   {
      // protect memory handle with ::GlobalLock and ::GlobalUnlock
      DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(m_hDevMode);
      // set orientation to landscape
      pDevMode->dmOrientation=DMORIENT_LANDSCAPE;
	  pDevMode->dmPaperSize = DMPAPER_A4;
      ::GlobalUnlock(m_hDevMode);
   }
}

// returns a DEVMODE and DEVNAMES for the printer name specified
BOOL CSignInRegisterApp::GetPrinterDevice(LPTSTR pszPrinterName, HGLOBAL* phDevNames, HGLOBAL* phDevMode)
{
    // if NULL is passed, then assume we are setting app object's
    // devmode and devnames
    if (phDevMode == NULL || phDevNames == NULL)
        return FALSE;

    // Open printer
    HANDLE hPrinter;
    if (OpenPrinter(pszPrinterName, &hPrinter, NULL) == FALSE)
        return FALSE;

    // obtain PRINTER_INFO_2 structure and close printer
    DWORD dwBytesReturned, dwBytesNeeded;
    GetPrinter(hPrinter, 2, NULL, 0, &dwBytesNeeded);
    PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,
        dwBytesNeeded);
    if (GetPrinter(hPrinter, 2, (LPBYTE)p2, dwBytesNeeded,
       &dwBytesReturned) == 0) {
       GlobalFree(p2);
       ClosePrinter(hPrinter);
       return FALSE;
    }
    ClosePrinter(hPrinter);

    // Allocate a global handle for DEVMODE
    HGLOBAL  hDevMode = GlobalAlloc(GHND, sizeof(*p2->pDevMode) +
       p2->pDevMode->dmDriverExtra);
    ASSERT(hDevMode);
    DEVMODE* pDevMode = (DEVMODE*)GlobalLock(hDevMode);
    ASSERT(pDevMode);

    // copy DEVMODE data from PRINTER_INFO_2::pDevMode
    memcpy(pDevMode, p2->pDevMode, sizeof(*p2->pDevMode) +
       p2->pDevMode->dmDriverExtra);
    GlobalUnlock(hDevMode);

    // Compute size of DEVNAMES structure from PRINTER_INFO_2's data
    DWORD drvNameLen = lstrlen(p2->pDriverName)+1;  // driver name
    DWORD ptrNameLen = lstrlen(p2->pPrinterName)+1; // printer name
    DWORD porNameLen = lstrlen(p2->pPortName)+1;    // port name

    // Allocate a global handle big enough to hold DEVNAMES.
    HGLOBAL hDevNames = GlobalAlloc(GHND,
        sizeof(DEVNAMES) +
        (drvNameLen + ptrNameLen + porNameLen)*sizeof(TCHAR));
    ASSERT(hDevNames);
    DEVNAMES* pDevNames = (DEVNAMES*)GlobalLock(hDevNames);
    ASSERT(pDevNames);

    // Copy the DEVNAMES information from PRINTER_INFO_2
    // tcOffset = TCHAR Offset into structure
    int tcOffset = sizeof(DEVNAMES)/sizeof(TCHAR);
    ASSERT(sizeof(DEVNAMES) == tcOffset*sizeof(TCHAR));

    pDevNames->wDriverOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pDriverName,
        drvNameLen*sizeof(TCHAR));
    tcOffset += drvNameLen;

    pDevNames->wDeviceOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pPrinterName,
        ptrNameLen*sizeof(TCHAR));
    tcOffset += ptrNameLen;

    pDevNames->wOutputOffset = tcOffset;
    memcpy((LPTSTR)pDevNames + tcOffset, p2->pPortName,
        porNameLen*sizeof(TCHAR));
    pDevNames->wDefault = 0;

    GlobalUnlock(hDevNames);
    GlobalFree(p2);   // free PRINTER_INFO_2

    // set the new hDevMode and hDevNames
    *phDevMode = hDevMode;
    *phDevNames = hDevNames;
    return TRUE;
}
