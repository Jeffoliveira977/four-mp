#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "../../GUI/CGUI.h"

class FMPGUI
{
	CGUI *m_Gui;
	bool g_Mouse[3];
	bool GuiReady;
	DWORD s_iWidth, s_iHeight;
	bool logged;

	friend class ConsoleWindow;
public:
	FMPGUI();

	void Load(IDirect3DDevice9 * g_pDevice);
	void HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
	void MoveMouse(int x, int y);
	void Draw();

	bool IsLogged();
	void Logged();

	DWORD GetWidth();
	DWORD GetHeight();
};