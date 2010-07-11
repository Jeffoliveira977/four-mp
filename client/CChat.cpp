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
	D3DXCreateTexture(pd3dDevice, m_dwWidth, m_dwHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pChatTexture);
	m_pFont = new CFont(pd3dDevice, m_pszFontName, m_iFontSize, m_bFontBold, m_bFontItalic);

	LeaveCriticalSection(&critSect);
}

void CChat::OnLostDevice()
{
	EnterCriticalSection(&critSect);

	if(m_pChatTexture) 
	{
		m_pChatTexture->Release();
		m_pChatTexture = NULL;
	}
	if(m_pSprite) m_pSprite->OnLostDevice();
	if(m_pFont) m_pFont->OnLostDevice();

	LeaveCriticalSection(&critSect);
}

void CChat::OnResetDevice()
{
	EnterCriticalSection(&critSect);

	if(m_pSprite) m_pSprite->OnResetDevice();
	if(m_pFont) m_pFont->OnResetDevice();
	D3DXCreateTexture(m_pd3dDevice, m_dwWidth, m_dwHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pChatTexture);

	LeaveCriticalSection(&critSect);
}

void CChat::OnDraw()
{
	EnterCriticalSection(&critSect);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->Draw(m_pChatTexture, NULL, NULL, NULL, 0xFFFFFFFF);
	m_pSprite->End();

	LeaveCriticalSection(&critSect);
}

void CChat::OnRelease()
{
	EnterCriticalSection(&critSect);

	if(m_pSprite) m_pSprite->Release();
	if(m_pFont) m_pFont->Release();
	if(m_pChatTexture) m_pChatTexture->Release();

	LeaveCriticalSection(&critSect);
}

void CChat::AddMessage(wchar_t * pszMsg)
{
	if(!pszMsg) return;

	// Color: 0001 AARR GGBB

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
	this->Update();
}

void CChat::DeleteMessage(const int index)
{
	if(index < 0 || index > m_iMaxHistory) return;
	if(m_pMessages[index]) 
	{
		delete m_pMessages[index];
		m_pMessages[index] = NULL;
	}
	this->Update();
}

void CChat::Clear()
{
	for(int i = 0; i < m_iMaxHistory; i++)
	{
		delete m_pMessages[i];
		m_pMessages[i] = NULL;
	}
	this->Update();
}

void CChat::Update()
{
	EnterCriticalSection(&critSect);

	IDirect3DSurface9 *pTextureTarget, *pOldRenderTarget;
	m_pChatTexture->GetSurfaceLevel(0, &pTextureTarget);
	Log::Debug("Update");
	if(pTextureTarget)
	{
		Log::Debug("Render to texture");

		m_pd3dDevice->GetRenderTarget(0, &pOldRenderTarget);
		m_pd3dDevice->SetRenderTarget(0, pTextureTarget);

		m_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
		int iY = 1;
		for(int i = m_iScrollPos; i < m_iScrollPos + m_iMaxMessages && i < m_iMaxHistory; i++)
		{
			if(m_pMessages[i])
			{
				MESSAGE * pTmpMsg = m_pMessages[i];
				int iX = 1;
				while(pTmpMsg)
				{
					Log::Debug(L"Draw at (%d;%d) '%s' witd color #%X", iX, iY, pTmpMsg->msg, pTmpMsg->color);
					m_pFont->DrawText(pTmpMsg->msg, iX, iY, m_pSprite, pTmpMsg->color);
					iX += m_pFont->GetTextWidth(pTmpMsg->msg);
					pTmpMsg = pTmpMsg->next;
				}
			}
			iY += m_iFontSize + 2;
		}

		m_pSprite->End();

		pTextureTarget->Release();
		m_pd3dDevice->SetRenderTarget(0, pOldRenderTarget);
		pOldRenderTarget = NULL;
	}

	LeaveCriticalSection(&critSect);
}