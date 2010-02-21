#pragma once
#include "CGUI.h"

class CVerticalSliderBar : public CHorizontalSliderBar
{
	CColor * pLines, * pString;
	CTexture * pSlider;
public:
	CVerticalSliderBar( int X, int Y, int Width, int Height, int minValue = 0, int maxValue = 0, int step = 1, int value = 0, const char * String = NULL, const char * String2 = NULL, const char * Callback = NULL );

	void Draw();
	void MouseMove( CMouse * pMouse );

	void UpdateTheme( int iIndex );
};