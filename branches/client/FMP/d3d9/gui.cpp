#include <windows.h>
#include "gui.h"

extern IDirect3DDevice9 * g_pDevice;
bool g_Mouse[3] = {0, 0, 0};

void GuiLoad()
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

	CWindow * cWin = new CWindow(10, 200, 340, 220, "FOUR-MP TEAM", "WINDOW_TEST");

	int widths[] = {100, 200};
	CListView * cLW = new CListView(10, 10, widths, 180, 2);

	cLW->SetTitle("ID");
	cLW->SetTitle("NAME");

	cLW->PutStr("1", 0);
	cLW->PutStr("WNeZRoS", 1);
	cLW->PutStr("2", 0);
	cLW->PutStr("VoLT", 1);
	cLW->PutStr("3", 0);
	cLW->PutStr("009", 1);
	cLW->PutStr("4", 0);
	cLW->PutStr("FaTony", 1);
	cLW->PutStr("5", 0);
	cLW->PutStr("XAOC", 1);

	cWin->AddElement(cLW);
	gpGui->AddWindow(cWin);

	gpGui->SetVisible( true );
	gpGui->GetMouse()->SetPos(100, 100);
}

void GuiHandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if( gpGui->IsVisible() )
	{
		gpGui->GetMouse()->HandleMessage(Msg, wParam, lParam);
		gpGui->GetKeyboard()->HandleMessage(Msg, wParam, lParam);
	}
}

void GuiMoveMouse(int x, int y)
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

void GuiDraw()
{
	gpGui->Draw();
}