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

	void AddMessage(const wchar_t *);

private:
	CFont * m_pFont;

	wchar_t ** m_pMessages;
	wchar_t ** m_pMessagesHistory;
	wchar_t ** m_pMyMsgHistory;
	wchar_t * m_pMyMsg;

	int m_iMaxMessages;
	int m_iMaxHistory;
	int m_iMaxMyHistory;
};