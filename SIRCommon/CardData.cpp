#include "StdAfx.h"
#include "CardData.h"

CardData::CardData(const char * data)
: m_raw_data(data),
m_valid(false)
{
	Parse();
}

CardData::~CardData(void)
{
}

void CardData::Parse()
{
	CStringA token;
	int questionPos = 0;
	int curPos = 0;
	bool parseError = false;

	// if multi-track, strip everything but track 1
	questionPos = m_raw_data.Find("?");
	if (questionPos > 0) {
		m_raw_data = m_raw_data.Left(questionPos);
	}

	if ( m_raw_data.Find("%B") == 0 )
	{
		curPos = 2; // after %B
	}
	else if ( m_raw_data.Find("%%B") == 0 )
	{
		curPos = 3; // after %%B
	}
	else if ( ValidAccountNumber(m_raw_data) )
	{
		// the card data is just the membership number
		curPos = 0;
	}
	else
	{
		parseError = true;
	}

	if ( !parseError )
	{
		// parse account number
		token = m_raw_data.Tokenize("^_", curPos); // tokenizers should be just ^ after fix to cards
		if ( curPos > 0 )
		{
			m_account_number = token;
			m_account_number.Remove(' ');

			// at this point the card data is valid, even if we don't find surname / firstname
			m_valid = true;
		}
		else
		{
			parseError = true;
		}
	}

	if ( !parseError )
	{
		// look for optional name
		token = m_raw_data.Tokenize("^", curPos);
		if ( curPos > 0 )
		{
			CStringA wholeName = token;
			wholeName.Trim(' ');
			m_name = wholeName;
			
			// now check if the name is actually surname/firstname
			int index;
			if ( (index = wholeName.Find('/')) > 0 )
			{
				m_name = wholeName.Right(wholeName.GetLength() - index - 1);
				m_name += " ";
				m_name  += wholeName.Left(index);
			}
		}
		else
		{
			parseError = true;
		}
	}
}

bool CardData::ValidAccountNumber(const char * str)
{
	bool result = false;
	if ( str != NULL )
	{
		int length = (int) strlen(str);
		if ( length > 0 )
		{
			result = true;
			for ( int i = 0; i < length; i++ )
			{
				if (!isdigit(str[i]))
				{
					result = false;
					break;
				}
			}
		}
	}
	return result;
}

CString CardData::getDisplayName()
{
	CString result;
	result += m_name;
	return result;
}