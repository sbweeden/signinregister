// MergeMembers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MergeMembers.h"
#include "ShooterDatabaseFactory.h"
#include "ACTAMembershipFile.h"
#include "ACTARecord.h"
#include "FieldLimits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

void Usage(LPCTSTR programName)
{
	_tprintf(_T("Usage: %s <actaFile>\n\n"), programName);
	_tprintf(_T("The actaFile should be a text file containing financial members with rows formatted like:\n"));
	_tprintf(_T("    ACTA_MEMBERSHIP_NUMBER;SURNAME;FIRSTNAME;CLUB\n\n"));
	_tprintf(_T("The signinRegisterFile is the shooterdatabase.sdb file from the signin register application, with rows fomatted like:\n"));
	_tprintf(_T("    ACTA_MEMBERSHIP_NUMBER;NAME;LICENSE;CLUB;CATEGORIES\n\n"));
	_tprintf(_T("The program will update the signinRegisterFile to match the set of people in the ACTA file, preserving any license and category information we have."));
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;



	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		if ( argc == 2 )
		{
			ShooterDatabase *pDB = ShooterDatabaseFactory::GetShooterDatabase();
			ACTAMembershipFile mFile(argv[1]);

			CMapStringToPtr& records = mFile.GetRecords();
			CMapStringToPtr new_shooter_database;
			CMapStringToPtr new_gcctc_records;

			CString key;
			ACTARecord * pACTARecord = NULL;
			ShooterRecord * pShooterRecord = NULL;
			int num_new_records = 0, num_skipped_records = 0;
			POSITION pos;

			// process all current ACTA records
			for( pos = records.GetStartPosition(); pos != NULL; )
			{
				records.GetNextAssoc( pos, key, (void*&)pACTARecord );
				if ( isValid(pACTARecord) )
				{
					// find out if we know anything about this person already
					const ShooterRecord * pSR = pDB->Lookup(pACTARecord->getMembershipNumber());

					// is this a new person?
					if ( !pSR )
					{
						_tprintf(_T("Adding new record for membership number: %s\n"), pACTARecord->getMembershipNumber());
						num_new_records++;
					}

					// build the record which is going to form part of our new shooter database
					pShooterRecord = new ShooterRecord(
								pACTARecord->getMembershipNumber(),
								pACTARecord->getShooterName(),
								pSR ? pSR->getLicense() : _T("0"),
								pACTARecord->getClub(),
								pSR ? pSR->getCategory() : _T("0"));

					new_shooter_database.SetAt(pShooterRecord->getMembershipNumber(), pShooterRecord);

					// if this is a NEW GCCTC member, remember them for the final report
					if ( pACTARecord->getClub() == CString(_T("GOLD COAST")) && !pSR )
					{
						new_gcctc_records.SetAt(pACTARecord->getMembershipNumber(), pACTARecord);
					}
				}
				else
				{
					num_skipped_records++;
				}
			}

			// now we have all the new records - delete the old database and create with this one.
			pDB->RemoveAll(false);
			for ( pos = new_shooter_database.GetStartPosition(); pos != NULL; )
			{
				pShooterRecord = NULL;
				new_shooter_database.GetNextAssoc( pos, key, (void*&) pShooterRecord);
				if ( pShooterRecord )
				{
					pDB->Store(*pShooterRecord, false);
				}
			}

			// now save the new database
			pDB->Save();

			_tprintf(_T("Finished. Number of new records added: %d. Number of skipped records: %d\n"), num_new_records, num_skipped_records);
			_tprintf(_T("New GOLD COAST records (total %d):\n"), new_gcctc_records.GetCount());
			for( POSITION pos = new_gcctc_records.GetStartPosition(); pos != NULL; )
			{
				pACTARecord = NULL;
				new_gcctc_records.GetNextAssoc( pos, key, (void*&)pACTARecord );
				_tprintf(_T("%s;%s;%s\n"), pACTARecord->getMembershipNumber(), pACTARecord->getSurname(), pACTARecord->getFirstname());
			}
		}
		else
		{
			Usage(argv[0]);
		}
	}

	return nRetCode;
}

