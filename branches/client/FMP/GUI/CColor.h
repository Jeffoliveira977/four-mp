#pragma once

#include <map>
#include "TinyXML\TinyXML.h"

class CColor
{
	int m_iRed, m_iGreen, m_iBlue, m_iAlpha;
public:
	CColor();
	CColor( int iRed, int iGreen, int iBlue, int iAlpha );
	CColor( D3DCOLOR d3dColor );
	CColor( TiXmlElement * pElement );
	~CColor();

	void SetD3DCOLOR( D3DCOLOR d3dColor );
	void SetRed( int iRed );
	void SetGreen( int iGreen );
	void SetBlue( int iBlue );
	void SetAlpha( int iAlpha );

	D3DCOLOR GetD3DCOLOR();
	int GetRed();
	int GetGreen();
	int GetBlue();
	int GetAlpha();

	CColor operator / ( int iDivisor );
	CColor operator * ( int iMultiplicator );

	CColor operator - ( CColor cSubColor );
	CColor operator + ( CColor cAddColor );
};

struct SElement;

struct SElementState
{
	SElement * pParent;

	CColor * GetColor( std::string sString );
	CTexture * GetTexture( std::string sString );

	std::map<std::string, CColor*> mColors;
	std::map<std::string, CTexture*> mTextures;
};

struct SElement
{
	std::string sDefaultState;
	std::map<std::string, SElementState*> m_mStates;
};