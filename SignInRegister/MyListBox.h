#pragma once
#include "afxwin.h"

#define WM_CARDSWIPED_MESSAGE (WM_USER+100)

class MyListBox :
	public CListBox
{
public:
	MyListBox(CWnd * pParent = NULL);
public:
	~MyListBox(void);
	virtual void SetParent( CWnd * pParent );
protected:
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnSysCommand(UINT nID, LPARAM lParam);
	void DisplayKey(LPCTSTR title, UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL PreTranslateMessage(MSG * pMsg);
	const CStringA& GetAndClearCardData();
protected:
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CWnd * m_pParent;
	CStringA m_card_data;

};
