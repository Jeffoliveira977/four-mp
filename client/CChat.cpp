#include "CChat.h"
#include "./log/log.h"

CChat::CChat(const int iMaxMessages, const int iMaxHistory, const int iMaxMyHistory, const int iFontSize, const char * pszFontName, const bool bFontBold, const bool bFontItalic) : CD3DManager()
{
	m_pMessages = new MESSAGE*[iMaxHistory];
	memset(m_pMessages, 0, 4 * iMaxHistory);

	m_pMyMsgHistory = new MESSAGE*[iMaxMyHistory];
	memset(m_pMyMsgHistory, 0, 4 * iMaxMyHistory);

	m_pMyMsg = new wchar_t[MAX_CHAT_MESSAGE_LENGTH];
	memset(m_pMyMsg, 0, MAX_CHAT_MESSAGE_LENGTH * sizeof(wchar_t));

	m_iMaxMessages = iMaxMessages;
	m_iMaxHistory = iMaxHistory;
	m_iMaxMyHistory = iMaxMyHistory;

	m_iScrollPos = 0;
	m_iCursorPos = 0;

	m_iFontSize = iFontSize;
	m_pszFontName = pszFontName;
	m_bFontBold = bFontBold;
	m_bFontItalic = bFontItalic;

	m_bUserScroll = true;

	InitializeCriticalSection(&critSect);
}

CChat::~CChat()
{
	DeleteCriticalSection(&critSect);
}

void CChat::OnCreateDevice(IDirect3DDevice9 * pd3dDevice, HWND hWnd)
{
	CD3DManager::OnCreateDevice(pd3dDevice, hWnd);

	RECT rct;
	GetWindowRect(hWnd, &rct);
	m_dwWidth = rct.right - rct.left;
	m_dwHeight = rct.bottom - rct.top;

	EnterCriticalSection(&critSect);

	D3DXCreateSprite(pd3dDevice, &m_pSprite);
	m_pFont = new CFont(pd3dDevice, m_pszFontName, m_iFontSize, m_bFontBold, m_bFontItalic);

	LeaveCriticalSection(&critSect);
}

void CChat::OnLostDevice()
{
	EnterCriticalSection(&critSect);

	if(m_pSprite) m_pSprite->OnLostDevice();
	if(m_pFont) m_pFont->OnLostDevice();

	LeaveCriticalSection(&critSect);
}

void CChat::OnResetDevice()
{
	EnterCriticalSection(&critSect);

	if(m_pSprite) m_pSprite->OnResetDevice();
	if(m_pFont) m_pFont->OnResetDevice();

	LeaveCriticalSection(&critSect);
}

void CChat::OnDraw()
{
	EnterCriticalSection(&critSect);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR2 vTransformation = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 vScaling(1.0f, 1.0f);

	D3DXMATRIX mainMatrix;
	D3DXMatrixTransformation2D(&mainMatrix, 0, 0, &vScaling, 0, 0, &vTransformation);
	m_pSprite->SetTransform(&mainMatrix);

	int iY = 1;
	for(int i = m_iScrollPos; i < m_iScrollPos + m_iMaxMessages && i < m_iMaxHistory; i++)
	{
		if(m_pMessages[i])
		{
			MESSAGE * pTmpMsg = m_pMessages[i];
			int iX = 1;
			while(pTmpMsg)
			{
				m_pFont->DrawText(pTmpMsg->msg, iX, iY, m_pSprite, pTmpMsg->color);
				iX += m_pFont->GetTextWidth(pTmpMsg->msg);
				pTmpMsg = pTmpMsg->next;
			}
		}
		iY += m_iFontSize + 2;
	}

	m_pSprite->End();

	LeaveCriticalSection(&critSect);
}

void CChat::OnBeginDraw()
{

}

void CChat::OnRelease()
{
	EnterCriticalSection(&critSect);

	if(m_pSprite) m_pSprite->Release();
	if(m_pFont) m_pFont->Release();

	LeaveCriticalSection(&critSect);
}

