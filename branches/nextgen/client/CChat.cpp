#include "CChat.h"
#include "./log/log.h"

CChat::CChat(const int maxMessages, const int maxHistory, const int maxMyHistory) : CD3DManager()
{
	m_pMessages = new wchar_t*[maxMessages];
	for(int i = 0; i < maxMessages; i++)
	{
		m_pMessages[i] = new wchar_t[MAX_CHAT_MESSAGE_LENGTH];
		memset(m_pMessages[i], 0, MAX_CHAT_MESSAGE_LENGTH * sizeof(wchar_t));
	}

	m_pMessagesHistory = new wchar_t*[maxHistory];
	for(int i = 0; i < maxHistory; i++)
	{
		m_pMessagesHistory[i] = new wchar_t[MAX_CHAT_MESSAGE_LENGTH];
		memset(m_pMessagesHistory[i], 0, MAX_CHAT_MESSAGE_LENGTH * sizeof(wchar_t));
	}

	m_pMyMsgHistory = new wchar_t*[maxMyHistory];
	for(int i = 0; i < maxMyHistory; i++)
	{
		m_pMyMsgHistory[i] = new wchar_t[MAX_CHAT_MESSAGE_LENGTH];
		memset(m_pMyMsgHistory[i], 0, MAX_CHAT_MESSAGE_LENGTH * sizeof(wchar_t));
	}

	m_pMyMsg = new wchar_t[MAX_CHAT_MESSAGE_LENGTH];
	memset(m_pMyMsg, 0, MAX_CHAT_MESSAGE_LENGTH * sizeof(wchar_t));

	m_iMaxMessages = maxMessages;
	m_iMaxHistory = maxHistory;
	m_iMaxMyHistory = maxMyHistory;
}

CChat::~CChat()
{

}

void CChat::OnCreateDevice(IDirect3DDevice9 * pd3dDevice)
{
	CD3DManager::OnCreateDevice(pd3dDevice);

	m_pFont = new CFont(pd3dDevice, "Arial", 10, false, false);
}

void CChat::OnLostDevice()
{
	m_pFont->OnLostDevice();
}

void CChat::OnResetDevice()
{
	m_pFont->OnResetDevice();
}

void CChat::OnDraw()
{
	
}

void CChat::OnRelease()
{
	m_pFont->Release();
}
