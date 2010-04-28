#include "../masterserver.h"
#include <windows.h>
#include "gui.h"
#include "../log.h"
#include "../ConsoleWindow.h"


extern ConsoleWindow conwindow;

// Windows
CWindow * fServBrowser;
CWindow * fChat;
CWindow * fOption;
CWindow * fUserRegister;
CWindow * fUserLogin;
CWindow * fInfo;
// Chat elements
CTextBox * cc_tChat;
CEditBox * cc_tEnter;
CButton * cc_bEnter;
// Server Browser elemenets
CListView *sbServList, *sbPlayerList;
CButton *sbTab[4];
CEditBox *sbEnterIP, *sbEnterPort;
CButton *sbConnect, *sbRefresh, *sbAddToFav;
CDropDown *sbFltPing; 
CCheckBox *sbFltNotFull, *sbFltNoPassword, *sbFltNotEmpty;
CEditBox *sbFltLocation, *sbFltMode;
// Register elements
CEditBox *urLogin, *urPass, *urConfirm, *urEmail, *urNick;
CText *urLoginText, *urPassText, *urConfirmText, *urEmailText, *urNickText, *urInfoText;
CButton *urSendReg, *urCancelReg;
// Login elements
CText *upLoginInfo, *upStrLogin, *upStrPass;
CEditBox *upLogin, *upPassword;
CButton *upSendLogin, *upShowRegister;
CCheckBox *upRemeberMe;

int tab = 3;

MasterServer fmpms;

