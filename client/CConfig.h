#include <map>

class CConfig
{
public:
	CConfig(char *);
	CConfig();
	~CConfig();

	bool LoadConfig(char *);
	bool SaveConfig();
	void Dump();

	char GetCharValue(const wchar_t *, const char cDefValue = 0);
	int GetIntValue(const wchar_t *, const int iDefValue = 0);
	float GetFloatValue(const wchar_t *, const float fDefValue = 0);
	wchar_t * GetStringValue(const wchar_t *, const wchar_t * pszDefVale = 0);

	void SetCharValue(const wchar_t *, const char);
	void SetIntValue(const wchar_t *, const int);
	void SetFloatValue(const wchar_t *, const float);
	void SetStringValue(const wchar_t *, const wchar_t *);

private:
	char m_pszConfigName[256];
	std::map<const wchar_t *, const wchar_t *> m_mKeyValues;
	bool m_bChanged;
};