#include "CListView.h"

CListView::CListView( int X, int Y, int *Width, int Height, int Columns, const char *String, const char *String2, const char * Callback )
{
	Count = Columns;
	Poss = new int[Count];
	Widths = new int[Count];

	int AllWidth = 0;
	for(int i = 0; i < Count; i++)
	{
		Widths[i] = Width[i];
		AllWidth += Width[i];
		if(i == 0)
			Poss[i] = 0;
		else
			Poss[i] = Poss[i-1]+Widths[i-1];
	}

	SetElement( X, Y, AllWidth, Height, String, String2, Callback );
	m_iMouseOverIndex = -1;
	m_iMouseSelect = -1;
	m_vRows = new std::vector<std::string>[Count];

	pSlider = new CHelperSlider( CPos( GetWidth() - HELPERSLIDER_WIDTH + 2, 0 ), GetHeight() );

	SetThemeElement( gpGui->GetThemeElement( "ListBox" ) );

	ShowSlider(1);

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "ListView", 0 );
	else
		SetElementState( "Norm" );
}

void CListView::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	int iAddHeight = gpGui->GetFont()->GetStringHeight();
				
	for( int j = 0; j < Count; j++)
	{
		if(mTitles.size() > j)
			gpGui->GetFont()->DrawString( Pos.GetX() + 3 + Poss[j], Pos.GetY(), 0, pString, mTitles[j], Widths[j] );
		
		int iHeight = iAddHeight + 5;

		if( m_vRows[j].size() )
		{
			for( int i = pSlider->GetValue(); i < static_cast<int>( m_vRows[j].size() ) && iHeight < GetHeight() - gpGui->GetFont()->GetStringHeight(); i++ )
			{
				CColor * pColor = 0;

				if( i == m_iMouseOverIndex && GetMouseOver())
					pColor = pMouseOverString;
				else
					pColor = pString;

				if(m_iMouseSelect == i)	
				{
					if(j == 0)
						gpGui->DrawOutlinedBox( Pos.GetX() + 2, Pos.GetY() + iHeight, GetWidth() - HELPERSLIDER_WIDTH, iAddHeight, pString->GetD3DCOLOR(), pString->GetD3DCOLOR() );
					pColor = pInner;
				}

				gpGui->GetFont()->DrawString( Pos.GetX() + 3 + Poss[j], Pos.GetY() + iHeight, 0, pColor, m_vRows[j][i], Widths[j] );

				iHeight += iAddHeight;
			}
		}
	}
	pSlider->Draw( Pos );
}	

void CListView::PreDraw()
{
	pSlider->PreDraw();
}

void CListView::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();
	
	SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) );
	
	m_iMouseOverIndex = -1;
	int iStringHeight = gpGui->GetFont()->GetStringHeight();
	for( int i = pSlider->GetValue(), iHeight = iStringHeight+5; i < static_cast<int>( m_vRows[0].size() ) || iHeight < GetHeight(); i++ )
	{
		if( pMouse->InArea( Pos.GetX(), Pos.GetY() + iHeight, GetWidth() - BUTTON_HEIGHT, iStringHeight ) )
			m_iMouseOverIndex = i;

		iHeight += iStringHeight;
	}
	pSlider->MouseMove( Pos, pMouse );
}

void CListView::KeyEvent( SKey sKey )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	if( !sKey.m_vKey )
	{
		if( GetMouseOver() )
		{
			if( m_iMouseOverIndex >= 0 && gpGui->GetMouse()->GetLeftButton() )
			{
				m_iMouseSelect = m_iMouseOverIndex;
				if( GetAction() )
					GetAction()( reinterpret_cast<char*>( m_iMouseOverIndex ), this );
			}
		}
	}

	if( GetMouseOver() || ( !sKey.m_bDown && !gpGui->GetMouse()->GetWheel() )  )
	{
		pSlider->KeyEvent( Pos, sKey );
		MouseMove( gpGui->GetMouse() );
	}
}

int CListView::GetSize(int Index)
{
	if(Index >= Count || Index < 0) return -1;
	return m_vRows[Index].size();
}

void CListView::PutStr( std::string sString, int Column, int Row )
{
	if(Column < 0 || Column >= Count) return;
	if(Row >= m_vRows[Column].size() || Row == -1)
	{
		pSlider->SetMaxValue( m_vRows[Column].size() );
		m_vRows[Column].push_back(sString);
	}
	else
	{
		m_vRows[Column][Row] = sString;
	}
}

std::string CListView::GetStr( int Row, int Column )
{
	if(Column < 0 || Column >= Count) return NULL;

	if( Row >= 0 && Row < static_cast<int>( m_vRows[Column].size() ) )
		return m_vRows[ Column ][ Row ];
	return std::string();
}

void CListView::SetTitle( std::string sString, int Column )
{
	if(Column == -1)
	{
		mTitles.push_back(sString);
	}
	else
	{
		mTitles[Column] = sString;
	}
}

void CListView::Clear()
{
	for(int i = 0; i < Count; i++)
		m_vRows[i].clear();
}

void CListView::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
	pString = pState->GetColor( "String" );
	pMouseOverString = pState->GetColor( "MouseOverString" );
}

void CListView::ShowSlider( bool bShow )
{
	pSlider->Show = bShow;
}

int CListView::GetSelected()
{
	return m_iMouseSelect;
}
