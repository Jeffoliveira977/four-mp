#include "ConCmd.h"
#include "Console.h"

extern Console con;

ConCmd::ConCmd(const char *cmdname, void *callback, const char *desc, const int cmdflags)
{
	this->Init(cmdname, desc, cmdflags);
	handler = callback;
	ConsoleSymbol *tempsymbol = (ConsoleSymbol *)calloc(1, sizeof(ConsoleSymbol));
	tempsymbol->name = (char *)calloc(strlen(cmdname) + 1, sizeof(char));
	strcpy(tempsymbol->name, cmdname);
	tempsymbol->type = ConsoleSymbolTypeConCmd;
	tempsymbol->ptr.concmd = this;
	con.AddConsoleSymbol(tempsymbol);
}

ConCmd::~ConCmd(void)
{
	this->Uninit();
}

void ConCmd::Execute(void)
{
	unsigned char numargs = con.GetCmdArgs();
	((void (*) (const unsigned char))handler)(numargs);
}
