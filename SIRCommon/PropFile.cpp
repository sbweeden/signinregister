#include "StdAfx.h"
#include "PropFile.h"

PropFile::PropFile(LPCTSTR filename)
	: m_filename(filename)
{
	Load();
}

PropFile::~PropFile(void) 
{
}

CString PropFile::getProperty(LPCTSTR name)
{
	CString result;
	m_props.Lookup(name, result);
	return result;
}

void PropFile::Load()
{
	CWaitCursor wait;
	CFile f;
	CStringA alldata;
	CStringA line;
	int curPos = 0;
	BYTE * buffer = new BYTE[65536];
	DWORD dwRead;
	
	if (f.Open(m_filename, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareExclusive))
	{
		do
		{
			dwRead = f.Read(buffer, 65536);
			if (dwRead > 0)
			{
				alldata.Append((const char *) buffer, dwRead);
			}
		}
		while (dwRead > 0);

		if ( alldata.GetLength() > 0 )
		{
			// tokenize into lines and add a property for each line that isn't a comment
			do
			{
				line = alldata.Tokenize("\n", curPos);
				if ( curPos >= 0 )
				{
					if (line.GetLength() > 0 && !line.Find("#") == 0) 
					{
						int epos = line.Find('=');
						if (epos > 0 && epos < line.GetLength())
						{
							CString name(line.Left(epos));
							name.TrimLeft(_T("\r\n\t "));
							name.TrimRight(_T("\r\n\t "));
							CString value(line.Right(line.GetLength()-epos-1));
							value.TrimLeft(_T("\r\n\t "));
							value.TrimRight(_T("\r\n\t "));
							m_props.SetAt(name, value);
						}
					}
				}
			} while ( curPos >= 0 );
		}
		f.Close();
	}
	delete buffer;
}
