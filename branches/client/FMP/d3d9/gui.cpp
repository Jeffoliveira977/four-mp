#include <windows.h>
#include "gui.h"
#include "../log.h"

FMPGUI Gui;
CButton * sm_bStart;
CButton * sm_bExit;
CTextBox * cc_tChat;
CEditBox * cc_tEnter;
CButton * cc_bEnter;
CListView * sl_tList;

void StartCallBack(CElement *pElement, CMSG msg, int Param)
{
	Debug("Start callback");
	if(msg != CLICK) { Debug("Start callback pre end"); return; }

	if(pElement == sm_bExit)
		exit(0);
	else if(pElement == sm_bStart)
	{
		// START GAME
	}
	Debug("Start callback end");
}

void ChatCallBack(CElement *pElement, CMSG msg, int Param)
{
	Debug("Chat callback");
	if(pElement == cc_tChat && msg == SELECT)
	{
		// Select chat string
	}
	else if((pElement == cc_tEnter && msg == END) || (pElement == cc_bEnter && msg == CLICK))
	{
		// Send message
		cc_tChat->AddString(cc_tEnter->GetString());
	}
	Debug("Chat callback end");
}

void ServerListCallback(CElement *pElement, CMSG msg, int Param)
{
	
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

	// Create Start Menu
	StartMenu = new CWindow(100, 100, 600, 400, "FOUR-MP START MENU");
	sm_bStart = new CButton(50, 50, 500, 0, "START", NULL, StartCallBack);
	sm_bExit = new CButton(50, 100, 500, 0, "EXIT", NULL, StartCallBack);

	StartMenu->AddElement(sm_bStart);
	StartMenu->AddElement(sm_bExit);
	StartMenu->SetVisible( 0 );

	// Create Servers List
	ServerList = new CWindow(100, 100, 600, 400, "FOUR-MP SERVERS LIST");
	int width[] = {20, 200, 180, 50, 100, 50};
	sl_tList = new CListView(0, 0, width, 300, 6, NULL, NULL, ServerListCallback);
	ServerList->AddElement(sl_tList);
	ServerList->SetVisible( 1 );

	sl_tList->SetTitle("Password");
	sl_tList->SetTitle("Name");
	sl_tList->SetTitle("Mode");
	sl_tList->SetTitle("Players");
	sl_tList->SetTitle("Location");
	sl_tList->SetTitle("Ping");

	sl_tList->PutStr("X", 0, 0);
	sl_tList->PutStr("FOUR-MP Test server [FAKE]", 1, 0);
	sl_tList->PutStr("Build mode by 009", 2, 0);
	sl_tList->PutStr("0/0", 3, 0);
	sl_tList->PutStr("Russia", 4, 0);
	sl_tList->PutStr("0", 5, 0);


	// Create Chat
	Chat = new CWindow(10, 10, 200, 300, "FOUR-MP CHAT");
	cc_tChat = new CTextBox(0, 0, 200, 258, NULL, NULL, ChatCallBack);
	cc_tEnter = new CEditBox(0, 257, 160, 0, NULL, NULL, ChatCallBack);
	cc_bEnter = new CButton(160, 257, 40, 0, "SEND", NULL, ChatCallBack);
	Chat->AddElement(cc_tChat);
	Chat->AddElement(cc_tEnter);
	Chat->AddElement(cc_bEnter);
	Chat->SetVisible( 1 );

	// Create Option
	Option = new CWindow(200, 200, 400, 300, "FOUR-MP OPTIONS");
	CText * tInfo = new CText(10, 10, 300, 100, "WAIT OR CREATE INTERFACE");
	Option->AddElement(tInfo);
	Option->SetVisible( 0 );

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