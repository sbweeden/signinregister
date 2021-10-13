#pragma once

class CardData
{
public:
	CardData(const char * data);
public:
	~CardData(void);

	bool isValid() { return m_valid; }

	const CStringA& getAccountNumber() { return m_account_number; }
	const CStringA& getName() { return m_name; }
	CString getDisplayName();

protected:
	void Parse();
	bool ValidAccountNumber(const char * str);

private:
	CStringA m_raw_data;
	bool m_valid;
	CStringA m_account_number;
	CStringA m_name;
};
