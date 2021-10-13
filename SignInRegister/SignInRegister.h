// SignInRegister.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSignInRegisterApp:
// See SignInRegister.cpp for the implementation of this class
//

class CSignInRegisterApp : public CWinApp
{
public:
	CSignInRegisterApp();
	~CSignInRegisterApp();
	void SetPrinterOptions();
	BOOL GetPrinterDevice(LPTSTR pszPrinterName, HGLOBAL* phDevNames, HGLOBAL* phDevMode);

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSignInRegisterApp theApp;