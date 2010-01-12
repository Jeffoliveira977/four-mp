#include <stdio.h>

class CFG
{
public:
	void LoadFile(const char *filename);
	char *GetVara(const char *varname);
	int GetVari(const char *varname);
	float GetVarf(const char *varname);

	CFG();
	CFG(const char *filename);
	~CFG();

protected:
	FILE *fd;
	int var_count;
	char buff[512];
	char var[32][32];
	char value[32][128];

	void Parse();
};