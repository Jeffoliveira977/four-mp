#include "../masterserver.h"
#include <windows.h>
#include "gui.h"
#include "../../GUI/CGUI.h"
#include "../main.h"
#include "../log.h"
#include "../ConsoleWindow.h"
#include "../Hook/classes.h"
#include "../../../Shared/RakNet/RakPeerInterface.h"

#include <vector>

extern ConsoleWindow conwindow;
extern RakPeerInterface *net;

// Windows
CWindow * fServBrowser;
CWindow * fChat;
CWindow * fOption;
CWindow * fUserRegister;
CWindow * fUserLogin;
CWindow * fInfo;
CWindow * fEnterNick;
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
// Enter nick
CEditBox * enNick; 
CButton * enOK;

std::vector<MasterServerInfo*> server_list;
std::vector<MasterServerInfo*> server_list_fav;

int tab = 3;

MasterServer fmpms;
extern FMPHook HOOK;
extern FMPGUI Gui;

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
			server_list.clear();

			bool r = fmpms.QueryServerList(0, tab == 2, sbFltNotEmpty->GetChecked(), sbFltNotFull->GetChecked(), 
				sbFltNoPassword->GetChecked(),	0, 0, sbFltMode->GetString().size()>0?(char*)sbFltMode->GetString().c_str():0, sbFltLocation->GetString().size()?(char*)sbFltLocation->GetString().c_str():0);

			if(!r) { fInfo->GetElementByString("INFO_TEXT", 1)->SetString("Load server list failed"); fInfo->SetVisible(1); }
			else
			{
				char *tmp = new char[128];
				MasterServerInfo *msi;
				for(int i = 0; i < fmpms.GetNumServers(); i++)
				{
					msi = fmpms.GetServerInfo(i);
					if(!msi) continue;
					net->Ping(msi->ip, msi->port, false); 
				}
			}
		}
		else if(tab == 3)
		{
			for(int i = 0; i < (int)server_list_fav.size(); i++)
			{
				if(server_list_fav[i])
				{
					Gui.UpdateServer(server_list_fav[i]);
					net->Ping(server_list_fav[i]->ip, server_list_fav[i]->port, false); 
				}
			}
		}
		else if(tab == 1)
		{
			net->Ping("255.255.255.255", 7777, false);
		}
		Debug("CALLBACKS::Refresh complete");
	}
	void GetLAN( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		Debug("CALLBACKS::GetLAN called");

		sbServList->Clear();
		server_list.clear();

		sbTab[tab]->SetEnabled(1);
		tab = 1;
		sbTab[tab]->SetEnabled(0);

		net->Ping("255.255.255.255", 7777, false);

		Debug("CALLBACKS::GetLAN complete");
	}
	void GetInet( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		if(!Gui.IsLogged()) 
		{
			fUserLogin->SetVisible(1);
			fServBrowser->SetVisible(0);
			return;
		}

		Debug("CALLBACKS::GetInet called");

		sbTab[tab]->SetEnabled(1);
		tab = 0;
		sbTab[tab]->SetEnabled(0);

		if(fmpms.GetNumServers() > 0)
		{
			char tmp[64];
			MasterServerInfo *msi;
			sbServList->Clear();
			server_list.clear();

			for(int i = 0; i < fmpms.GetNumServers(); i++)
			{
				msi = fmpms.GetServerInfo(i);
				if(!msi) continue;
				net->Ping(msi->ip, msi->port, false); 
			}
		}
		Debug("CALLBACKS::GetInet complete");
	}
	void GetVIP( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		if(!Gui.IsLogged()) 
		{
			fUserLogin->SetVisible(1);
			fServBrowser->SetVisible(0);
			return;
		}

		Debug("CALLBACKS::GetVIP called");

		sbTab[tab]->SetEnabled(1);
		tab = 2;
		sbTab[tab]->SetEnabled(0);

		if(fmpms.GetNumServers() > 0)
		{
			char tmp[64];
			MasterServerInfo *msi;
			sbServList->Clear();
			server_list.clear();

			for(int i = 0; i < fmpms.GetNumServers(); i++)
			{
				msi = fmpms.GetServerInfo(i);
				if(!msi) continue;
				if(msi->vip == 1)
					net->Ping(msi->ip, msi->port, false); 
			}
		}
		Debug("CALLBACKS::GetVIP complete");
	}
	void GetFavourite( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK) return;
		/*if(!Gui.IsLogged()) 
		{
			fUserLogin->SetVisible(1);
			fServBrowser->SetVisible(0);
			return;
		}*/
		Debug("CALLBACKS::GetFavourite called");

		sbServList->Clear();
		server_list.clear();

		sbTab[tab]->SetEnabled(1);
		tab = 3;
		sbTab[tab]->SetEnabled(0);

		if(server_list_fav.size() == 0) return;

		for(int i = 0; i < (int)server_list_fav.size(); i++)
		{
			if(server_list_fav[i])
			{
				Gui.UpdateServer(server_list_fav[i]);
				net->Ping(server_list_fav[i]->ip, server_list_fav[i]->port, false); 
			}
		}

		Debug("CALLBACKS::GetFavourite complete");
	}
	void Login( CElement * pElement, CMSG msg, int param )
	{
		if(msg != CLICK && msg != END) return;
		Debug("CALLBACKS::Login called");

		std::string login = upLogin->GetString();
		std::string pass = upPassword->GetString();

		if(login.length() < 3 || pass.length() < 6) return;

		bool r = fmpms.QueryUserLogin((char*)login.c_str(), (char*)pass.c_str());
		if(r) 
		{
			fUserLogin->SetVisible(0);
			fServBrowser->SetVisible(1);
			Gui.Logged();
			strcpy_s(Conf.Name, 32, login.c_str());
		}
		else Gui.Message("Can't login");

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

		std::string login = urLogin->GetString();
		std::string pass = urPass->GetString();
		std::string confirm = urConfirm->GetString();
		std::string email = urEmail->GetString();
		std::string nick = urNick->GetString();

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
	void FilterPing(CElement *pElement, CMSG msg, int Param)
	{
		if(msg != CLICK) return;

	}
	void Connect(CElement *pElement, CMSG msg, int Param)
	{
		if(msg != CLICK) return;

		Debug("CALLBACKS::Connect called");
		if(!sbServList) return;

		int sel = sbServList->GetSelected();
		Debug("Selected %d (%d/%d)", sel, sbServList->GetSize(), server_list.size());
		if(sel >= 0)
		{
			if(!Gui.IsLogged())
			{
				fEnterNick->SetVisible(1);
				return;
			}
			MasterServerInfo *tmp_msi = server_list.at(sel);
			Debug("FROM LIST - Server: [%s:%d]", tmp_msi->ip, tmp_msi->port);
			HOOK.ConnectToServer(tmp_msi->ip, tmp_msi->port);
		}
		Debug("CALLBACKS::Connect complete");
	}
	void EnterNick(CElement *pElement, CMSG msg, int Param)
	{
		if(msg != CLICK && msg != END) return;

		std::string n = enNick->GetString();
		if(n.length() < 4) 
		{
			if(n.length() > 0)
				Gui.Message("Small nick");
			return;
		}
		
		strcpy_s(Conf.Name, 32, n.c_str());
		fEnterNick->SetVisible(0);

		if(!sbServList) return;

		int sel = sbServList->GetSelected();
		if(sel >= 0)
		{
			MasterServerInfo *tmp_msi = server_list.at(sel);
			Debug("FROM LIST - Server: [%s:%d]", tmp_msi->ip, tmp_msi->port);
			HOOK.ConnectToServer(tmp_msi->ip, tmp_msi->port);
		}
	}
	void ServerBrowser(CElement *pElement, CMSG msg, int Param)
	{
		if(msg == END)
		{
			if(clientstate.game != GameStateOffline && clientstate.game != GameStateConnecting)
			{
				if(fChat->IsVisible()) fChat->SetVisible(0);

				if(!conwindow.IsVisible()) 
				{
					clientstate.input = InputStateGame;
					HOOK.InputFreeze(0);
				}
			}
			else if(!Gui.IsLogged())
			{
				fUserLogin->SetVisible(1);
				fServBrowser->SetVisible(0);
			}
		}
	}
	void UserLogin(CElement *pElement, CMSG msg, int Param)
	{
		if(msg == END)
		{
			fUserLogin->SetVisible(0);
			fServBrowser->SetVisible(1);
			sbTab[1]->SetEnabled(0);
			sbTab[tab]->SetEnabled(1);
			tab = 1;
		}
	}
	void AddFav(CElement *pElement, CMSG msg, int Param)
	{
		if(msg != CLICK) return;

		/*if(!Gui.IsLogged()) 
		{
			fUserLogin->SetVisible(1);
			fServBrowser->SetVisible(0);
			return;
		}*/

		if(sbEnterIP->GetString().length() > 0 && sbEnterPort->GetString().length() > 0)
		{
			// Add entered server
			MasterServerInfo * tmp_msi = new MasterServerInfo;
			memset(tmp_msi, 0, sizeof(MasterServerInfo));
			strcpy_s(tmp_msi->ip, 64, sbEnterIP->GetString().c_str());
			tmp_msi->port = atoi(sbEnterPort->GetString().c_str());
			sprintf_s(tmp_msi->name, 128, "%s:%d", tmp_msi->ip, tmp_msi->port);

			if(tab == 3)
			{
				Gui.UpdateServer(tmp_msi);
				net->Ping(tmp_msi->ip, tmp_msi->port, false);
			}
			server_list_fav.push_back(tmp_msi);

			sbEnterIP->SetString("");
			sbEnterPort->SetString("");
		}
	}
};

