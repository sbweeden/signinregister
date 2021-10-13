#include "StdAfx.h"
#include "MyListBox.h"

#define INTER_KEY_TIMEOUT 250

MyListBox::MyListBox(CWnd * pParent)
: m_pParent(pParent)
{
}

MyListBox::~MyListBox(void)
{
}

BEGIN_MESSAGE_MAP(MyListBox, CListBox)
	ON_WM_CHAR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void MyListBox::SetParent( CWnd * pParent )
{
	m_pParent = pParent;
}

afx_msg void MyListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar != 16 )
	{
		DisplayKey(_T("OnKeyDown"), nChar, nRepCnt, nFlags);
	}
}

afx_msg void MyListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(1);
	m_card_data += (char) nChar;
	SetTimer(1, INTER_KEY_TIMEOUT, NULL);
/*
	if ( m_card_data.GetLength() > 0 || (char) nChar == '%' )
	{
		m_card_data += (char) nChar;
	}
*/
}

afx_msg void MyListBox::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	DisplayKey(_T("OnSysKeyDown"), nChar, nRepCnt, nFlags);
}

afx_msg void MyListBox::OnSysCommand(UINT nID, LPARAM lParam)
{
	DisplayKey(_T("OnSysCommand"), nID, 0, 0);
}

BOOL MyListBox::PreTranslateMessage(MSG * pMsg)
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			KillTimer(1);
			if ( m_card_data.GetLength() > 0 )
			{
				CStringA * pData = new CStringA(m_card_data);
				m_card_data.Empty();
				m_pParent->SendMessage(WM_CARDSWIPED_MESSAGE, 0, (LPARAM) pData);
			}
			// swallow the return even if we didn't like the card
			return TRUE;
		}
		/*
		else if ( pMsg->wParam == VK_ESCAPE )
		{
			DisplayKey(_T("Escape key"), 0,0,0);
			return TRUE;
		}
		*/
	}
	return CListBox::PreTranslateMessage(pMsg);
}

void MyListBox::DisplayKey(LPCTSTR title, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString msg;
	msg.Format(_T("%s: nChar: %d nRepCnt: %d nFlags: %d"), title, nChar, nRepCnt, nFlags);
	MessageBox(msg, _T("Debug"), MB_OK);
}


afx_msg void MyListBox::OnTimer(UINT_PTR nIDEvent)
{
	// timeout on card data - reset
	m_card_data.Empty();
}