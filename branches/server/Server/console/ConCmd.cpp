#include <stdlib.h>
#include <string.h>

#include "ConCmd.h"
#include "ConsoleCore.h"

extern ConsoleCore concore;

ConCmd::ConCmd(const char *cmdname, void *callback, const char *desc, const int cmdflags)
{
	this->Init(cmdname, desc, cmdflags);
	handler = callback;
	if (!concore.AddConCmd(cmdname, this))
	{
		delete this;
	}
}

ConCmd::~ConCmd(void)
{
	concore.DeleteConCmd(this->name, this);
	this->Uninit();
}

void ConCmd::Execute(void)
{
	unsigned char numargs = concore.GetCmdArgs();
	((void (*) (const unsigned char))handler)(numargs);
}
