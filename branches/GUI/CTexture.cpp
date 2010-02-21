#include "CGUI.h"

CTexture::~CTexture()
{
	SAFE_RELEASE( m_pTexture );
}

CTexture::CTexture( ID3DXSprite * pSprite, const char * szPath )
{
	pSprite->GetDevice( &m_pDevice );

	D3DXCreateTextureFromFileA( m_pDevice, szPath, &m_pTexture );

	if( !m_pTexture )
	{
		std::stringstream sStream;
		sStream << "Failed to load texture: " << szPath;
		MessageBoxA( 0, sStream.str().c_str(), "CTexture::CTexture( ... )", 0 );
		return;
	}

	m_pTexture->GetLevelDesc( 0, &m_TexDesc );
	m_pSprite = pSprite;
}

void CTexture::Draw( CPos cpPos, int iWidth, int iHeight )
{
	D3DXVECTOR2 vTransformation = D3DXVECTOR2( static_cast<float>( cpPos.GetX() ), static_cast<float>( cpPos.GetY() ) );

	D3DXVECTOR2 vScaling( ( 1.0f / m_TexDesc.Width ) * iWidth, ( 1.0f / m_TexDesc.Height ) * iHeight );

	D3DXMATRIX mainMatrix;

	D3DXMatrixTransformation2D( &mainMatrix, 0, 0, &vScaling, 0, 0, &vTransformation );

	m_pSprite->SetTransform( &mainMatrix );

	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_pSprite->Draw( m_pTexture, 0, 0, 0, 0xFFFFFFFF );
	m_pSprite->End();
}