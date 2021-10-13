#include "StdAfx.h"
#include "SignInRecord.h"
#include "resource.h"
#include "Msg.h"

#define SEPARATOR ";"

SignInRecord::SignInRecord(
	LPCTSTR membership_number /* = NULL */, 
	LPCTSTR shooter_name /* = NULL */, 
	LPCTSTR license /* = NULL */, 
	LPCTSTR club /* = NULL */, 
	LPCTSTR category /* = NULL */, 
	const Signature * signature /* = NULL */, 
	LPCTSTR range_officer /* = NULL */,
	__time64_t license_expiry)
: m_timestamp(CTime::GetCurrentTime()),
  m_membership_number(membership_number),
  m_shooter_name(shooter_name),
  m_license(license),
  m_club(club),
  m_category(category),
  m_signature(signature),
  m_range_officer(range_officer),
  m_license_expiry(license_expiry)
{
}

SignInRecord::~SignInRecord(void)
{
}

CStringA SignInRecord::ToAsciiString() const
{
	CStringA result;
	char buf[256];
	CString tstr = m_timestamp.Format("%Y-%m-%d %H:%M:%S");

	result += tstr;
	result += SEPARATOR;
	result += m_membership_number;
	result += SEPARATOR;
	result += m_shooter_name;
	result += SEPARATOR;
	result += m_license;
	result += SEPARATOR;
	result += m_club;
	result += SEPARATOR;
	result += m_category;
	result += SEPARATOR;
	result += m_range_officer;
	result += SEPARATOR;

	result += m_signature.ToAsciiString();
	result += SEPARATOR;

	if (!_i64toa_s(m_license_expiry, buf, 256, 10)) {
		result += buf;
	}

	return result;
}

void SignInRecord::FromAsciiString(const char * str)
{
	CStringA s(str);
	CStringA token;
	int curPos = 0;
	bool parseError = false;

	// first scan timestamp
	token = s.Tokenize(SEPARATOR, curPos);
	if ( curPos > 0 )
	{
		int year, month, day, hour, minute, second;
		if ( sscanf( token, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6 )
		{
			CTime timestamp(year, month, day, hour, minute, second);
			m_timestamp = timestamp;
		} 
		else 
		{
			parseError = true;
		}
	}
	else
	{
		parseError = true;
	}

	if ( parseError )
	{
		CString wholestr(str);
		::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Timestamp"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
	}

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
		// parse range officer
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_range_officer = token;
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Range Officer"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
		}
	}

	if ( !parseError )
	{
		// parse signature
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_signature.FromAsciiString(token);
		}
		else
		{
			parseError = true;
			CString wholestr(str);
			::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_RECORD, _T("Signature"), wholestr ), CMsg(IDS_MB_ERROR), MB_OK);
		}
	}

	if ( !parseError )
	{
		// parse license expiry
		token = s.Tokenize(SEPARATOR, curPos);
		if ( curPos > 0 )
		{
			m_license_expiry = _atoi64((const char *) token);
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
