/// \file
/// \brief Header file that describes the ConsoleWindow class.
/// \details See class description.
/// \author FaTony

#pragma once

#include <windows.h>
#include "..\GUI\CGUI.h"

class ConsoleWindow
{
public:
	ConsoleWindow(void);
	~ConsoleWindow(void);
	void Load(void);
	bool IsVisible(void);
	void Show(void);
	void Hide(void);
	void Print(const char *string, ...);
private:
	bool IsLoaded;
	CWindow *mainwindow;
	CTextBox *outputbox;
	CEditBox *inputbox;
	CButton *submitbutton;
	friend void InputBoxCallback(CElement *pElement, CMSG msg, int Param);
	friend void SubmitButtonCallback(CElement *pElement, CMSG msg, int Param);
};

void InputBoxCallback(CElement *pElement, CMSG msg, int Param);
void SubmitButtonCallback(CElement *pElement, CMSG msg, int Param);