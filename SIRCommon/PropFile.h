#pragma once

class PropFile
{
public:
	PropFile(LPCTSTR filename);
public:
	virtual ~PropFile(void);

	CString getProperty(LPCTSTR name);

private:
	void Load();

private:
	CMapStringToString m_props;
	CString m_filename;
};