namespace CALLBACKS
{
	void Refresh( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::Refresh called");

		sbServList->Clear();

		if(tab == 0 || tab == 2)
		{
			fmpms.ClearServerList();
			bool r = fmpms.QueryServerList(0, tab == 2, sbFltNotEmpty->GetChecked(), sbFltNotFull->GetChecked(), 
				sbFltNoPassword->GetChecked(),	0, 0, sbFltMode->GetString().size()>0?(char*)sbFltMode->GetString().c_str():0, sbFltLocation->GetString().size()?(char*)sbFltLocation->GetString().c_str():0);
			
			if(!r) { fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Load server list failed"); fInfo->SetVisible(1); }
			else
			{
				char tmp[128];
				MasterServerInfo *msi;
				
				for(int i = 0; i < fmpms.GetNumServers(); i++)
				{
					msi = fmpms.GetServerInfo(i);
					sprintf(tmp, "%s:%d", msi->ip, msi->port);
					sbServList->PutStr((msi->password?"1":"0"), 0);
					sbServList->PutStr(msi->name, 1, -1, tmp);
					sprintf(tmp, "%d/%d", msi->players, msi->maxplayers);
					sbServList->PutStr(tmp, 2);
					sprintf(tmp, "%d", 9999);
					sbServList->PutStr(tmp, 3);
					sbServList->PutStr(msi->mode, 4);
					sbServList->PutStr(msi->loc, 5);
				}
			}
		}
		Debug("CALLBACKS::Refresh complete");
	}
	void GetLAN( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::GetLAN called");

		sbServList->Clear();

		sbTab[tab]->SetEnabled(1);
		tab = 1;
		sbTab[tab]->SetEnabled(0);
		Debug("CALLBACKS::GetLAN complete");
	}
	void GetInet( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::GetInet called");

		sbTab[tab]->SetEnabled(1);
		tab = 0;
		sbTab[tab]->SetEnabled(0);

		if(fmpms.GetNumServers() > 0)
		{
			char tmp[64];
			MasterServerInfo *msi;
			sbServList->Clear();
			for(int i = 0; i < fmpms.GetNumServers(); i++)
			{
				msi = fmpms.GetServerInfo(i);
				sprintf(tmp, "%s:%d", msi->ip, msi->port);
				sbServList->PutStr((msi->password?"1":"0"), 0);
				sbServList->PutStr(msi->name, 1, -1, tmp);
				sprintf(tmp, "%d/%d", msi->players, msi->maxplayers);
				sbServList->PutStr(tmp, 2);
				sprintf(tmp, "%d", 9999);
				sbServList->PutStr(tmp, 3);
				sbServList->PutStr(msi->mode, 4);
				sbServList->PutStr(msi->loc, 5);
			}
		}
		Debug("CALLBACKS::GetInet complete");
	}
	void GetVIP( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::GetVIP called");

		sbTab[tab]->SetEnabled(1);
		tab = 2;
		sbTab[tab]->SetEnabled(0);

		if(fmpms.GetNumServers() > 0)
		{
			char tmp[64];
			MasterServerInfo *msi;
			sbServList->Clear();
			for(int i = 0; i < fmpms.GetNumServers(); i++)
			{
				msi = fmpms.GetServerInfo(i);
				if(msi->vip == 1)
				{
					sprintf(tmp, "%s:%d", msi->ip, msi->port);
					sbServList->PutStr((msi->password?"1":"0"), 0);
					sbServList->PutStr(msi->name, 1, -1, tmp);
					sprintf(tmp, "%d/%d", msi->players, msi->maxplayers);
					sbServList->PutStr(tmp, 2);
					sprintf(tmp, "%d", 9999);
					sbServList->PutStr(tmp, 3);
					sbServList->PutStr(msi->mode, 4);
					sbServList->PutStr(msi->loc, 5);
				}
			}
		}
		Debug("CALLBACKS::GetVIP complete");
	}
	void GetFavourite( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::GetFavourite called");

		sbServList->Clear();

		sbTab[tab]->SetEnabled(1);
		tab = 3;
		sbTab[tab]->SetEnabled(0);
		Debug("CALLBACKS::GetFavourite complete");
	}
	void Login( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::Login called");

		string login = upLogin->GetString();
		string pass = upPassword->GetString();

		bool r = fmpms.QueryUserLogin((char*)login.c_str(), (char*)pass.c_str());
		if(!r) fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Autorization failed");
		else 
		{
			fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Autorization ok");
			fUserLogin->SetVisible(0);
		}

		fInfo->SetVisible(1);
		Debug("CALLBACKS::Login complete");
	}
	void ShowRegister( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::ShowRegister called");

		fUserRegister->SetVisible(1);
		fUserLogin->SetVisible(0);
		Debug("CALLBACKS::ShowRegister complete");
	}
	void HideRegister( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::HideRegister called");

		fUserRegister->SetVisible(0);
		fUserLogin->SetVisible(1);
		Debug("CALLBACKS::HideRegister complete");
	}
	void Register( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::Register called");

		string login = urLogin->GetString();
		string pass = urPass->GetString();
		string confirm = urConfirm->GetString();
		string email = urEmail->GetString();
		string nick = urNick->GetString();

		if(strcmp(pass.c_str(), confirm.c_str()) == 0)
		{
			bool r = fmpms.QueryUserRegister(login.c_str(), nick.c_str(), pass.c_str(), email.c_str());
			if(!r) fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Registration failed");
			else
			{
				fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Registration ok");
				fUserRegister->SetVisible(0);
				fUserLogin->SetVisible(1);
			}
		}
		else
			fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Password != confirm password");

		fInfo->SetVisible(1);
		Debug("CALLBACKS::Register complete");
	}
	void InfoOK( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;

		Debug("CALLBACKS::InfoOK called");
		fInfo->SetVisible(0);
		Debug("CALLBACKS::IfoOK complete");
	}
	void Logout( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::Logout called");
		
		bool r = fmpms.QueryUserLogout();
		if(!r) fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Logout failed");
		else 
		{
			fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Logout ok");
			fUserLogin->SetVisible(1);
		}
		fInfo->SetVisible(1);
		Debug("CALLBACKS::Logout complete");
	}
	void Chat(CElement *pElement, CMSG msg, int Param)
	{
		if(msg != SELECT && msg != END && msg != CLICK) return;
		Debug("CALLBACKS::Chat called");
		//Debug("Chat callback");
		if(pElement == cc_tChat && msg == SELECT)
		{
			Debug("CALLBACKS::Chat >> Select");
			// Select chat string
		}
		else if((pElement == cc_tEnter && msg == END) || (pElement == cc_bEnter && msg == CLICK))
		{
			// Send message
			Debug("CALLBACKS::Chat >> Add");
			cc_tChat->AddString(cc_tEnter->GetString());
			Debug("CALLBACKS::Chat >> Add end");
		}
		//Debug("Chat callback end");
		Debug("CALLBACKS::Chat complete");
	}
	void ServerList(CElement *pElement, CMSG msg, int Param)
	{
	}
};

FMPGUI::FMPGUI()
{
	Debug("FMPGUI::FMPGUI called");
	g_Mouse[0] = 0;
	g_Mouse[1] = 0;
	g_Mouse[2] = 0;
	GuiReady = 0;
	s_iWidth = 800;
	s_iHeight = 600;
	Debug("FMPGUI::FMPGUI complete");
}

void FMPGUI::Load(IDirect3DDevice9 * g_pDevice)
{
	Debug("FMPGUI::Load called");
	fmpms.Load();
	Debug("FMPGUI::Load >> Master Server");

	D3DVIEWPORT9 vp;
	g_pDevice->GetViewport(&vp);
	s_iWidth = vp.Width;
	s_iHeight = vp.Height;

	m_Gui = new CGUI( g_pDevice );
	m_Gui->LoadInterfaceFromFile( "FMP\\GUITheme.xml" );

	Debug("FMPGUI::Load >> Load Font");
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

	Debug("FMPGUI::Load >> Create server browser");
	// Create Servers Brouser
	fServBrowser = new CWindow(m_Gui, 20, 20, 750, 500, "SERVER BROWSER", "SERVER_BROWSER");
	
	int ServerWidth[6] = {16, 210, 60, 60, 100, 100};
	sbServList = new CListView(m_Gui, 0, 29, ServerWidth, 350, 6, NULL, "SERVER_LIST", CALLBACKS::ServerList);

	sbServList->SetTitle("P");
	sbServList->SetTitle("Server Name");
	sbServList->SetTitle("Slots");
	sbServList->SetTitle("Ping");
	sbServList->SetTitle("Mode");
	sbServList->SetTitle("Location");

	int PlayerWidth[2] = {128, 76};
	sbPlayerList = new CListView(m_Gui, 546, 49, PlayerWidth, 200, 2, NULL, "PLAYER_LIST");

	sbPlayerList->SetTitle("Player");
	sbPlayerList->SetTitle("Score");

	sbTab[0] = new CButton(m_Gui, 20, 0, 200, 0, "Internet", "TAB_INTERNET", CALLBACKS::GetInet);
	sbTab[1] = new CButton(m_Gui, 169, 0, 200, 0, "LAN", "TAB_LAN", CALLBACKS::GetLAN);
	sbTab[2] = new CButton(m_Gui, 318, 0, 200, 0, "VIP", "TAB_VIP", CALLBACKS::GetVIP);
	sbTab[3] = new CButton(m_Gui, 467, 0, 200, 0, "Favourite", "TAB_FAVOURITE", CALLBACKS::GetFavourite);

	sbEnterIP = new CEditBox(m_Gui, 600, 290, 120, 0, "127.0.0.1", "EDIT_IP");
	sbEnterPort = new CEditBox(m_Gui, 625, 330, 60, 0, "7777", "EDIT_PORT");

	CText *sbTextIP = new CText(m_Gui, 560, 295, 100, 20, "IP", "TEXT_IP", NULL);
	CText *sbTextPort = new CText(m_Gui, 560, 335, 100, 20, "Port", "TEXT_PORT", NULL);

	sbConnect = new CButton(m_Gui, 560, 380, 80, 0, "Connect", "MAN_CONNECT");
	sbRefresh = new CButton(m_Gui, 660, 380, 80, 0, "Refresh", "MAN_REFRESH", CALLBACKS::Refresh);
	sbAddToFav = new CButton(m_Gui, 600, 405, 120, 0, "Add to favourites", "MAN_ADDFAV");

	sbFltPing = new CDropDown(m_Gui, 150, 380, 80, 20, "Ping", "FILTER_PING");
	sbFltNotFull = new CCheckBox(m_Gui, 150, 410, 0, 0, 0, "", "FILTER_NOT_FULL");
	sbFltLocation = new CEditBox(m_Gui, 305, 380, 80, 20, "", "FILTER_LOCATION");
	sbFltMode = new CEditBox(m_Gui, 305, 410, 80, 20, "", "FILTER_MODE");
	sbFltNotEmpty = new CCheckBox(m_Gui, 480, 385, 0, 0, 0, "", "FILTER_NOT_EMPTY");
	sbFltNoPassword = new CCheckBox(m_Gui, 480, 410, 0, 0, 1, "", "FILTER_NO_PASSWORD");

	sbFltPing->AddElement("< 50", "50");
	sbFltPing->AddElement("50 - 100", "50100");
	sbFltPing->AddElement("100 - 200", "100200");
	sbFltPing->AddElement("200 - 500", "200500");
	sbFltPing->AddElement("> 500", "500");

	fServBrowser->AddElement(sbServList);
	fServBrowser->AddElement(sbPlayerList);
	fServBrowser->AddElement(sbTab[0]);
	fServBrowser->AddElement(sbTab[1]);
	fServBrowser->AddElement(sbTab[2]);
	fServBrowser->AddElement(sbTab[3]);
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
	Debug("FMPGUI::Load >> Create chat");
	fChat = new CWindow(m_Gui, 10, 10, 200, 300, "FOUR-MP CHAT");
	cc_tChat = new CTextBox(m_Gui, 0, 0, 200, 258, NULL, NULL, CALLBACKS::Chat);
	cc_tEnter = new CEditBox(m_Gui, 0, 257, 160, 0, NULL, NULL, CALLBACKS::Chat);
	cc_bEnter = new CButton(m_Gui, 160, 257, 40, 0, "SEND", NULL, CALLBACKS::Chat);
	fChat->AddElement(cc_tChat);
	fChat->AddElement(cc_tEnter);
	fChat->AddElement(cc_bEnter);
	fChat->SetVisible( 1 );

	// Create Option
	Debug("FMPGUI::Load >> Create options");
	fOption = new CWindow(m_Gui, 200, 200, 400, 300, "FOUR-MP OPTIONS");
	CText * tInfo = new CText(m_Gui, 10, 10, 300, 100, "WAIT OR CREATE INTERFACE");
	fOption->AddElement(tInfo);
	fOption->SetVisible( 0 );

	// Create login window
	Debug("FMPGUI::Load >> Create login");
	fUserLogin = new CWindow(m_Gui, 10, 10, 300, 200, "USER LOGIN", "WND_USER_LOGIN");
	upLoginInfo = new CText(m_Gui, 20, 15, 280, 40, "Please login or Regsiter", "UP_INFO");
	upLogin = new CEditBox(m_Gui, 100, 45, 180, 0, "", "UP_LOGIN");
	upPassword = new CEditBox(m_Gui, 100, 75, 180, 0, "", "UP_PASSWORD");
	upPassword->HideContent(1);
	upStrLogin = new CText(m_Gui, 20, 50, 100, 20, "Login", "UP_STR_LOGIN");
	upStrPass = new CText(m_Gui, 20, 80, 100, 20, "Password", "UP_STR_PASSWORD");
	upSendLogin = new CButton(m_Gui, 10, 140, 135, 0, "Login", "UP_BTN_LOGIN", CALLBACKS::Login);
	upShowRegister = new CButton(m_Gui, 150, 140, 135, 0, "Register", "UP_BTN_REGISTER", CALLBACKS::ShowRegister);
	upRemeberMe = new CCheckBox(m_Gui, 70, 110, 0, 0, 0, "    Remember me", "UP_CB_REMEBER"); 

	fUserLogin->AddElement(upLoginInfo);
	fUserLogin->AddElement(upLogin);
	fUserLogin->AddElement(upPassword);
	fUserLogin->AddElement(upStrLogin);
	fUserLogin->AddElement(upStrPass);
	fUserLogin->AddElement(upSendLogin);
	fUserLogin->AddElement(upShowRegister);
	fUserLogin->AddElement(upRemeberMe);
	fUserLogin->SetCloseButton(0);

	// Create register window
	Debug("FMPGUI::Load >> Create register");
	fUserRegister = new CWindow(m_Gui, 10, 220, 300, 280, "USER REGISTER", "WND_USER_REG");
	urLogin = new CEditBox(m_Gui, 100, 50, 180, 0, "", "REG_LOGIN");
	urPass = new CEditBox(m_Gui, 100, 80, 180, 0, "", "REG_PASSWORD");
	urConfirm = new CEditBox(m_Gui, 100, 110, 180, 0, "", "REG_CONFIRM");
	urEmail = new CEditBox(m_Gui, 100, 140, 180, 0, "", "REG_EMAIL");
	urNick = new CEditBox(m_Gui, 100, 170, 180, 0, "", "REG_NICK");
	urLoginText = new CText(m_Gui, 10, 55, 100, 20, "Login", "REG_LOGIN_TEXT");
	urPassText = new CText(m_Gui, 10, 85, 100, 20, "Password", "REG_PASSWORD_TEXT");
	urConfirmText = new CText(m_Gui, 10, 115, 100, 20, "Confirm", "REG_CONFIRM_TEXT");
	urEmailText = new CText(m_Gui, 10, 145, 100, 20, "Email", "REG_EMAIL_TEXT");
	urNickText = new CText(m_Gui, 10, 175, 100, 20, "Nick", "REG_NICK_TEXT");
	urInfoText = new CText(m_Gui, 20, 15, 280, 20, "Please register to play", "UR_INFO");
	urSendReg = new CButton(m_Gui, 10, 220, 135, 0, "Register", "UR_BTN_REG", CALLBACKS::Register);
	urCancelReg = new CButton(m_Gui, 150, 220, 135, 0, "Cancel", "UR_BTN_CANCEL", CALLBACKS::HideRegister);

	urPass->HideContent(1);
	urConfirm->HideContent(1);

	fUserRegister->AddElement(urLogin);
	fUserRegister->AddElement(urPass);
	fUserRegister->AddElement(urConfirm);
	fUserRegister->AddElement(urEmail);
	fUserRegister->AddElement(urNick);
	fUserRegister->AddElement(urLoginText);
	fUserRegister->AddElement(urPassText);
	fUserRegister->AddElement(urConfirmText);
	fUserRegister->AddElement(urEmailText);
	fUserRegister->AddElement(urNickText);
	fUserRegister->AddElement(urInfoText);
	fUserRegister->AddElement(urSendReg);
	fUserRegister->AddElement(urCancelReg);
	fUserRegister->SetVisible(0);
	fUserRegister->SetCloseButton(0);

	// Create messages window
	Debug("FMPGUI::Load >> Create messages window");
	fInfo = new CWindow(m_Gui, 300, 250, 200, 100, "Information", "WND_INFO");
	CText *iTxt = new CText(m_Gui, 10, 10, 180, 60, "INFO", "INFO_TEXT");
	CButton *iBt = new CButton(m_Gui, 50, 45, 100, 0, "OK", "INFO_OK", CALLBACKS::InfoOK);
	fInfo->AddElement(iTxt);
	fInfo->AddElement(iBt);
	fInfo->SetVisible(0);

	Debug("FMPGUI::Load >> Load console");
	conwindow.Load();

	Debug("FMPGUI::Load >> Update GUI");
	m_Gui->SetVisible( true );
	m_Gui->UpdateFromFile("FMP\\GUIStyle.xml");
	Debug("FMPGUI::Load >> Disable tab");

	sbTab[tab]->SetEnabled(0);
	GuiReady = 1;
	Debug("FMPGUI::Load complete");
}

void FMPGUI::HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if( GuiReady )
	{
		CMouse *mouse = m_Gui->GetMouse();
		if(mouse) mouse->HandleMessage(Msg, wParam, lParam);

		CKeyboard *keybd = m_Gui->GetKeyboard();
		if(keybd) keybd->HandleMessage(Msg, wParam, lParam);
	}
}

void FMPGUI::MoveMouse(int x, int y)
{
	if( GuiReady )
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
	if( GuiReady ) m_Gui->Draw();
}

DWORD FMPGUI::GetWidth()
{
	return s_iWidth;
}

DWORD FMPGUI::GetHeight()
{
	return s_iHeight;
}