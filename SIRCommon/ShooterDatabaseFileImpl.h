#pragma once
#include "shooterdatabase.h"

class ShooterDatabaseFileImpl :
	public ShooterDatabase
{
public:
	ShooterDatabaseFileImpl(void);
public:
	virtual ~ShooterDatabaseFileImpl(void);

	virtual const ShooterRecord * Lookup(LPCTSTR membership_number);

	virtual void Store(const ShooterRecord& shooter_record, bool bSave = true);

	virtual void RemoveAll(bool bSave = true);

	virtual void Remove(LPCTSTR membership_number, bool bSave = true);

	virtual void SearchNames( LPCTSTR searchstr, CPtrArray &records );

	virtual void Save();
private:
	void Load();
	void Empty();

private:
	CMapStringToPtr m_records;
	CString m_filename;
};
