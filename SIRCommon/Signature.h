#pragma once

#define SIG_PIXEL_WIDTH 680
#define SIG_PIXEL_HEIGHT 265

class Signature
{
public:
	Signature();
	Signature(const Signature& sig);
	Signature(const Signature * sig);
	Signature& operator=(const Signature& sig);
	void Draw(CDC * pDC, CPoint topLeftAnchor, int penWidth = 1, bool border = true, double factor = 1.0);
	void AddStroke(CPoint start, CPoint end);
	void AddStrokeAndDraw(CDC * pDC, CPoint topLeftAnchor, CPoint start, CPoint end, int penWidth = 1);
	INT_PTR NumStrokes() const { return m_siglines.GetCount(); }
	CStringA ToAsciiString() const;
	void FromAsciiString(const char * str);
	void Scale(CRect& rect, double factor);

	void ClearAllStrokes();
public:
	virtual ~Signature(void);

private:
	CPtrArray m_siglines;
};
