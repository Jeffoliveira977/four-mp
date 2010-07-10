#include "CChat.h"
#include "./log/log.h"

HRESULT CreateD3DXFont( IDirect3DDevice9 * dDev, ID3DXFont** ppd3dxFont, char* pstrFont, DWORD dwSize, bool bold, bool Italic )
{
    HRESULT hr;
    HDC hDC;
    int nHeight;

    hDC = GetDC( NULL );
    nHeight = -MulDiv( dwSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 );
    ReleaseDC( NULL, hDC );

    hr = D3DXCreateFontA( dDev, nHeight, 0, bold*FW_BOLD, 0, Italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pstrFont, ppd3dxFont);

    return hr;
}

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

	CreateD3DXFont(pd3dDevice, &m_pFont, "Arial", 10, 0, 0);
	m_pFont->PreloadCharacters(0, 255);
	m_pFont->PreloadGlyphs(0, 255);
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
