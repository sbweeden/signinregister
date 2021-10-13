#pragma once

#include "ShooterDatabase.h"

class ShooterDatabaseFactory
{
public:
	ShooterDatabaseFactory(void);
public:
	~ShooterDatabaseFactory(void);

public:
	static ShooterDatabase * GetShooterDatabase();
private:
	static ShooterDatabase * m_theDatabase;
};
