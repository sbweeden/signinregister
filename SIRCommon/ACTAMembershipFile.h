#pragma once

class ACTAMembershipFile
{
public:
	ACTAMembershipFile(LPCTSTR filename);
public:
	~ACTAMembershipFile(void);

	CMapStringToPtr& GetRecords() { return m_records; }

private:
	void Load();
	void Empty();

private:
	CMapStringToPtr m_records;
	CString m_filename;
};
