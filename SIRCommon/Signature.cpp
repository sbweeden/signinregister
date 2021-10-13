#include "StdAfx.h"
#include "Signature.h"
#include "Resource.h"
#include "Msg.h"

Signature::Signature(void)
{
}

Signature::Signature(const Signature& sig)
{
	for ( int i = 0; i < sig.m_siglines.GetCount(); i++ )
	{
		CRect * pRect = (CRect *) sig.m_siglines.GetAt(i);
		m_siglines.Add(new CRect(*pRect));
	}
}

Signature::Signature(const Signature * pSig)
{
	if ( pSig != NULL )
	{
		for ( int i = 0; i < pSig->m_siglines.GetCount(); i++ )
		{
			CRect * pRect = (CRect *) pSig->m_siglines.GetAt(i);
			m_siglines.Add(new CRect(*pRect));
		}
	}
}

Signature::~Signature(void)

{
	ClearAllStrokes();
}

Signature& Signature::operator=(const Signature& sig)
{
	ClearAllStrokes();
	for ( int i = 0; i < sig.m_siglines.GetCount(); i++ )
	{
		CRect * pRect = (CRect *) sig.m_siglines.GetAt(i);
		m_siglines.Add(new CRect(*pRect));
	}
	return *this;
}

void Signature::Draw(
	CDC * pDC, 
	CPoint topLeftAnchor, 
	int penWidth /* = 1 */,
	bool border /* = true */, 
	double factor /* = 1.0 */)
{
	CPen sigpen(PS_SOLID, penWidth, RGB(0,0,0));
	CPen * pOldPen = (CPen *) pDC->SelectObject(&sigpen);

	if ( border )
	{
		CRect borderRect(
			1, 
			1, 
			SIG_PIXEL_WIDTH+1, 
			SIG_PIXEL_HEIGHT+1);
		Scale(borderRect, factor);
		borderRect.MoveToXY(topLeftAnchor);
		pDC->Rectangle(borderRect);
	}

	for ( int i = 0; i < m_siglines.GetCount(); i++ )
	{
		CRect * pRect = (CRect *) m_siglines.GetAt(i);

		//pDC->MoveTo(topLeftAnchor + pRect->TopLeft());
		//pDC->LineTo(topLeftAnchor + pRect->BottomRight());

		CRect r(*pRect);
		Scale(r, factor);

		pDC->MoveTo(topLeftAnchor + r.TopLeft());
		pDC->LineTo(topLeftAnchor + r.BottomRight());
	}

	pDC->SelectObject(pOldPen);
}

void Signature::AddStroke(CPoint start, CPoint end)
{
	m_siglines.Add(new CRect(start, end));
}

void Signature::AddStrokeAndDraw(CDC * pDC, CPoint topLeftAnchor, CPoint start, CPoint end, int penWidth /* = 1 */)
{
	AddStroke(start, end);
	CPen sigpen(PS_SOLID, penWidth, RGB(0,0,0));
	CPen * pOldPen = (CPen *) pDC->SelectObject(&sigpen);
	pDC->MoveTo(topLeftAnchor + start);
	pDC->LineTo(topLeftAnchor + end);
	pDC->SelectObject(pOldPen);
}

void Signature::ClearAllStrokes()
{
	while ( m_siglines.GetSize() > 0 )
	{
		CRect * pRect = (CRect *) m_siglines.GetAt(0);
		m_siglines.RemoveAt(0);
		delete pRect;
	}
}

CStringA Signature::ToAsciiString() const
{
	CStringA result;
	CPoint lastPoint(-1,-1);
	for ( int i = 0; i < m_siglines.GetCount(); i++ )
	{
		CStringA thisSegmentStr;
		CRect * pRect = (CRect *) m_siglines.GetAt(i);
		if ( lastPoint == pRect->TopLeft() )
		{
			// just add the new endpoint of this line
			thisSegmentStr.Format("-%d,%d", pRect->BottomRight().x, pRect->BottomRight().y );
		}
		else
		{
			// start new line
			thisSegmentStr.Format(".%d,%d-%d,%d", pRect->TopLeft().x, pRect->TopLeft().y, pRect->BottomRight().x, pRect->BottomRight().y );
		}
		result += thisSegmentStr;
		lastPoint = pRect->BottomRight();
	}
	return result;
}

void Signature::FromAsciiString(const char * str)
{
	ClearAllStrokes();

	if ( str )
	{
		CPoint lastPoint(-1,-1);
		int x = 0, y = 0;
		CRect r;
		bool done = false;
		const char * p = str;
		while ( *p != NULL && !done )
		{
			// next char should be a control character, either . to signify new line, or - to signify continuation, or ; for end of field
			switch (*p++)
			{
				case '.':
					// str should point to a string of format "%d,%d-%d,%d". Just read the first point, and store it in lastPoint
					if (sscanf(p, "%d,%d", &x, &y) == 2)
					{
						lastPoint.x = x;
						lastPoint.y = y;
						while (*p && *p != '-') 
						{
							p++;
						}
					}
					break;
				case '-':
					// str should point to a string of format "%d,%d". Read this point, and create a stroke from it to lastPoint, then update lastPoint
					if (sscanf(p, "%d,%d", &x, &y) == 2)
					{
						CPoint nextPoint(x, y);
						AddStroke(lastPoint, nextPoint);
						lastPoint = nextPoint;
						while (*p && *p != '-' && *p != '.') 
						{
							p++;
						}
					}
					break;
				case ';':
					// end of field
					done = true;
					break;
				default:
					done = true;
					::MessageBox(NULL, CFMsg(IDS_ERR_PARSE_SIG, str, p-str ), CMsg(IDS_MB_ERROR), MB_OK);
			}
		}
	}
}

void Signature::Scale(CRect& rect, double factor)
{
	rect.left = (int) ((double) rect.left * factor);
	rect.top = (int) ((double) rect.top * factor);
	rect.right = (int) ((double) rect.right * factor);
	rect.bottom = (int) ((double) rect.bottom * factor);
}