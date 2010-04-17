#pragma once

#include "CGUI.h"

class CHorizontalSliderBar : public CElement
{
	friend class CVerticalSliderBar;

	int m_iMinValue, m_iMaxValue, m_iValue, m_iStep;
	bool m_bDragged, m_bShowString;

	tAction m_pUpdater;

	CColor * pLines, * pString;
	CTexture * pSlider;

public:
	CHorizontalSliderBar();
	CHorizontalSliderBar(CGUI *Gui, int X, int Y, int Width, int Height, int minValue = 0, int maxValue = 0, int step = 1, int value = 0, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void SetSliderElement(CGUI *Gui, int X, int Y, int Width, int Height, int minValue = 0, int maxValue = 0, int step = 1, int value = 0, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	int GetMinValue();
	int GetMaxValue();
	int GetValue();
	int GetStep();

	void SetMinValue( int iMinValue );
	void SetMaxValue( int iMaxValue );
	void SetValue( int iValue );
	void SetStep( int iStep );

	bool GetDragged();
	void SetDragged( bool bDragged );

	void SetShowString( bool bShow );
	bool GetShowString();

	void UpdateTheme( int iIndex );
};

std::string MinValue( const char *, CElement * pElement );
std::string MaxValue( const char *, CElement * pElement );
std::string SliderValue( const char *, CElement * pElement );