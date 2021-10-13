#pragma once

class ShooterRecord
{
public:
	ShooterRecord(
		LPCTSTR membership_number = NULL,
		LPCTSTR shooter_name = NULL,
		LPCTSTR license = NULL,
		LPCTSTR club = NULL,
		LPCTSTR category = NULL,
		__time64_t license_expiry_time = 0);

	ShooterRecord( const ShooterRecord& rec);

	bool operator==(const ShooterRecord& rec) const;

	const CString& getMembershipNumber() const { return m_membership_number; }
	const CString& getShooterName() const { return m_shooter_name; }
	const CString& getLicense() const { return m_license; }
	const CString& getClub() const { return m_club; }
	const CString& getCategory() const { return m_category; }
	__time64_t getLicenseExpiryTime() const { return m_license_expiry_time; }

	void setMembershipNumber(LPCTSTR membership_number) { m_membership_number = membership_number; }
	void setShooterName(LPCTSTR shooter_name) { m_shooter_name = shooter_name; }
	void setLicense(LPCTSTR license) { m_license = license; }
	void setClub(LPCTSTR club) { m_club = club; }
	void setCategory(LPCTSTR category) { m_category = category; }
	void setLicenseExpiryTime(__time64_t license_expiry_time) { m_license_expiry_time = license_expiry_time; }

	CStringA ToAsciiString() const;

	void FromAsciiString(const char * str);

	CString ToSearchResultString() const;

public:
	~ShooterRecord(void);

private:
	CString m_membership_number;
	CString m_shooter_name;
	CString m_license;
	CString m_club;
	CString m_category;
	__time64_t m_license_expiry_time;

};
