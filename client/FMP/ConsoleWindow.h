/// \file
/// \brief Header file that describes the ConsoleWindow class.
/// \details See class description.
/// \author FaTony

#pragma once

#include <windows.h>
#include "..\GUI\CGUI.h"
#include "log.h"

class ConsoleWindow : public Logger
{
public:
	ConsoleWindow(void);
	~ConsoleWindow(void);
	void Load(void);
	bool IsVisible(void);
	void Show(void);
	void Hide(void);
	virtual void Log(const char *type, const char *string, char* = 0);
private:
	bool IsLoaded;
	CWindow *mainwindow;
	CTextBox *outputbox;
	CEditBox *inputbox;
	CButton *submitbutton;
	void SubmitText(void);
	friend void InputBoxCallback(CElement *pElement, CMSG msg, int Param);
	friend void SubmitButtonCallback(CElement *pElement, CMSG msg, int Param);
};

void InputBoxCallback(CElement *pElement, CMSG msg, int Param);
void SubmitButtonCallback(CElement *pElement, CMSG msg, int Param);
void PrintToConsole(const char *string, ...);