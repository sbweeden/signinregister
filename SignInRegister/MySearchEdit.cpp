#include "StdAfx.h"
#include "MySearchEdit.h"
#include "LookupDialog.h"

MySearchEdit::MySearchEdit(CWnd * pParent /* = NULL */)
: m_pParent(pParent)
{
}

MySearchEdit::~MySearchEdit(void)
{
}

BOOL MySearchEdit::PreTranslateMessage(MSG * pMsg)
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			((CLookupDialog *) m_pParent)->OnBnClickedButtonSearch();

			// swallow the return
			return TRUE;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}
