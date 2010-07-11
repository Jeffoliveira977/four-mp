#pragma once
#include "./hook/CD3DManager.h"
#include "CFont.h"
#include <d3dx9.h>

#define MAX_CHAT_MESSAGE_LENGTH 0x200

class CChat : public CD3DManager
{
public:
	CChat(const int iMaxMessages = 16, const int iMaxHistory = 64, const int iMaxMyHistory = 16, const int iFontSize = 10, const char * pszFontName = "Arial", const bool bFontBold = false, const bool bFontItalic = false);
	~CChat();

	void OnCreateDevice(IDirect3DDevice9 *, HWND);
	void OnLostDevice();
	void OnResetDevice();
	void OnDraw();
	void OnRelease();

	void AddMessage(wchar_t *);
	void DeleteMessage(const int index);
	void Clear();

	void Update();

private:
	CFont * m_pFont;
	ID3DXSprite * m_pSprite;
	IDirect3DTexture9 * m_pChatTexture;

	CRITICAL_SECTION critSect;

	DWORD m_dwWidth;
	DWORD m_dwHeight;

	struct MESSAGE
	{
		MESSAGE(wchar_t * pszMsg, D3DCOLOR Color) 
		{ 
			this->msg = new wchar_t[0x100];
			memset(this->msg, 0, 0x100 * sizeof(wchar_t));

			int len = wcslen(pszMsg);
			for(int i = 0; i < len; i++)
			{
				if(pszMsg[i] < 32) break;
				this->msg[i] = pszMsg[i];
			}

			this->color = Color; 
			this->next = NULL;
		}
		~MESSAGE() 
		{ 
			if(msg) delete msg; 
			if(next) delete next; 
		}

		wchar_t *msg;
		D3DCOLOR color;
		MESSAGE * next;
	};

	MESSAGE ** m_pMessages;
	MESSAGE ** m_pMyMsgHistory;
	wchar_t * m_pMyMsg;

	int m_iMaxMessages;
	int m_iMaxHistory;
	int m_iMaxMyHistory;

	int m_iScrollPos;
	int m_iCursorPos;

	int m_iFontSize;
	const char *m_pszFontName;
	bool m_bFontBold;
	bool m_bFontItalic;
};