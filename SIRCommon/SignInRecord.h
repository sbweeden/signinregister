#pragma once

#include "Signature.h"

class SignInRecord
{
public:
	SignInRecord(
		LPCTSTR membership_number = NULL, 
		LPCTSTR shooter_name = NULL, 
		LPCTSTR license = NULL, 
		LPCTSTR club = NULL, 
		LPCTSTR category = NULL, 
		const Signature * signature = NULL, 
		LPCTSTR range_officer = NULL,
		__time64_t license_expiry = 0);

	const CTime& getTimestamp() const { return m_timestamp; }

	const CString& getMembershipNumber() const { return m_membership_number; }

	const CString& getShooterName() const { return m_shooter_name; }

	const CString& getLicense() const { return m_license; }

	const CString& getClub() const { return m_club; }

	const CString& getCategory() const { return m_category; }

	Signature& getSignature() { return m_signature; }

	const CString& getRangeOfficer() const { return m_range_officer; }

	__time64_t getLicenseExpiry() { return m_license_expiry; }

	CStringA ToAsciiString() const;

	void FromAsciiString(const char * str);

public:
	virtual ~SignInRecord(void);

private:
	CTime m_timestamp;
	CString m_membership_number;
	CString m_shooter_name;
	CString m_license;
	CString m_club;
	CString m_category;
	Signature m_signature;
	CString m_range_officer;
	__time64_t m_license_expiry;
};
