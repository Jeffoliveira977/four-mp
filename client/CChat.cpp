#include "CChat.h"
#include "./log/log.h"

CChat::CChat(const int maxMessages, const int maxHistory, const int maxMyHistory) : CD3DManager()
{
	m_pMessages = new MESSAGE*[maxHistory];
	memset(m_pMessages, 0, 4 * maxHistory);

	m_pMyMsgHistory = new MESSAGE*[maxMyHistory];
	memset(m_pMyMsgHistory, 0, 4 * maxMyHistory);

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

	D3DXCreateSprite(pd3dDevice, &m_pSprite);
	m_pFont = new CFont(pd3dDevice, "Arial", 10, false, false);
}

void CChat::OnLostDevice()
{
	m_pSprite->OnLostDevice();
	m_pFont->OnLostDevice();
}

void CChat::OnResetDevice()
{
	m_pSprite->OnResetDevice();
	m_pFont->OnResetDevice();
}

void CChat::OnDraw()
{
	
}

void CChat::OnRelease()
{
	m_pSprite->Release();
	m_pFont->Release();
}

void CChat::AddMessage(wchar_t * pszMsg)
{
	if(!pszMsg) return;

	// Color: 0001 AARR GGBB

	MESSAGE ** tmp_msg = NULL;
	for(int i = 0; i < m_iMaxHistory; i++)
	{
		if(!m_pMessages[i]) 
		{
			tmp_msg = &m_pMessages[i];
			break;
		}
	}

	if(!tmp_msg)
	{
		delete m_pMessages[0];
		m_pMessages[0] = NULL;

		for(int i = 1; i < m_iMaxHistory; i++)
		{
			m_pMessages[i - 1] = m_pMessages[i];
			m_pMessages[i] = NULL;
		}
		tmp_msg = &m_pMessages[m_iMaxHistory - 1];
	}

	if(!tmp_msg)
	{
		Log::Error("Can't add message to chat");
		return;
	}

	int len = wcslen(pszMsg);
	wchar_t * tmp_txt = pszMsg;
	D3DCOLOR clr = 0xFFFFFFFF;
	int i = 0;
	while(pszMsg[i] != NULL)
	{
		if(pszMsg[i] == 1)
		{
			D3DCOLOR tclr = clr;
			clr = *(DWORD *)(pszMsg + i + 1);

			pszMsg[i] = 0;
			pszMsg[i + 1] = 0;
			pszMsg[i + 2] = 0;

			if(i != 0)
			{
				*tmp_msg = new MESSAGE(tmp_txt, tclr);
				tmp_msg = &(*tmp_msg)->next;
			}

			i += 3;
			
			tmp_txt = pszMsg + i;

			i++;
			continue;
		}
		i++;
	}

	if(tmp_txt[0] != 0)
	{
		*tmp_msg = new MESSAGE(tmp_txt, clr);
	}
}

void CChat::DeleteMessage(const int index)
{
	
}

void CChat::Clear()
{
	for(int i = 0; i < m_iMaxHistory; i++)
	{
		delete m_pMessages[i];
	}
	//m_pSprite->
}