#pragma once

#include "CGUI.h"

class CDropDown : public CElement
{
	bool m_bDropped;
	int m_iMouseOverIndex, m_iSelected;
	
	struct SEntry
	{
		std::string m_sString, m_sValue;

		SEntry( std::string sString, std::string sValue )
		{
			m_sString = sString;
			m_sValue = sValue;
		}
	};
	std::vector<SEntry> m_vEntrys;

	CColor * pInner, * pBorder, * pString, * pSelectedInner, * pSelectedString;
	CTexture * pButton;

public:
	CDropDown( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, const char * Callback = NULL );

	void Draw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void AddElement( std::string sElem, std::string sValue );
	std::string GetValue();

	void UpdateTheme( int iIndex );
};