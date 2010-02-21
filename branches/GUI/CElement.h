#pragma once

#include "CPos.h"

class CElement
{
	bool m_bHasFocus, m_bMouseOver;

	std::string m_sRaw[2], m_sFormatted[2];
	int m_iWidth, m_iHeight;

	CPos m_relPos, m_absPos;
	CWindow * m_pParent;

	typedef std::string ( __cdecl * tAction )( const char * pszArgs, CElement * pElement );
	tAction m_pAction;

	SElement * m_pThemeElement[ 3 ];
	SElementState * m_pElementState[ 3 ];

public:

	void SetElement( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, const char * Callback = NULL, bool abs = 0 );

	void SetParent( CWindow * pParent );
	CWindow * GetParent();

	void SetAction( tAction pAction );
	tAction GetAction();

	void SetRelPos( CPos relPos );
	void SetAbsPos( CPos absPos );

	CPos * GetRelPos();
	CPos * GetAbsPos();

	void SetWidth( int iWidth );
	void SetHeight( int iHeight );

	int GetWidth();
	int GetHeight();

	bool HasFocus();

	void SetString( std::string sString, int iIndex = 0 );
	std::string GetString( bool bReplaceVars = false, int iIndex = 0 );
	std::string GetFormatted( int iIndex = 0 );

	bool GetMouseOver();
	bool SetMouseOver( bool bMouseOver );

	SElement * SetThemeElement( SElement * pThemeElement, int iIndex = 0 );
	SElement * GetThemeElement( int iIndex = 0 );

	void SetElementState( std::string sState, int iIndex = 0 );
	SElementState * GetElementState( int iIndex = 0 );

	virtual void UpdateTheme( int iIndex );

	virtual void Draw();
	virtual void PreDraw();
	virtual void MouseMove( CMouse * pMouse );
	virtual void KeyEvent( SKey sKey );
};