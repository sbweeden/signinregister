#pragma once

#include "ShooterRecord.h"

class ShooterDatabase
{
public:
	ShooterDatabase(void);
public:
	virtual ~ShooterDatabase(void);

	virtual const ShooterRecord * Lookup(LPCTSTR membership_number) = 0;

	virtual void Store(const ShooterRecord& shooter_record, bool bSave = true) = 0;

	virtual void RemoveAll(bool bSave = true) = 0;

	virtual void Remove(LPCTSTR membership_number, bool bSave = true) = 0;

	virtual void SearchNames(LPCTSTR searchstr, CPtrArray& records) = 0;

	virtual void Save() = 0;
};
