#include <windows.h>
#include "gui.h"
#include "../log.h"

FMPGUI Gui;
// Windows
CWindow * fServBrowser;
CWindow * fChat;
CWindow * fOption;
// Chat elements
CTextBox * cc_tChat;
CEditBox * cc_tEnter;
CButton * cc_bEnter;
// Server Browser elemenets
CListView *sbServList, *sbPlayerList;
CButton *sbTabInet, *sbTabLAN, *sbTabVIP, *sbTabFav;
CEditBox *sbEnterIP, *sbEnterPort;
CButton *sbConnect, *sbRefresh, *sbAddToFav;
CDropDown *sbFltPing; 
CCheckBox *sbFltNotFull, *sbFltNoPassword, *sbFltNotEmpty;
CEditBox *sbFltLocation, *sbFltMode;


void ServerBrCallBack(CElement *pElement, CMSG msg, int Param)
{
	Debug("SB callback");
	if((pElement == sbEnterIP || pElement == sbEnterPort) && msg == GOT_FOCUS)
		pElement->SetString("");

	Debug("SB callback end");
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

FMPGUI::FMPGUI()
{
	 g_Mouse[0] = 0;
	 g_Mouse[1] = 0;
	 g_Mouse[2] = 0;
}

void FMPGUI::Load(IDirect3DDevice9 * g_pDevice)
{
	m_Gui = new CGUI( g_pDevice );
	m_Gui->LoadInterfaceFromFile( "GUITheme.xml" );

	m_Gui->LoadFont();
	m_Gui->SetFontColors(0, 0, 0, 0, 255); // <!--black-->
	m_Gui->SetFontColors(1, 255, 255, 255, 255); // <!--white-->
	m_Gui->SetFontColors(2, 0, 0, 255, 255); // <!--blue-->
	m_Gui->SetFontColors(3, 0, 255, 0, 255); // <!--green-->
	m_Gui->SetFontColors(4, 255, 0, 0, 255); // <!--red-->
	m_Gui->SetFontColors(5, 255, 0, 128, 255); // <!--pink-->
	m_Gui->SetFontColors(6, 255, 0, 255, 255); // <!--purple-->
	m_Gui->SetFontColors(7, 255, 150, 0, 255); // <!--orange-->
	m_Gui->SetFontColors(8, 255, 255, 0, 255); // <!--yellow-->
	m_Gui->SetFontColors(9, 0, 0, 128, 255); // <!--dark blue-->

	// Create Servers Brouser
	fServBrowser = new CWindow(m_Gui, 20, 20, 750, 500, "SERVER BROWSER", "SERVER_BROWSER", ServerBrCallBack);
	
	int ServerWidth[6] = {16, 210, 60, 60, 100, 100};
	sbServList = new CListView(m_Gui, 0, 29, ServerWidth, 350, 6, NULL, "SERVER_LIST", ServerBrCallBack);

	sbServList->SetTitle("P");
	sbServList->SetTitle("Server Name");
	sbServList->SetTitle("Slots");
	sbServList->SetTitle("Ping");
	sbServList->SetTitle("Mode");
	sbServList->SetTitle("Location");

	sbServList->PutStr("*", 0);
	sbServList->PutStr("FOUR-MP Official server", 1);
	sbServList->PutStr("3/32", 2);
	sbServList->PutStr("4", 3);
	sbServList->PutStr("FreeDM", 4);
	sbServList->PutStr("Russia", 5);

	int PlayerWidth[2] = {128, 76};
	sbPlayerList = new CListView(m_Gui, 546, 49, PlayerWidth, 200, 2, NULL, "PLAYER_LIST", ServerBrCallBack);

	sbPlayerList->SetTitle("Player");
	sbPlayerList->SetTitle("Score");

	sbTabInet = new CButton(m_Gui, 20, 0, 200, 0, "Internet", "TAB_INTERNET", ServerBrCallBack);
	sbTabLAN = new CButton(m_Gui, 169, 0, 200, 0, "LAN", "TAB_LAN", ServerBrCallBack);
	sbTabVIP = new CButton(m_Gui, 318, 0, 200, 0, "VIP", "TAB_VIP", ServerBrCallBack);
	sbTabFav = new CButton(m_Gui, 467, 0, 200, 0, "Favourite", "TAB_FAVOURITE", ServerBrCallBack);

	sbEnterIP = new CEditBox(m_Gui, 600, 290, 120, 0, "127.0.0.1", "EDIT_IP", ServerBrCallBack);
	sbEnterPort = new CEditBox(m_Gui, 625, 330, 60, 0, "7777", "EDIT_PORT", ServerBrCallBack);

	CText *sbTextIP = new CText(m_Gui, 560, 295, 100, 20, "IP", "TEXT_IP", NULL);
	CText *sbTextPort = new CText(m_Gui, 560, 335, 100, 20, "Port", "TEXT_PORT", NULL);

	sbConnect = new CButton(m_Gui, 560, 380, 80, 0, "Connect", "MAN_CONNECT", ServerBrCallBack);
	sbRefresh = new CButton(m_Gui, 660, 380, 80, 0, "Refresh", "MAN_REFRESH", ServerBrCallBack);
	sbAddToFav = new CButton(m_Gui, 600, 405, 120, 0, "Add to favourites", "MAN_ADDFAV", ServerBrCallBack);

	sbFltPing = new CDropDown(m_Gui, 150, 380, 80, 20, "Ping", "FILTER_PING", ServerBrCallBack);
	sbFltNotFull = new CCheckBox(m_Gui, 150, 410, 0, 0, 0, "", "FILTER_NOT_FULL", ServerBrCallBack);
	sbFltLocation = new CEditBox(m_Gui, 305, 380, 80, 20, "", "FILTER_LOCATION", ServerBrCallBack);
	sbFltMode = new CEditBox(m_Gui, 305, 410, 80, 20, "", "FILTER_Mode", ServerBrCallBack);
	sbFltNotEmpty = new CCheckBox(m_Gui, 480, 385, 0, 0, 0, "", "FILTER_NOT_EMPTY", ServerBrCallBack);
	sbFltNoPassword = new CCheckBox(m_Gui, 480, 410, 0, 0, 1, "", "FILTER_NO_PASSWORD", ServerBrCallBack);

	sbFltPing->AddElement("< 50", "50");
	sbFltPing->AddElement("50 - 100", "50100");
	sbFltPing->AddElement("100 - 200", "100200");
	sbFltPing->AddElement("200 - 500", "200500");
	sbFltPing->AddElement("> 500", "500");

	fServBrowser->AddElement(sbServList);
	fServBrowser->AddElement(sbPlayerList);
	fServBrowser->AddElement(sbTabInet);
	fServBrowser->AddElement(sbTabLAN);
	fServBrowser->AddElement(sbTabVIP);
	fServBrowser->AddElement(sbTabFav);
	fServBrowser->AddElement(sbEnterPort);
	fServBrowser->AddElement(sbEnterIP);
	fServBrowser->AddElement(sbTextIP);
	fServBrowser->AddElement(sbTextPort);
	fServBrowser->AddElement(sbConnect);
	fServBrowser->AddElement(sbRefresh);
	fServBrowser->AddElement(sbAddToFav);
	fServBrowser->AddElement(sbFltPing);
	fServBrowser->AddElement(sbFltLocation);
	fServBrowser->AddElement(sbFltMode);
	fServBrowser->AddElement(sbFltNotFull);
	fServBrowser->AddElement(sbFltNotEmpty);
	fServBrowser->AddElement(sbFltNoPassword);

	// Create Chat
	fChat = new CWindow(m_Gui, 10, 10, 200, 300, "FOUR-MP CHAT");
	cc_tChat = new CTextBox(m_Gui, 0, 0, 200, 258, NULL, NULL, ChatCallBack);
	cc_tEnter = new CEditBox(m_Gui, 0, 257, 160, 0, NULL, NULL, ChatCallBack);
	cc_bEnter = new CButton(m_Gui, 160, 257, 40, 0, "SEND", NULL, ChatCallBack);
	fChat->AddElement(cc_tChat);
	fChat->AddElement(cc_tEnter);
	fChat->AddElement(cc_bEnter);
	fChat->SetVisible( 1 );

	// Create Option
	fOption = new CWindow(m_Gui, 200, 200, 400, 300, "FOUR-MP OPTIONS");
	CText * tInfo = new CText(m_Gui, 10, 10, 300, 100, "WAIT OR CREATE INTERFACE");
	fOption->AddElement(tInfo);
	fOption->SetVisible( 0 );

	m_Gui->SetVisible( true );
	m_Gui->UpdateFromFile("GUIStyle.xml");
}

void FMPGUI::HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if( m_Gui->IsVisible() )
	{
		m_Gui->GetMouse()->HandleMessage(Msg, wParam, lParam);
		m_Gui->GetKeyboard()->HandleMessage(Msg, wParam, lParam);
	}
}

