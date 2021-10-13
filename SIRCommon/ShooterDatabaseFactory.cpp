#include "StdAfx.h"
#include "ShooterDatabaseFactory.h"

#include "ShooterDatabaseFileImpl.h"

ShooterDatabase * ShooterDatabaseFactory::m_theDatabase = NULL;

ShooterDatabaseFactory::ShooterDatabaseFactory(void)
{
}

ShooterDatabaseFactory::~ShooterDatabaseFactory(void)
{
}

ShooterDatabase * ShooterDatabaseFactory::GetShooterDatabase()
{
	if ( !m_theDatabase )
	{
		m_theDatabase = new ShooterDatabaseFileImpl();
	}
	return m_theDatabase;
}