void CChat::AddMessage(wchar_t * pszMsg)
{
	if(!pszMsg) return;

	// Color: 0001 AARR GGBB

	EnterCriticalSection(&critSect);

	MESSAGE ** ppTmpMsg = NULL;
	for(int i = 0; i < m_iMaxHistory; i++)
	{
		if(!m_pMessages[i]) 
		{
			ppTmpMsg = &m_pMessages[i];
			break;
		}
	}

	if(!ppTmpMsg)
	{
		delete m_pMessages[0];
		m_pMessages[0] = NULL;

		for(int i = 1; i < m_iMaxHistory; i++)
		{
			m_pMessages[i - 1] = m_pMessages[i];
			m_pMessages[i] = NULL;
		}
		ppTmpMsg = &m_pMessages[m_iMaxHistory - 1];
	}

	if(!ppTmpMsg)
	{
		Log::Error("Can't add message to chat");
		LeaveCriticalSection(&critSect);
		return;
	}

	wchar_t * szTmpTxt = pszMsg;
	D3DCOLOR Color = 0xFFFFFFFF;

	int i = 0;
	while(pszMsg[i] != NULL)
	{
		if(pszMsg[i] == 1)
		{
			D3DCOLOR tColor = Color;
			Color = *(DWORD *)(pszMsg + i + 1);

			pszMsg[i] = 0;
			pszMsg[i + 1] = 0;
			pszMsg[i + 2] = 0;

			if(i != 0)
			{
				*ppTmpMsg = new MESSAGE(szTmpTxt, tColor);
				ppTmpMsg = &(*ppTmpMsg)->next;
			}

			i += 3;
			
			szTmpTxt = pszMsg + i;

			i++;
			continue;
		}
		i++;
	}

	if(szTmpTxt[0] != 0)
	{
		*ppTmpMsg = new MESSAGE(szTmpTxt, Color);
	}

	if(!m_bUserScroll) this->ScrollEnd();
	LeaveCriticalSection(&critSect);
}

void CChat::DeleteMessage(const int index)
{
	if(index < 0 || index > m_iMaxHistory) return;

	EnterCriticalSection(&critSect);
	if(m_pMessages[index]) 
	{
		delete m_pMessages[index];
		m_pMessages[index] = NULL;
	}
	
	LeaveCriticalSection(&critSect);
}

void CChat::Clear()
{
	EnterCriticalSection(&critSect);
	for(int i = 0; i < m_iMaxHistory; i++)
	{
		delete m_pMessages[i];
		m_pMessages[i] = NULL;
	}
	this->ScrollEnd();
	LeaveCriticalSection(&critSect);
}

bool CChat::ScrollDown()
{
	if(m_iScrollPos + m_iMaxMessages > m_iMaxHistory) return false;
	if(!m_pMessages[m_iScrollPos + m_iMaxMessages]) return false;

	m_bUserScroll = true;
	m_iScrollPos++;

	int i = 0;
	for(i = 0; i < m_iMaxHistory; i++)
	{
		if(!m_pMessages[i]) break;
	}

	if(i - m_iMaxMessages < 0) 
	{
		if(m_iScrollPos == 0) m_bUserScroll = false;
	}
	else
	{
		if(m_iScrollPos == i - m_iMaxMessages) m_bUserScroll = false;
	}

	return true;
}

bool CChat::ScrollUp()
{
	if(m_iScrollPos <= 0) return false;
	m_bUserScroll = true;
	m_iScrollPos--;
	return true;
}

void CChat::ScrollEnd()
{
	m_bUserScroll = false;

	int i = 0;
	for(i = 0; i < m_iMaxHistory; i++)
	{
		if(!m_pMessages[i]) break;
	}

	if(i - m_iMaxMessages < 0) 
	{
		m_iScrollPos = 0;
	}
	else
	{
		m_iScrollPos = i - m_iMaxMessages;
	}
}