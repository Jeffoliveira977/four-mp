#pragma once

#include "CGUI.h"

class CHelperSlider : public CHorizontalSliderBar
{
	bool m_bMouseOver[ 3 ], m_bPressed[ 2 ], m_bDragged;
	CTimer m_tUpArrow, m_tDownArrow;

	SElement * m_pThemeElement[ 3 ];
	SElementState * m_pElementState[ 3 ];

	void SetDragged( bool bDragged );
	bool GetDragged();

	CColor * pInner, * pBorder;
	CTexture * pSlider, * pUpArrow, * pDownArrow;

public:
	bool Show;
	CHelperSlider(CGUI *Gui, CPos relPos, int iHeight );

	void Draw( CPos basePos );
	void PreDraw();
	void MouseMove( CPos basePos, CMouse * pMouse );
	bool KeyEvent( CPos basePos, SKey sKey );

	void UpdateTheme( int iIndex );
};