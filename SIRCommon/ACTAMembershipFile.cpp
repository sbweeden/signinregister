#include "StdAfx.h"
#include "ACTAMembershipFile.h"
#include "ACTARecord.h"

ACTAMembershipFile::ACTAMembershipFile(LPCTSTR filename)
: m_filename(filename)
{
	Load();
}

ACTAMembershipFile::~ACTAMembershipFile(void)
{
}

void ACTAMembershipFile::Load()
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
					ACTARecord * pRecord = new ACTARecord();
					pRecord->FromAsciiString(line, false);
					m_records.SetAt(pRecord->getMembershipNumber(), pRecord);
				}
			} while ( curPos >= 0 );
		}
		f.Close();
	}
	delete buffer;
}

void ACTAMembershipFile::Empty()
{
	POSITION pos;
	CString key;
	ACTARecord * pRecord;

	for( pos = m_records.GetStartPosition(); pos != NULL; )
	{
		m_records.GetNextAssoc( pos, key, (void*&)pRecord );
		delete pRecord;
	}

	m_records.RemoveAll();
}
