#pragma once

class Console
{
public:
	Console(void);
	~Console(void);
	void CheckUserInput(void); //Handles keyboard input
	void Print(const char *string, ...); //Prints string to the output page
	void Debug(const char *string, ...);
private:
	char **outputbuffer;
	char **inputbuffer;
	unsigned short outputbuffersize; //Holds current size of the output buffer (in lines)
	unsigned short maxoutputbuffersize; // Holds maximum size of the output buffer (in lines)
	unsigned char inputbuffersize; //Holds curent size of the input buffer (in lines)
	unsigned char maxinputbuffersize; // Holds maximum size of the input buffer (in lines)
	unsigned char inputbufferposition[2]; //Holds current cursor position in the input buffer
	unsigned char pagesize; //How many lines will be shown on one page (excluding caption and input line)
	unsigned short screenposition; //Holds first line in the output buffer that will be shown on current page
	void ClearScreen(void); //Clears the screen
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
	void ResizeArrayBuffer(char **&buffer, const unsigned short size); //Wrapper for realloc (don't touch)
	void ResizeStringBuffer(char *&buffer, const unsigned int size); //Wrapper for realloc (don't touch)
	void InterpretLine(const char *string); //Interprets line
};
