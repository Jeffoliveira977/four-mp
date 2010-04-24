#include "CVar.h"

CVar::CVar( double dValue )
{
	SetDouble( dValue );
	SetAction( 0 );
	SetString( "" );
}

CVar::CVar( float fValue )
{
	SetFloat( fValue );
	SetAction( 0 );
	SetString( "" );
}

CVar::CVar( int iValue )
{
	SetInteger( iValue );
	SetAction( 0 );
	SetString( "" );
}

CVar::CVar( bool bValue )
{
	SetBoolean( bValue );
	SetAction( 0 );
	SetString( "" );
}

CVar::CVar( std::string sString )
{
	SetString( sString );
	SetAction( 0 );
	SetDouble( 0.0 );
}

CVar::CVar( tAction pAction )
{
	SetAction( pAction );
	SetDouble( 0.0 );
	SetString( "" );
}

void CVar::SetDouble( double dValue )
{
	m_dValue = dValue;
}

void CVar::SetFloat( float fValue )
{
	m_dValue = static_cast<double>( fValue );
}

void CVar::SetInteger( int iValue )
{
	m_dValue = static_cast<double>( iValue );
}

void CVar::SetBoolean( bool bValue )
{
	if( bValue )
		m_dValue = 1.0;
	else
		m_dValue = 0.0;
}

void CVar::SetString( std::string sString )
{
	m_sString = sString;
}

void CVar::SetAction( tAction pAction )
{
	m_pAction = pAction;
}

double CVar::GetDouble()
{
	return m_dValue;
}

float CVar::GetFloat()
{
	return static_cast<float>( m_dValue );
}

int CVar::GetInteger()
{
	return static_cast<int>( m_dValue );
}

bool CVar::GetBoolean()
{
	return ( m_dValue != 0.0 );
}

std::string CVar::GetString()
{
	return m_sString;
}

std::string CVar::GetActionString( const char * pszString, CElement * pElement )
{
	/*std::stringstream sStream;

	if( m_pAction )
		sStream << m_pAction( pszString, pElement );
	else if( GetString().length() )
		sStream << GetString();
	else if( GetInteger() )
		sStream << GetInteger();
	else
		sStream << "Cvar invalid.";

	return sStream.str();*/
	if( GetAction() )
		return GetAction()( pszString, pElement );
	return std::string();
}

CVar::tAction CVar::GetAction()
{
	return m_pAction;
}