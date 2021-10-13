#pragma once

class FinancialMembers
{
public:
	FinancialMembers(LPCTSTR filename);
public:
	~FinancialMembers(void);

	bool IsFinancial(LPCTSTR membership_number);
	void SetFinancial(LPCTSTR membership_number, bool bFinancial);

	static FinancialMembers * getFinancialMembers();

private:
	void Save();
	void Load();
	void Empty();

private:
	CMapStringToString m_records;
	CString m_filename;
	static FinancialMembers * m_theFinancialMembers;
};
