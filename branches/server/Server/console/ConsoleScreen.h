#pragma once

#ifdef WIN32
#include <windows.h>
#endif

class ConsoleScreen
{
public:
	ConsoleScreen(void);
	~ConsoleScreen(void);
	void SetCaption(const char *string);
	void CheckUserInput(void); //Handles keyboard input
	void Print(const char *string, ...); //Prints string to the output page
private:
	char *caption;
	char **outputbuffer;
	char **inputbuffer;
#ifdef WIN32
	HANDLE outputhandle;
#endif
	unsigned short outputbuffersize; //Holds current size of the output buffer (in lines)
	unsigned short maxoutputbuffersize; // Holds maximum size of the output buffer (in lines)
	unsigned char inputbuffersize; //Holds curent size of the input buffer (in lines)
	unsigned char maxinputbuffersize; // Holds maximum size of the input buffer (in lines)
	unsigned char inputbufferposition[2]; //Holds current cursor position in the input buffer ([0] - Line index, [1] - Character index)
	unsigned char pagesize; //How many lines will be shown on one page (excluding caption and input line)
	unsigned short screenposition; //Holds index of the first line in the output buffer that will be shown on the current page
	void ClearScreen(void); //Clears the screen
	void ClearInputLine(void); //Clears input line
	void SetCursorPosition(const unsigned char x, const unsigned char y); //Sets cursor position on the console screen
	unsigned char GetInputLineScreenPosition(void); //Gets y coordinate of the input line on the console screen
	void SetFinalCursorPosition(void); //Sets cursor position to where user should see it
	void PrintCaption(void); //Prints caption
	void PrintOutputPage(void); //Prints output page
	void PrintInputLine(void); //Prints input line
	void UpdateScreen(void); //Performs full update of screen
	void GoOnePageUp(void); //Goes up one page
	void GoOnePageDown(void); //Goes down one page
	void InputHistoryBack(void); //Goes one entry back in the input history
	void InputHistoryForward(void); //Goes one entry forward in the input history
	void WriteToOutputBuffer(const char *string); //Appends string to the output buffer
	void AppendToOutputBuffer(const char *string); //Writes prepared string to the output buffer (don't touch)
	void FillInputBufferFromHistory(void); //Replaces current input with one from history
	void AddCharToInputBuffer(const int ch); //Adds character to the current position in the input buffer
	void BackspaceCharInInputBuffer(void); //Backspaces from the current position in the input buffer
	void MoveCursorLeft(void); //Moves cursor left in the input buffer
	void MoveCursorRight(void); //Moves cursor right in the input buffer
	void DeleteCharInInputBuffer(void); //Deletes character from the current position in the input buffer
	void AcceptUserInput(void); //Accepts inputted line
	void WriteToInputBuffer(void); //Appends current string to the input buffer
};
