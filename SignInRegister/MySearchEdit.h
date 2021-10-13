#pragma once
#include "afxwin.h"

class MySearchEdit :
	public CEdit
{
public:
	MySearchEdit(CWnd * pParent = NULL);
public:
	~MySearchEdit(void);
	void SetParent(CWnd * pParent) { m_pParent = pParent; }

protected:
	BOOL PreTranslateMessage(MSG * pMsg);

private:
	CWnd * m_pParent;
};
