#include "StdAfx.h"
#include "ShooterRecord.h"
#include "Msg.h"
#include "Resource.h"

#define SEPARATOR ";"

ShooterRecord::ShooterRecord(
	LPCTSTR membership_number /* = NULL */,
	LPCTSTR shooter_name /* = NULL */,
	LPCTSTR license /* = NULL */,
	LPCTSTR club /* = NULL */,
	LPCTSTR category /* = NULL */,
	__time64_t license_expiry_time /* = 0 */)
	: m_membership_number(membership_number),
	m_shooter_name(shooter_name),
	m_license(license),
	m_club(club),
	m_category(category),
	m_license_expiry_time(license_expiry_time)
{
}

ShooterRecord::ShooterRecord( const ShooterRecord& rec)
	: m_membership_number(rec.m_membership_number),
	m_shooter_name(rec.m_shooter_name),
	m_license(rec.m_license),
	m_club(rec.m_club),
	m_category(rec.m_category),
	m_license_expiry_time(rec.m_license_expiry_time)
{
}

ShooterRecord::~ShooterRecord(void)
{
}

bool ShooterRecord::operator==(const ShooterRecord& rec) const
{
	return ( m_membership_number == rec.m_membership_number &&
		m_shooter_name == rec.m_shooter_name &&
		m_license == rec.m_license &&
		m_club == rec.m_club &&
		m_category == rec.m_category &&
		m_license_expiry_time == rec.m_license_expiry_time );
}

CStringA ShooterRecord::ToAsciiString() const
{
	CStringA result;
	char buf[256];
	result += m_membership_number;
	result += SEPARATOR;
	result += m_shooter_name;
	result += SEPARATOR;
	result += m_license;
	result += SEPARATOR;
	result += m_club;
	result += SEPARATOR;
	result += m_category;
	if (!_i64toa_s(m_license_expiry_time, buf, 256, 10)) {
		result += SEPARATOR;
		result += buf;
	}
	return result;
}

void ShooterRecord::FromAsciiString(const char * str)
{
	CStringA s(str);
	CStringA token;
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
			::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Membership Number"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
		}
	}

	if ( !parseError )
	{
		// parse shooter name
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_shooter_name = token;
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Shooter Name"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
		}
	}

	if ( !parseError )
	{
		// parse license
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_license = token;
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("License"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
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
			::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Club"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
		}
	}

	if ( !parseError )
	{
		// parse category
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_category = token;
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Category"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
		}
	}

	if ( !parseError )
	{
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_license_expiry_time = _atoi64((const char *) token);
		}
		// optional - don't consider this a failure
		//else
		//{
		//	parseError = true;
		//	CString wholestr(str);
		//	::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("License Expiry Time"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
		//}
	}
}

CString ShooterRecord::ToSearchResultString() const
{
	CString result;
	result.Format(_T("%-25s %-16s %-20s"),
		m_shooter_name,
		m_membership_number,
		m_club );

	return result;
}