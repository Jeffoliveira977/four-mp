#pragma once

#define USE_D3DX

#define TITLEBAR_HEIGHT 24
#define BUTTON_HEIGHT	20
#define HELPERSLIDER_WIDTH 22

#define SAFE_DELETE( pData ) if( pData ){ delete pData; pData = 0; }

class CTexture;
class CTimer;
class CPos;
class CColor;
class CMouse;
class CKeyboard;
class CElement;
class CWindow;
class CButton;
class CCheckBox;
class CProgressBar;
class CTextBox;
class CListBox;

#include <windowsx.h>
#include <shlwapi.h>

#include <map>
#include <set>
#include <limits>
#include <iomanip>
#include <sstream>
#include <vector>

#include "D3D9.h"
#include "CD3DRender.h"
#include "TinyXML\tinyxml.h"

#include "CTexture.h"
#include "CFont.h"

#include "CVar.h"
#include "CTimer.h"

#include "CPos.h"
#include "CColor.h"

#include "CMouse.h"
#include "CKeyboard.h"

#include "CElement.h"
#include "CWindow.h"
#include "CHorizontalSliderBar.h"
#include "CVerticalSliderBar.h"
#include "CHelperSlider.h"
#include "CButton.h"
#include "CCheckBox.h"
#include "CProgressBar.h"
#include "CText.h"
#include "CEditBox.h"
#include "CDropDown.h"
#include "CTextBox.h"
#include "CListBox.h"
#include "CListView.h"

class CGUI
{
	bool m_bVisible, m_bReload;

	CMouse * m_pMouse;
	CKeyboard * m_pKeyboard;
	CFont * m_pFont;

	IDirect3DDevice9 * m_pDevice;

	ID3DXSprite * m_pSprite;

#ifdef USE_D3DX
	ID3DXLine * m_pLine;
#else
	CD3DRender * m_pRender;
#endif
	
	CTimer m_tPreDrawTimer;

	std::vector<CWindow*> m_vWindows;
	CWindow * m_wFocus;

	std::string m_sCurTheme;

	typedef std::map<std::string, SElement*> tTheme;
	std::map<std::string, tTheme> m_mThemes;
public:

	CGUI( IDirect3DDevice9 * pDevice );
	~CGUI();

	void LoadFont(int size = 10, char *font = "Tahoma");
	void SetFontColors(int Index, int r, int g, int b, int a);
	void LoadInterfaceFromFile( const char * pszFilePath );

	void FillArea( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dColor );
	void DrawLine( int iStartX, int iStartY, int iEndX, int iEndY, int iWidth, D3DCOLOR d3dColor );
	void DrawOutlinedBox( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dInnerColor, D3DCOLOR d3dBorderColor );

	CWindow * AddWindow( CWindow * pWindow );
	void BringToTop( CWindow * pWindow );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	bool KeyEvent( SKey sKey );

	void OnLostDevice();
	void OnResetDevice( IDirect3DDevice9 * pDevice );

	CMouse * GetMouse();
	CKeyboard * GetKeyboard();

	IDirect3DDevice9 * GetDevice();
	CFont * GetFont();
	ID3DXSprite * GetSprite();

	CWindow * GetWindowByString( std::string sString, int iIndex = 0 );

	SElement * GetThemeElement( std::string sElement );

	void SetVisible( bool bVisible );
	bool IsVisible();

	bool ShouldReload();
	void Reload();

	void SetVarString(const char *name, std::string value);
	void SetVarInt(const char *name, int value);
	void SetVarBool(const char *name, bool value);

	bool IsFocus(CWindow * w);

	std::map<std::string,CVar*> Cvars;
};

extern CGUI * gpGui;