FMPGUI::FMPGUI()
{
	g_Mouse[0] = 0;
	g_Mouse[1] = 0;
	g_Mouse[2] = 0;
	GuiReady = 0;
	s_iWidth = 800;
	s_iHeight = 600;
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
	fServBrowser = new CWindow(m_Gui, s_iWidth/2-375, s_iHeight/2-250, 750, 500, "SERVER BROWSER", "SERVER_BROWSER", CALLBACKS::ServerBrowser);
	
	int ServerWidth[6] = {16, 210, 60, 60, 100, 100};
	sbServList = new CListView(m_Gui, 0, 29, ServerWidth, 350, 6, NULL, "SERVER_LIST", CALLBACKS::ServerList);
	sbServList->SetColumnImage(0, "FMPGUI/Pass.png", "FMPGUI/noPass.png", "FMPGUI/Pass.png", 16, 16);

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

	sbConnect = new CButton(m_Gui, 560, 380, 80, 0, "Connect", "MAN_CONNECT", CALLBACKS::Connect);
	sbRefresh = new CButton(m_Gui, 660, 380, 80, 0, "Refresh", "MAN_REFRESH", CALLBACKS::Refresh);
	sbAddToFav = new CButton(m_Gui, 600, 405, 120, 0, "Add to favourites", "MAN_ADDFAV", CALLBACKS::AddFav);

	sbFltPing = new CDropDown(m_Gui, 150, 380, 80, 20, "Ping", "FILTER_PING", CALLBACKS::FilterPing);
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
	fServBrowser->SetVisible(0);

	// Create Chat
	Debug("FMPGUI::Load >> Create chat");
	fChat = new CWindow(m_Gui, 10, 10, 300, 400, "FOUR-MP CHAT");
	cc_tChat = new CTextBox(m_Gui, 10, 10, 280, 320, NULL, NULL, CALLBACKS::Chat);
	cc_tEnter = new CEditBox(m_Gui, 10, 330, 220, 0, NULL, NULL, CALLBACKS::Chat);
	cc_bEnter = new CButton(m_Gui, 230, 330, 60, 0, "SEND", NULL, CALLBACKS::Chat);
	fChat->AddElement(cc_tChat);
	fChat->AddElement(cc_tEnter);
	fChat->AddElement(cc_bEnter);
	fChat->SetVisible( 0 );

	// Create Option
	Debug("FMPGUI::Load >> Create options");
	fOption = new CWindow(m_Gui, 200, 200, 400, 300, "FOUR-MP OPTIONS");
	CText * tInfo = new CText(m_Gui, 10, 10, 300, 100, "WAIT OR CREATE INTERFACE");
	fOption->AddElement(tInfo);
	fOption->SetVisible( 0 );

	// Create login window
	Debug("FMPGUI::Load >> Create login");
	fUserLogin = new CWindow(m_Gui, s_iWidth/2-150, s_iHeight/2-100, 300, 200, "USER LOGIN", "WND_USER_LOGIN", CALLBACKS::UserLogin);
	upLoginInfo = new CText(m_Gui, 20, 15, 280, 40, "Please login or Regsiter", "UP_INFO");
	upLogin = new CEditBox(m_Gui, 100, 45, 180, 0, "", "UP_LOGIN", CALLBACKS::Login);
	upPassword = new CEditBox(m_Gui, 100, 75, 180, 0, "", "UP_PASSWORD", CALLBACKS::Login);
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

	// Create register window
	Debug("FMPGUI::Load >> Create register");
	fUserRegister = new CWindow(m_Gui, s_iWidth/2-150, s_iHeight/2-140, 300, 280, "USER REGISTER", "WND_USER_REG");
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
	fInfo = new CWindow(m_Gui, s_iWidth/2-100, s_iHeight/2-50, 200, 100, "Information", "WND_INFO");
	CText *iTxt = new CText(m_Gui, 10, 10, 180, 60, "INFO", "INFO_TEXT");
	CButton *iBt = new CButton(m_Gui, 50, 45, 100, 0, "OK", "INFO_OK", CALLBACKS::InfoOK);
	fInfo->AddElement(iTxt);
	fInfo->AddElement(iBt);
	fInfo->SetVisible(0);

	fEnterNick = new CWindow(m_Gui, s_iWidth/2-100, s_iHeight/2-40, 200, 80, "Enter nick", "WND_ENTER_NICK");
	enNick = new CEditBox(m_Gui, 10, 10, 130, 0, NULL, NULL, CALLBACKS::EnterNick);
	enOK = new CButton(m_Gui, 150, 10, 40, 0, "OK", NULL, CALLBACKS::EnterNick);
	fEnterNick->AddElement(enNick);
	fEnterNick->AddElement(enOK);
	fEnterNick->SetVisible(0);

	Debug("FMPGUI::Load >> Load console");
	conwindow.Load();
	conwindow.Hide();

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

void FMPGUI::Logged()
{
	logged = 1;
}

bool FMPGUI::IsLogged()
{
	return logged;
}

void FMPGUI::UpdateServer(MasterServerInfo *msi)
{
	int index = -1;
	char tmp[32];

	for(int i = 0; i < (int)server_list.size(); i++)
		if(strcmp(server_list[i]->ip, msi->ip) == 0 && server_list[i]->port == msi->port)
		{
			index = i;
			break;
		}

	sbServList->PutStr((msi->password?"1":"0"), 0, index);
	sbServList->PutStr(msi->name, 1, index);
	sprintf_s(tmp, 32, "%d/%d", msi->players, msi->maxplayers);
	sbServList->PutStr(tmp, 2, index);
	sprintf_s(tmp, 32, "%d", msi->ping);
	sbServList->PutStr(tmp, 3, index);
	sbServList->PutStr(msi->mode, 4, index);
	sbServList->PutStr(msi->loc, 5, index);

	if(index == -1)
	{
		server_list.push_back(msi);
	}
	else
	{
		//delete server_list[index];
		server_list[index] = msi;
	}
}

void FMPGUI::Message(char *data)
{
	fInfo->GetElementByString("INFO_TEXT", 1)->SetString(data);
	fInfo->SetVisible(1);
}
