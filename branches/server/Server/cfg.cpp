#include "cfg.h"
#include <stdlib.h>
#include <string.h>

CFG::CFG()
{
	var_count = 0;
}

CFG::CFG(const char *filename)
{
	var_count = 0;
	this->LoadFile(filename);
}

CFG::~CFG()
{
	delete [] buff;
	delete [] var;
	delete [] value;
}

void CFG::LoadFile(const char *filename)
{
	fd = fopen(filename, "r");
	if(!fd)
		return;

	while(!feof(fd))
	{
		fgets(buff, 512, fd);
		Parse();
	}
	fclose(fd);
}

void CFG::Parse()
{
	if(var_count == 32) return;
	if(strlen(buff) <= 2) return;
	if((buff[0] == buff[1] && (buff[0] == '/' || buff[0] == '-')) || buff[0] == '#') return;

	bool val = 0;
	int index = 0;
	for(int i = 0; i != strlen(buff); i++)
	{
		if(buff[i] == '\t' || buff[i] == '\n' || buff[i] == '\r') continue; 
		if(i < 511) 
		{
			if(buff[i] == buff[i+1] && (buff[i] == '/' || buff[i] == '-')) break;
			if((index == 0 || buff[i+1] == '=') && buff[i] == ' ') continue;
		}
		if(buff[i] == '=') { val = 1; var[var_count][index] = 0; index = 0; continue; }
		if(val)
			value[var_count][index] = buff[i];
		else
			var[var_count][index] = buff[i];
		index++;
	}
	value[var_count][index] = 0;
	var_count++;
}

char *CFG::GetVara(const char *varname)
{
	for(int i = 0; i < var_count; i++)
	{
		if(strcmp(varname, var[i]) == 0)
			return value[i];
	}
	return "\0";
}

int CFG::GetVari(const char *varname)
{
	for(int i = 0; i < var_count; i++)
	{
		if(strcmp(varname, var[i]) == 0)
			return atoi(value[i]);
	}
	return 0;
}

float CFG::GetVarf(const char *varname)
{
	for(int i = 0; i < var_count; i++)
	{
		if(strcmp(varname, var[i]) == 0)
			return atof(value[i]);
	}
	return 0;
}