void FMPGUI::MoveMouse(int x, int y)
{
	if( m_Gui->IsVisible() )
	{
		m_Gui->GetMouse()->SetPos(x, y);
		m_Gui->MouseMove(m_Gui->GetMouse());

		bool mouse = 0;
		if(g_Mouse[0] != (GetAsyncKeyState(1) != 0))
		{
			g_Mouse[0] = GetAsyncKeyState(1) != 0;
			m_Gui->GetMouse()->SetLeftButton(g_Mouse[0]);
			mouse = 1;
		}
		if(g_Mouse[1] != (GetAsyncKeyState(2) != 0))
		{
			g_Mouse[1] = GetAsyncKeyState(2) != 0;
			m_Gui->GetMouse()->SetRightButton(g_Mouse[1]);
			mouse = 1;
		}
		if(g_Mouse[2] != (GetAsyncKeyState(3) != 0))
		{
			g_Mouse[2] = GetAsyncKeyState(3) != 0;
			m_Gui->GetMouse()->SetMiddleButton(g_Mouse[2]);
			mouse = 1;
		}

		if(mouse)
			m_Gui->KeyEvent( SKey( 0, (GetAsyncKeyState(1)||GetAsyncKeyState(2)||GetAsyncKeyState(3)) && 1 ) );
	}
}

void FMPGUI::Draw()
{
	m_Gui->Draw();
}
