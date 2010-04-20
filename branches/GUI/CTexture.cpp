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

	m_pColor = new CColor(255,255,255,255);
	m_pTexture->GetLevelDesc( 0, &m_TexDesc );
	m_pSprite = pSprite;
}

CTexture::CTexture( ID3DXSprite * pSprite, const char * szPath, CColor *color )
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

	m_pColor = color;
	m_pTexture->GetLevelDesc( 0, &m_TexDesc );
	m_pSprite = pSprite;
}

void CTexture::SetColor(CColor *color)
{
	m_pColor = color;
}

CColor *CTexture::GetColor()
{
	return m_pColor;
}

void CTexture::Draw( CPos cpPos, int iWidth, int iHeight )
{
	D3DXVECTOR2 vTransformation = D3DXVECTOR2( static_cast<float>( cpPos.GetX() ), static_cast<float>( cpPos.GetY() ) );

	D3DXVECTOR2 vScaling( ( 1.0f ), ( 1.0f ) );
	if(iWidth >= 0 && m_TexDesc.Width > 0) vScaling.x = vScaling.x*iWidth/m_TexDesc.Width;
	if(iHeight >= 0 && m_TexDesc.Height > 0) vScaling.y = vScaling.y*iHeight/m_TexDesc.Height;

	D3DXMATRIX mainMatrix;

	D3DXMatrixTransformation2D( &mainMatrix, 0, 0, &vScaling, 0, 0, &vTransformation );

	m_pSprite->SetTransform( &mainMatrix );

	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_pSprite->Draw( m_pTexture, 0, 0, 0, m_pColor->GetD3DXCOLOR() );
	m_pSprite->End();
}

void CTexture::Draw( int iX, int iY, int iWidth, int iHeight )
{
	D3DXVECTOR2 vTransformation = D3DXVECTOR2( static_cast<float>( iX ), static_cast<float>( iY ) );

	D3DXVECTOR2 vScaling( ( 1.0f ), ( 1.0f ) );
	if(iWidth >= 0 && m_TexDesc.Width > 0) vScaling.x = vScaling.x*iWidth/m_TexDesc.Width;
	if(iHeight >= 0 && m_TexDesc.Height > 0) vScaling.y = vScaling.y*iHeight/m_TexDesc.Height;

	D3DXMATRIX mainMatrix;

	D3DXMatrixTransformation2D( &mainMatrix, 0, 0, &vScaling, 0, 0, &vTransformation );

	m_pSprite->SetTransform( &mainMatrix );

	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	m_pSprite->Draw( m_pTexture, 0, 0, 0, m_pColor->GetD3DXCOLOR() );
	m_pSprite->End();
}