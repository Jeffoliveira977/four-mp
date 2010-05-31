#include <windows.h>
#include "../main.h"
#include "window.h"
#include "../chat.h"
#include "../../Shared/ClientCore.h"
#include "../Hook/classes.h"
#include "../d3d9/Gui.h"
#include "../ConsoleWindow.h"

extern ClientCore client;
extern FMPHook HOOK;
extern FMPGUI Gui;
extern ConsoleWindow conwindow;
extern CWindow * fServBrowser;
extern CWindow * fChat;
WNDPROC gameProc;
DWORD g_iMinimized = 1;

LRESULT DefWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	if(Msg == WM_ACTIVATEAPP)
	{
		if(wParam) g_iMinimized = GetTickCount();
		else g_iMinimized = 0;
	}

	if (client.GetGameState() == GameStateLoading || g_iMinimized != 1)
	{
		return CallWindowProc(gameProc, hWnd, Msg, wParam, lParam);
	}
	InputState state = client.GetInputState();
	switch (state)
	{
	case InputStateGame:
		{
			if (Msg == WM_KEYUP)
			{
				switch (wParam)
				{
				case VK_F5:
					{
						client.SetInputState(InputStateGui);
						fServBrowser->SetVisible(1);
						HOOK.InputFreeze(1);
						break;
					}
				case VK_F6:
					{
						client.SetInputState(InputStateChat);
						HOOK.InputFreeze(1);
						break;
					}
				case 192:
					{
						client.SetInputState(InputStateGui);
						HOOK.InputFreeze(1);
						conwindow.Show();
						break;
					}
				}
			}
			return CallWindowProc(gameProc, hWnd, Msg, wParam, lParam);
			break;
		}
	case InputStateChat:
		{
			if (Msg == WM_KEYUP)
			{
				if(wParam == VK_F6)
				{
					client.SetInputState(InputStateGame); 
					HOOK.InputFreeze(0);
					break; 
				}
				else if(wParam == VK_F5)
				{
					client.SetInputState(InputStateGui);
					HOOK.InputFreeze(0);
					fChat->SetVisible(1);
					break;
				}

				if(wParam == 13)
				{
					if(strlen(enterMsg) != 0)
						SendChatMessage();

					client.SetInputState(InputStateGame); 
					HOOK.InputFreeze(0);
				}
				else if(wParam == 8)
				{
					if(strlen(enterMsg) > 0)
						enterMsg[strlen(enterMsg)-1] = 0;
				}
				else
				{
					if(strlen(enterMsg) != 255)
					{
						int enterChat = strlen(enterMsg);

						BYTE bKeys[256] = { 0 };
						GetKeyboardState( bKeys );

						WORD wKey = 0;
						ToAscii( wParam, HIWORD( lParam )&0xFF, bKeys, &wKey, 0 );

						if(wKey < 32) break;

						enterMsg[enterChat] = wKey;

						enterChat++;
						enterMsg[enterChat] = 0;
					}
				}
			}
			break;
		}
	case InputStateGui:
		{
			if (Msg == WM_KEYUP)
			{
				switch (wParam)
				{
				case VK_F5:
					{
						//fServBrowser->SetVisible(!fServBrowser->IsVisible() && Gui.IsLogged());
						//if(fChat->IsVisible()) fChat->SetVisible(0);

						//if(clientstate.game != GameStateOffline && clientstate.game != GameStateConnecting)
						//	if(!conwindow.IsVisible()) 
						//	{
						client.SetInputState(InputStateGame);
						HOOK.InputFreeze(0);
						//	}
						break;
					}
				case VK_F6:
					{
						/*if(clientstate.game != GameStateOffline && clientstate.game != GameStateConnecting)
						{
							Log::Info("Chat in GUI not working");
							if(!fChat->IsVisible())
								fChat->SetVisible(1);
							else
								fChat->SetVisible(0);
						}*/
						break;
					}
				case 192:
					{
						if(conwindow.IsVisible())
						{
							conwindow.Hide();
							client.SetInputState(InputStateGame);
							HOOK.InputFreeze(0);
						}
						else
						{
							conwindow.Show();
						}
						break;
					}
				}
			}
			if(client.IsRunning()) Gui.HandleMessage(Msg, wParam, lParam);
			break;
		}
	}
	return CallWindowProc(gameProc, hWnd, Msg, wParam, lParam);
}