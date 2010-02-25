#pragma once

#include "CGUI.h"

class CWindow : public CElement
{
	bool m_bMaximized, m_bDragging, m_bVisible;
	CPos posDif;
	std::vector<CElement*> m_vElements;
	CElement * m_pFocussedElement;

	bool m_bCloseButtonEnabled;
	CTimer m_tCloseButtonPressed;
	bool m_bTitleVisible;

	CColor * pTitle, * pBodyInner, * pBodyBorder;
	CTexture * pTitlebar, * pButton;
public:
	CWindow( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );
	~CWindow();

	void AddElement( CElement * pElement );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void SetMaximized( bool bMaximized );
	bool GetMaximized();

	void SetVisible( bool bVisible );
	bool IsVisible();

	void SetDragging( bool bDragging );
	bool GetDragging();

	void SetCloseButton( bool bEnabled );
	bool GetCloseButton();

	void SetFocussedElement( CElement * pElement );
	CElement * GetFocussedElement();

	CElement * GetNextElement( CElement * pElement );
	CElement * GetElementByString( const char * pszString, int iIndex = 0 );
	
	void BringToTop( CElement * pElement );

	void UpdateTheme( int iIndex );

	void SetTitleVisible( bool v );
	bool GetTitleVisible();
};