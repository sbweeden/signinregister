#pragma once

class MyDelete
{
public:
	MyDelete(LPVOID pVoid) { m_pVoid = pVoid; }
	~MyDelete() { if ( m_pVoid != NULL ) { delete m_pVoid; } }

private:
	LPVOID m_pVoid;
};