bool isValid(ACTARecord * pACTARecord)
{
	bool result = false;
	CMapStringToPtr m_known_bad_records;
	void *dummyval = NULL;

	m_known_bad_records.SetAt(_T("11111"), NULL);
	m_known_bad_records.SetAt(_T("12170"), NULL);
	m_known_bad_records.SetAt(_T("12171"), NULL);
	m_known_bad_records.SetAt(_T("12175"), NULL);
	m_known_bad_records.SetAt(_T("14005"), NULL);
	m_known_bad_records.SetAt(_T("19859"), NULL);
	m_known_bad_records.SetAt(_T("20005"), NULL);
	m_known_bad_records.SetAt(_T("20011"), NULL);
	m_known_bad_records.SetAt(_T("20014"), NULL);
	m_known_bad_records.SetAt(_T("20016"), NULL);
	m_known_bad_records.SetAt(_T("20019"), NULL);
	m_known_bad_records.SetAt(_T("20031"), NULL);
	m_known_bad_records.SetAt(_T("20032"), NULL);
	m_known_bad_records.SetAt(_T("20033"), NULL);
	m_known_bad_records.SetAt(_T("20034"), NULL);
	m_known_bad_records.SetAt(_T("20035"), NULL);
	m_known_bad_records.SetAt(_T("20036"), NULL);
	m_known_bad_records.SetAt(_T("20037"), NULL);
	m_known_bad_records.SetAt(_T("20038"), NULL);
	m_known_bad_records.SetAt(_T("20039"), NULL);
	m_known_bad_records.SetAt(_T("20040"), NULL);
	m_known_bad_records.SetAt(_T("20050"), NULL);
	m_known_bad_records.SetAt(_T("20051"), NULL);
	m_known_bad_records.SetAt(_T("20052"), NULL);
	m_known_bad_records.SetAt(_T("20053"), NULL);
	m_known_bad_records.SetAt(_T("20055"), NULL);
	m_known_bad_records.SetAt(_T("20056"), NULL);
	m_known_bad_records.SetAt(_T("20057"), NULL);
	m_known_bad_records.SetAt(_T("20059"), NULL);
	m_known_bad_records.SetAt(_T("20060"), NULL);
	m_known_bad_records.SetAt(_T("23362"), NULL);
	m_known_bad_records.SetAt(_T("29172"), NULL);
	m_known_bad_records.SetAt(_T("68409"), NULL);
	m_known_bad_records.SetAt(_T("69956"), NULL);

	if ( pACTARecord )
	{
		if ( !m_known_bad_records.Lookup(pACTARecord->getMembershipNumber(), dummyval) )
		{
			if ( pACTARecord->getMembershipNumber().GetLength() == 0 ||
				pACTARecord->getMembershipNumber().GetLength() > MAX_MEMBERSHIP_LEN )
			{
				_tprintf(_T("Skipping Record - Membership number missing or too long in record: %s\n"), pACTARecord->getMembershipNumber());
			}
			else if ( pACTARecord->getShooterName().GetLength() == 0 || pACTARecord->getShooterName().GetLength() > MAX_NAME_LEN )
			{
				_tprintf(_T("Skipping Record  - Shooter name missing or too long in record: %s  Name: %s\n"), pACTARecord->getMembershipNumber(), pACTARecord->getShooterName());
			}
			else if ( pACTARecord->getClub().GetLength() == 0 || pACTARecord->getClub().GetLength() > MAX_CLUB_LEN )
			{
				_tprintf(_T("Skipping Record  - Club name missing or too long in record: %s  Club: %s\n"), pACTARecord->getMembershipNumber(), pACTARecord->getClub());
			}
			else
			{
				// passed all checks
				result = true;
			}
		}
	}

	return result;
}