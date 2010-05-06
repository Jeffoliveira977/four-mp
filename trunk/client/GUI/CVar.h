#pragma once

class CVar;

#include "CGUI.h"

class CVar
{
	double m_dValue;
	std::string m_sString;

	typedef std::string ( __cdecl * tAction )( const char * pszArgs, CElement * pElement );
	tAction m_pAction;
public:
	CVar( double dValue );
	CVar( float fValue );
	CVar( int iValue );
	CVar( bool bValue );
	CVar( std::string sString );
	CVar( tAction pAction );

	void SetDouble( double dValue );
	void SetFloat( float fValue );
	void SetInteger( int iValue );
	void SetBoolean( bool bValue );
	void SetString( std::string sString );
	void SetAction( tAction pAction );

	double GetDouble();
	float GetFloat();
	int GetInteger();
	bool GetBoolean();
	std::string GetString();
	std::string GetActionString( const char * pszString, CElement * pElement = 0 );
	tAction GetAction();
};

class CCommand
{
public:
};