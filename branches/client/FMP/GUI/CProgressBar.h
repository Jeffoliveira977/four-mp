#pragma once

#include "CGUI.h"

class CProgressBar : public CElement
{
	int m_iProgress, m_iStrWidth, m_iStrHeight;
	std::string m_sBuf;

	CColor * pInner, * pBorder, * pString;
	CTexture * pProgressBar;
public:
	CProgressBar(CGUI *Gui, int X, int Y, int Width, int Height, int value = 0, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void Draw();

	int GetProgress();
	void SetProgress( int iProgress );

	void UpdateTheme( int iIndex );
};