#pragma once
#include "./hook/CD3DManager.h"
#include "CFont.h"
#include <d3dx9.h>

#define MAX_CHAT_MESSAGE_LENGTH 0x200

class CChat : public CD3DManager
{
public:
	CChat(const int maxMessages = 16, const int maxHistory = 64, const int maxMyHistory = 16);
	~CChat();

	void OnCreateDevice(IDirect3DDevice9 *);
	void OnLostDevice();
	void OnResetDevice();
	void OnDraw();
	void OnRelease();

	void AddMessage(wchar_t *);
	void DeleteMessage(const int index);
	void Clear();

private:
	CFont * m_pFont;
	ID3DXSprite * m_pSprite;

	struct MESSAGE
	{
		MESSAGE(wchar_t * msg, D3DCOLOR color) 
		{ 
			this->msg = new wchar_t[0x100];
			memset(this->msg, 0, 0x100 * sizeof(wchar_t));

			int len = wcslen(msg);
			for(int i = 0; i < len; i++)
			{
				if(msg[i] < 32) break;
				this->msg[i] = msg[i];
			}

			this->color = color; 
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
};