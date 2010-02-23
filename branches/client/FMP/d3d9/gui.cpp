#include <windows.h>
#include "gui.h"
#include "../log.h"

FMPGUI Gui;
CButton * sm_bStart;
CButton * sm_bExit;

void StartMenuButtons(const char * Args, CElement *pElement)
{
	if(pElement == sm_bExit)
		exit(0);
}

FMPGUI::FMPGUI()
{
	 g_Mouse[0] = 0;
	 g_Mouse[1] = 0;
	 g_Mouse[2] = 0;
}

void FMPGUI::Load(IDirect3DDevice9 * g_pDevice)
{
	gpGui = new CGUI( g_pDevice );
	gpGui->LoadInterfaceFromFile( "ColorThemes.xml" );

	gpGui->LoadFont();
	gpGui->SetFontColors(0, 0, 0, 0, 255); // <!--black-->
	gpGui->SetFontColors(1, 255, 255, 255, 255); // <!--white-->
	gpGui->SetFontColors(2, 0, 0, 255, 255); // <!--blue-->
	gpGui->SetFontColors(3, 0, 255, 0, 255); // <!--green-->
	gpGui->SetFontColors(4, 255, 0, 0, 255); // <!--red-->
	gpGui->SetFontColors(5, 255, 0, 128, 255); // <!--pink-->
	gpGui->SetFontColors(6, 255, 0, 255, 255); // <!--purple-->
	gpGui->SetFontColors(7, 255, 150, 0, 255); // <!--orange-->
	gpGui->SetFontColors(8, 255, 255, 0, 255); // <!--yellow-->
	gpGui->SetFontColors(9, 0, 0, 128, 255); // <!--dark blue-->

	gpGui->SetVarInt("Titlebar height", 24);
	gpGui->SetVarInt("Button height", 20);

	CText * tInfo = new CText(10, 10, 300, 100, "WAIT OR CREATE INTERFACE");

	// Create Start Menu
	StartMenu = new CWindow(100, 100, 600, 400, "FOUR-MP START MENU");
	sm_bStart = new CButton(50, 50, 500, 0, "START", NULL, StartMenuButtons);
	sm_bExit = new CButton(50, 100, 500, 0, "EXIT", NULL, StartMenuButtons);

	StartMenu->AddElement(sm_bStart);
	StartMenu->AddElement(sm_bExit);
	StartMenu->SetVisible( 0 );

	// Create Servers List
	ServerList = new CWindow(100, 100, 600, 400, "FOUR-MP SERVERS LIST");
	ServerList->AddElement(tInfo);
	ServerList->SetVisible( 0 );

	// Create Chat
	Chat = new CWindow(10, 10, 200, 300, "FOUR-MP CHAT");
	Chat->AddElement(tInfo);
	Chat->SetVisible( 0 );

	// Create Option
	ServerList = new CWindow(100, 100, 600, 400, "FOUR-MP OPTIONS");
	ServerList->AddElement(tInfo);
	ServerList->SetVisible( 0 );

	gpGui->AddWindow( StartMenu );
	gpGui->AddWindow( ServerList );
	gpGui->AddWindow( Chat );
	gpGui->AddWindow( Option );

	gpGui->SetVisible( true );
}

void FMPGUI::HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if( gpGui->IsVisible() )
	{
		gpGui->GetMouse()->HandleMessage(Msg, wParam, lParam);
		gpGui->GetKeyboard()->HandleMessage(Msg, wParam, lParam);
	}
}

void FMPGUI::MoveMouse(int x, int y)
{
	if( gpGui->IsVisible() )
	{
		gpGui->GetMouse()->SetPos(x, y);
		gpGui->MouseMove(gpGui->GetMouse());

		bool mouse = 0;
		if(g_Mouse[0] != (GetAsyncKeyState(1) != 0))
		{
			g_Mouse[0] = GetAsyncKeyState(1) != 0;
			gpGui->GetMouse()->SetLeftButton(g_Mouse[0]);
			mouse = 1;
		}
		if(g_Mouse[1] != (GetAsyncKeyState(2) != 0))
		{
			g_Mouse[1] = GetAsyncKeyState(2) != 0;
			gpGui->GetMouse()->SetRightButton(g_Mouse[1]);
			mouse = 1;
		}
		if(g_Mouse[2] != (GetAsyncKeyState(3) != 0))
		{
			g_Mouse[2] = GetAsyncKeyState(3) != 0;
			gpGui->GetMouse()->SetMiddleButton(g_Mouse[2]);
			mouse = 1;
		}

		if(mouse)
			gpGui->KeyEvent( SKey( 0, (GetAsyncKeyState(1)||GetAsyncKeyState(2)||GetAsyncKeyState(3)) && 1 ) );
	}
}

void FMPGUI::Draw()
{
	gpGui->Draw();
}

CWindow * FMPGUI::GetStartMenu()
{
	return StartMenu;
}
CWindow * FMPGUI::GetServerList()
{
	return ServerList;
}
CWindow * FMPGUI::GetChat()
{
	return Chat;
}
CWindow * FMPGUI::GetOption()
{
	return Option;
}