/// \file
/// \brief Source file that contains implementation of the ConsoleWindow class.
/// \details See class description.
/// \author FaTony

#include "ConsoleWindow.h"
#include "..\..\Shared\Console\ConsoleCore.h"
#include "d3d9\gui.h"

extern ConsoleCore concore;
extern FMPGUI Gui;
extern ConsoleWindow conwindow;

ConsoleWindow::ConsoleWindow(void)
{
	IsLoaded = false;
}

ConsoleWindow::~ConsoleWindow(void)
{
	if (IsLoaded)
	{
		delete outputbox;
		delete inputbox;
		delete submitbutton;
		delete mainwindow;
	}
}

void ConsoleWindow::Load(void)
{
	mainwindow = new CWindow(Gui.m_Gui, 412, 25, 587, 568, "Console");
	outputbox = new CTextBox(Gui.m_Gui, 8, 8, 571, 492);
	outputbox->ShowSlider(true);
	inputbox = new CEditBox(Gui.m_Gui, 8, 508, 492, 24, NULL, NULL, InputBoxCallback);
	submitbutton = new CButton(Gui.m_Gui, 508, 508, 64, 24, "Submit", NULL, SubmitButtonCallback);
	mainwindow->AddElement(outputbox);
	mainwindow->AddElement(inputbox);
	mainwindow->AddElement(submitbutton);
	IsLoaded = true;
}

void ConsoleWindow::Print(const char *string, ...)
{
	if (!IsLoaded)
	{
		return;
	}
	if (string == NULL)
	{
		return;
	}
	va_list arglist; 
    va_start(arglist, string);
	int stringsize = _vscprintf(string, arglist);
	char *tempstring = (char *)calloc(stringsize + 1, sizeof(char));
	vsprintf(tempstring, string, arglist);
	va_end(arglist);
	outputbox->AddString(tempstring);
	free(tempstring);
}

void InputBoxCallback(CElement *pElement, CMSG msg, int Param)
{
}

void SubmitButtonCallback(CElement *pElement, CMSG msg, int Param)
{
	if (msg != CLICK)
	{
		return;
	}
	conwindow.outputbox->AddString(conwindow.inputbox->GetString());
}