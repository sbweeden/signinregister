#include "StdAfx.h"
#include "ShooterDatabaseFileImpl.h"

ShooterDatabaseFileImpl::ShooterDatabaseFileImpl(void)
	: m_filename(_T("c:\\signinregister\\shooterdatabase.sdb"))
{
	Load();
}

ShooterDatabaseFileImpl::~ShooterDatabaseFileImpl(void)
{
	Empty();
}

const ShooterRecord * ShooterDatabaseFileImpl::Lookup(LPCTSTR membership_number)
{
	ShooterRecord * pRecord = NULL;
	m_records.Lookup(membership_number, (void*&) pRecord);
	return pRecord;
	
}

void ShooterDatabaseFileImpl::Store(const ShooterRecord& shooter_record, bool bSave /* = true */ )
{
	// figure out if a new or updated record is necessary
	bool newRecordNeeded = true;
	ShooterRecord * pOldRecord = NULL;
	m_records.Lookup(shooter_record.getMembershipNumber(), (void*&) pOldRecord);

	if ( pOldRecord )
	{
		if ( shooter_record == (*pOldRecord) )
		{
			newRecordNeeded = false;
		}
		else
		{
			// delete the old record now since we are going to replace it
			delete pOldRecord;
			pOldRecord = NULL;
		}
	}

	if ( newRecordNeeded )
	{
		ShooterRecord * pRecord = new ShooterRecord(shooter_record);
		m_records.SetAt(shooter_record.getMembershipNumber(), pRecord);

		if ( bSave )
		{
			Save();
		}
	}
}

void ShooterDatabaseFileImpl::RemoveAll(bool bSave /* = true */)
{
	Empty();
	if ( bSave )
	{
		Save();
	}
}

void ShooterDatabaseFileImpl::Remove(LPCTSTR membership_number, bool bSave /* = true */ )
{
	ShooterRecord * pRecord = NULL;
	m_records.Lookup(membership_number, (void*&) pRecord);
	if ( pRecord )
	{
		m_records.RemoveKey(membership_number);
		delete pRecord;
		pRecord = NULL;

		if ( bSave )
		{
			Save();
		}
	}
}


void ShooterDatabaseFileImpl::SearchNames(LPCTSTR searchstr, CPtrArray& records)
{
	POSITION pos;
	CString key;
	ShooterRecord * pRecord;

	for( pos = m_records.GetStartPosition(); pos != NULL; )
	{
		m_records.GetNextAssoc( pos, key, (void*&)pRecord );
		if ( pRecord->getShooterName().Find(searchstr) >= 0 )
		{
			records.Add(pRecord);
		}
	}
}

void ShooterDatabaseFileImpl::Load()
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
					ShooterRecord * pRecord = new ShooterRecord();
					pRecord->FromAsciiString(line);
					m_records.SetAt(pRecord->getMembershipNumber(), pRecord);
				}
			} while ( curPos >= 0 );
		}
		f.Close();
	}
	delete buffer;
}

void ShooterDatabaseFileImpl::Save()
{
	CWaitCursor wait;
	CFile f;
	if ( f.Open(
		m_filename, 
		CFile::shareExclusive | CFile::modeCreate | CFile::modeWrite) )
	{
		POSITION pos;
		CString key;
		ShooterRecord * pRecord;

		for( pos = m_records.GetStartPosition(); pos != NULL; )
		{
			m_records.GetNextAssoc( pos, key, (void*&)pRecord );
			CStringA line = pRecord->ToAsciiString();
			line += "\r\n";
			f.Write(line, line.GetLength());			
		}

		f.Close();
	}
}

void ShooterDatabaseFileImpl::Empty()
{
	POSITION pos;
	CString key;
	ShooterRecord * pRecord;

	for( pos = m_records.GetStartPosition(); pos != NULL; )
	{
		m_records.GetNextAssoc( pos, key, (void*&)pRecord );
		delete pRecord;
	}

	m_records.RemoveAll();
}
