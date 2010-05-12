#include <windows.h>
#include "../main.h"
#include "window.h"
#include "../chat.h"
#include "../Hook/classes.h"
#include "../d3d9/Gui.h"
#include "../ConsoleWindow.h"


extern FMPHook HOOK;
extern FMPGUI Gui;
extern ConsoleWindow conwindow;
extern CWindow * fServBrowser;
extern CWindow * fChat;
WNDPROC gameProc;

LRESULT DefWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (clientstate.input)
	{
	case InputStateGame:
		{
			if (Msg == WM_KEYUP)
			{
				switch (wParam)
				{
				case VK_F5:
					{
						clientstate.input = InputStateGui;
						fServBrowser->SetVisible(1);
						HOOK.InputFreeze(1);
						break;
					}
				case VK_F6:
					{
						clientstate.input = InputStateChat;
						HOOK.InputFreeze(1);
						break;
					}
				case 192:
					{
						clientstate.input = InputStateGui;
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
					clientstate.input = InputStateGame; 
					HOOK.InputFreeze(0);
					break; 
				}
				else if(wParam == VK_F5)
				{
					clientstate.input = InputStateGui;
					HOOK.InputFreeze(0);
					fChat->SetVisible(1);
					break;
				}

				if(wParam == 13)
				{
					if(strlen(enterMsg) != 0)
						SendChatMessage();

					clientstate.input = InputStateGame; 
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
						BYTE ks[256];
						GetKeyboardState(ks);
						ToAsciiEx(wParam,0,ks, (LPWORD)&enterMsg[enterChat],0, GetKeyboardLayout(GetCurrentThreadId()));
						if(enterMsg[enterChat] < 32) enterChat--;
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
						clientstate.input = InputStateGame;
						HOOK.InputFreeze(0);
						//	}
						break;
					}
				case VK_F6:
					{
						if(clientstate.game != GameStateOffline && clientstate.game != GameStateConnecting)
						{
							Log("Chat in GUI not working");
							if(!fChat->IsVisible())
								fChat->SetVisible(1);
							else
								fChat->SetVisible(0);
						}
						break;
					}
				case 192:
					{
						if(conwindow.IsVisible())
						{
							conwindow.Hide();
							clientstate.input = InputStateGame;
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
			Gui.HandleMessage(Msg, wParam, lParam);
			break;
		}
	}
	return CallWindowProc(gameProc, hWnd, Msg, wParam, lParam);
}