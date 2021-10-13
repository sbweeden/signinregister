#pragma once

class ACTARecord
{
public:
	ACTARecord(
		LPCTSTR membership_number = NULL,
		LPCTSTR surname = NULL,
		LPCTSTR firstname = NULL,
		LPCTSTR club = NULL);

	ACTARecord( const ACTARecord& rec);

	const CString& getMembershipNumber() const { return m_membership_number; }
	const CString& getSurname() const { return m_surname; }
	const CString& getFirstname() const { return m_firstname; }
	const CString& getShooterName() const { return m_shooter_name; }
	const CString& getClub() const { return m_club; }

	void setMembershipNumber(LPCTSTR membership_number) { m_membership_number = membership_number; }
	void setShooterName(LPCTSTR shooter_name) { m_shooter_name = shooter_name; }
	void setClub(LPCTSTR club) { m_club = club; }

	CStringA ToAsciiString() const;

	void FromAsciiString(const char * str, bool report_errors_as_mb = true);

public:
	~ACTARecord(void);

private:
	void MakeShooterName();
private:
	CString m_membership_number;
	CString m_surname;
	CString m_firstname;
	CString m_shooter_name;
	CString m_club;
};
