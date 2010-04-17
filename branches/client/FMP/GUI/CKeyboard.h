#pragma once

#include "CGUI.h"

struct SKey
{
	char m_vKey;
	bool m_bDown;
	LPARAM m_lParam;

	SKey()
	{
		m_vKey = 0;
		m_bDown = false;
		m_lParam = 0;
	}
	SKey( char vKey, bool bDown )
	{
		m_vKey = vKey;
		m_bDown = bDown;
		m_lParam = 0;
	}
	SKey( char vKey, bool bDown, LPARAM lParam )
	{
		m_vKey = vKey;
		m_bDown = bDown;
		m_lParam = lParam;
	}
};

class CKeyboard
{
	CGUI *pGui;
	SKey m_sKey;

public:
	CKeyboard(CGUI *Gui);
	bool HandleMessage( unsigned int uMsg, __w64 unsigned int wParam, _w64 long lParam );

	void SetKey( SKey sKey );
	SKey GetKey();
};