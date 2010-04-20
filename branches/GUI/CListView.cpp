#include "CListView.h"

CListView::CListView( CGUI *Gui, int X, int Y, int *Width, int Height, int Columns, const char *String, const char *String2, tAction Callback )
{
	Count = Columns;
	Poss = new int[Count];
	Widths = new int[Count];

	int AllWidth = 0;
	Poss[0] = 0;
	for(int i = 0; i < Count; i++)
	{
		Widths[i] = Width[i];
		AllWidth += Width[i];
		if(i > 0)
			Poss[i] = Poss[i-1]+Widths[i-1];
	}

	SetElement( Gui, X, Y, AllWidth, Height, String, String2, Callback );
	m_iMouseOverIndex = -1;
	m_iMouseSelect = -1;
	m_vRows = new std::vector<std::string>[Count];

	pSlider = new CScrollBar( Gui, CPos( GetWidth() - 1, 1 ), GetHeight()-2 );

	SetThemeElement( gpGui->GetThemeElement( "ListView" ) );

	ShowSlider(1);
	Widths[Count-1] -= pSlider->GetWidth();

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "ListView", 0 );
	else
		SetElementState( "Norm" );
}

void CListView::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	int iAddHeight = GetFont()->GetStringHeight();

	gpGui->DrawOutlinedBox( Pos.GetX() + 1, Pos.GetY() + 1, GetWidth()-2, iAddHeight, pTitle->GetD3DCOLOR(), pTitle->GetD3DCOLOR() );
	for( int j = 0; j < Count; j++)
	{
		if( static_cast<int>( mTitles.size() ) > j)
		{
			GetFont()->DrawString( Pos.GetX() + Widths[j]/2 + (Poss[j]?Poss[j]:0), Pos.GetY(), FT_CENTER, pTitleString, mTitles[j], Widths[j] );
			if(j != Count-1) gpGui->DrawLine(Pos.GetX()+Widths[j]+(Poss[j]?Poss[j]:0), Pos.GetY()+2, Pos.GetX()+Widths[j]+Poss[j] , Pos.GetY()+iAddHeight-1, 1, pbTitle->GetD3DCOLOR());
		}

		int iHeight = iAddHeight + 5;

		if(j != Count-1) gpGui->DrawLine(Pos.GetX()+Widths[j]+(Poss[j]?Poss[j]:0), Pos.GetY()+iHeight, Pos.GetX()+Widths[j]+Poss[j] , Pos.GetY()+GetHeight()-2, 1, pbInner->GetD3DCOLOR());

		if( m_vRows[j].size() )
		{
			for( int i = pSlider->GetValue(); i < static_cast<int>( m_vRows[j].size() ) && iHeight < GetHeight() - GetFont()->GetStringHeight(); i++ )
			{
				CColor * pColor = 0;

				if( i == m_iMouseOverIndex && GetMouseOver())
					pColor = pMouseOverString;
				else
					pColor = pString;

				if(m_iMouseSelect == i)	
				{
					if(j == 0)
						gpGui->DrawOutlinedBox( Pos.GetX() + 1, Pos.GetY() + iHeight, GetWidth()-2, iAddHeight, pString->GetD3DCOLOR(), pString->GetD3DCOLOR() );
					pColor = pInner;
				}

				GetFont()->DrawString( Pos.GetX() + (Poss[j]?Poss[j]:0) + Widths[j]/2, Pos.GetY() + iHeight, FT_CENTER, pColor, m_vRows[j][i], Widths[j] );

				if(j == 0 && iHeight != iAddHeight + 5)
					gpGui->DrawLine(Pos.GetX()+2, Pos.GetY()+iHeight, Pos.GetX()+GetWidth()-4 , Pos.GetY()+iHeight, 1, pbInner->GetD3DCOLOR());

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

bool CListView::MouseMove( CMouse * pMouse, bool over )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();
	
	bool inArea = pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) && over;
	SetMouseOver( inArea );
	
	m_iMouseOverIndex = -1;
	int iStringHeight = GetFont()->GetStringHeight();
	for( int i = pSlider->GetValue(), iHeight = iStringHeight+5; i < static_cast<int>( m_vRows[0].size() ) || iHeight < GetHeight(); i++ )
	{
		if( pMouse->InArea( Pos.GetX(), Pos.GetY() + iHeight, GetWidth() - 10, iStringHeight ) )
			m_iMouseOverIndex = i;

		iHeight += iStringHeight;
	}

	pSlider->MouseMove( Pos, pMouse );
	return inArea;
}

bool CListView::KeyEvent( SKey sKey )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	if( !sKey.m_vKey )
	{
		if( GetMouseOver() )
		{
			if( m_iMouseOverIndex >= 0 && gpGui->GetMouse()->GetLeftButton() )
			{
				if(m_iMouseSelect > -2) m_iMouseSelect = m_iMouseOverIndex;
				if( GetAction() )
					GetAction()( this, SELECT, m_iMouseOverIndex );
			}
		}
	}

	if( GetMouseOver() || ( !sKey.m_bDown && !gpGui->GetMouse()->GetWheel() )  )
	{
		pSlider->KeyEvent( Pos, sKey );
		MouseMove( gpGui->GetMouse(), 1 );
	}

	return 0;
}

int CListView::GetSize(int Index)
{
	if(Index >= Count || Index < 0) return -1;
	return m_vRows[Index].size();
}

void CListView::PutStr( std::string sString, int Column, int Row )
{
	if(Column < 0 || Column >= Count) return;
	if(Row >= static_cast<int>( m_vRows[Column].size() ) || Row == -1)
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

	pTitle = pState->GetColor( "Title" );
	pbTitle = pState->GetColor( "TitleDel" );
	pbInner = pState->GetColor( "InnerDel" );
	pTitleString = pState->GetColor( "TitleString" );
}

void CListView::ShowSlider( bool bShow )
{
	pSlider->SetShow(bShow);
}

int CListView::GetSelected()
{
	return m_iMouseSelect;
}

void CListView::SetSelect(int Item)
{
	m_iMouseSelect = Item;
}