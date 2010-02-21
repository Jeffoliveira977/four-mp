#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "../GUI/CGUI.h"

void GuiLoad();
void GuiHandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
void GuiMoveMouse(int x, int y);
void GuiDraw();