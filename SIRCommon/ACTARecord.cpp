#include "StdAfx.h"
#include "ACTARecord.h"
#include "Resource.h"
#include "Msg.h"

#define SEPARATOR ";"

ACTARecord::ACTARecord(
	LPCTSTR membership_number /* = NULL */,
	LPCTSTR surname /* = NULL */,
	LPCTSTR firstname /* = NULL */,
	LPCTSTR club /* = NULL */)
	: m_membership_number(membership_number),
	m_surname(surname),
	m_firstname(firstname),
	m_club(club)
{
	MakeShooterName();
}

ACTARecord::ACTARecord( const ACTARecord& rec)
	: m_membership_number(rec.m_membership_number),
	m_shooter_name(rec.m_shooter_name),
	m_club(rec.m_club)
{
}

ACTARecord::~ACTARecord(void)
{
}

CStringA ACTARecord::ToAsciiString() const
{
	CStringA result;
	result += m_membership_number;
	result += SEPARATOR;
	result += m_shooter_name;
	result += SEPARATOR;
	result += m_club;
	return result;
}

void ACTARecord::FromAsciiString(const char * str, bool report_errors_as_mb /* = true */)
{
	CStringA s(str);
	CStringA token;
	CStringA surname;
	CStringA firstname;
	int curPos = 0;
	bool parseError = false;

	if ( !parseError )
	{
		// parse membership number
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_membership_number = token;
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			if ( report_errors_as_mb )
			{
				::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Membership Number"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
			}
		}
	}

	if ( !parseError )
	{
		// parse surname name
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_surname = token;
			m_surname.MakeUpper();
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			if ( report_errors_as_mb )
			{
				::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Shooter Name"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
			}
		}
	}

	if ( !parseError )
	{
		// parse firstname
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_firstname = token;
			m_firstname.MakeUpper();
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			if ( report_errors_as_mb )
			{
				::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Shooter Name"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
			}
		}
	}

	if ( !parseError )
	{
		// parse club
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_club = token;
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			if ( report_errors_as_mb )
			{
				::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Club"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
			}
		}
	}

	MakeShooterName();
}


void ACTARecord::MakeShooterName()
{
	m_shooter_name = m_firstname;
	m_shooter_name += _T(" ");
	m_shooter_name += m_surname;
	m_shooter_name.MakeUpper();
}