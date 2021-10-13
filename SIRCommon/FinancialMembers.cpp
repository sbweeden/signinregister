#include "StdAfx.h"
#include "FinancialMembers.h"

FinancialMembers * FinancialMembers::m_theFinancialMembers = NULL;

FinancialMembers::FinancialMembers(LPCTSTR filename)
: m_filename(filename)
{
	Load();
}

FinancialMembers::~FinancialMembers(void)
{
}

FinancialMembers * FinancialMembers::getFinancialMembers()
{
	if ( m_theFinancialMembers == NULL )
	{
		m_theFinancialMembers = new FinancialMembers(_T("c:\\signinregister\\financial_members.txt"));
	}
	return m_theFinancialMembers;
}

bool FinancialMembers::IsFinancial(LPCTSTR membership_number)
{
	CString str;
	return (m_records.Lookup(membership_number, str) == TRUE);
}

void FinancialMembers::SetFinancial(LPCTSTR membership_number, bool bFinancial)
{
	CString str;
	BOOL exists = m_records.Lookup(membership_number, str);

	if ( bFinancial && !exists )
	{
		m_records.SetAt(membership_number, membership_number);
		Save();
	}
	else if ( !bFinancial && exists )
	{
		m_records.RemoveKey(membership_number);
		Save();
	}
}

void FinancialMembers::Empty()
{
	m_records.RemoveAll();
}

void FinancialMembers::Save()
{
	CWaitCursor wait;
	CFile f;
	if ( f.Open(
		m_filename, 
		CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite) )
	{
		POSITION pos = NULL;
		CString key;
		CString value;

		for( pos = m_records.GetStartPosition(); pos != NULL; )
		{
			m_records.GetNextAssoc( pos, key, value );
			CStringA line_ascii;
			line_ascii += key;
			line_ascii += "\r\n";
			f.Write(line_ascii, line_ascii.GetLength());			
		}

		f.Close();
	}
}

void FinancialMembers::Load()
{
	CWaitCursor wait;
	CFile f;
	CStringA alldata;
	CStringA line;
	int curPos = 0;
	BYTE * buffer = new BYTE[65536];
	DWORD dwRead;
	
	if (f.Open(m_filename, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareExclusive))
	{
		// remove current records
		Empty();

		do
		{
			dwRead = f.Read(buffer, 65536);
			if (dwRead > 0)
			{
				alldata.Append((const char *) buffer, dwRead);
			}
		}
		while (dwRead > 0);

		if ( alldata.GetLength() > 0 )
		{
			// tokenize into lines and add a record for each line
			do
			{
				line = alldata.Tokenize("\r\n", curPos);
				if ( curPos >= 0 )
				{
					CString wline;
					wline = line;
					m_records.SetAt(wline, wline);
				}
			} while ( curPos >= 0 );
		}
		f.Close();
	}
	delete buffer;
}
