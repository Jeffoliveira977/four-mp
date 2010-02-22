#include "CGUI.h"

CWindow::CWindow( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetMaximized( true );
	SetTitleVisible( true );
	SetFocussedElement( 0 );
	posDif = CPos();
	m_bDragging = false;
	SetMouseOver( false );
	SetElement( X, Y, Width, Height, String, String2, Callback, 1 );

	SetVisible( true );

	SetThemeElement( gpGui->GetThemeElement( "Window" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "Window", 0 );
	else
		SetElementState( "Norm" );

	SetThemeElement( gpGui->GetThemeElement( "CloseButton" ), 1 );

	if( !GetThemeElement( 1 ) )
		MessageBoxA( 0, "Theme element invalid.", "CloseButton", 0 );
	else
	{
		SetElementState( "Norm", 1 );
		SetCloseButton( true );

		MouseMove( gpGui->GetMouse() );
	}
}

CWindow::~CWindow()
{
	for( int iIndex = 0; iIndex < static_cast<int>( m_vElements.size() ); iIndex++ )
		SAFE_DELETE( m_vElements[ iIndex ] )
	m_vElements.clear();
}

void CWindow::AddElement( CElement * pElement )
{
	if(m_bTitleVisible)
		pElement->SetRelPos( *pElement->GetRelPos() + CPos( 0, TITLEBAR_HEIGHT ) );
	pElement->SetParent( this );

	m_vElements.push_back( pElement );
}

void CWindow::Draw()
{	
	if(m_bTitleVisible)
	{
		pTitlebar->Draw( *GetAbsPos(), GetWidth(), TITLEBAR_HEIGHT );
		gpGui->GetFont()->DrawString( GetAbsPos()->GetX() + 5, GetAbsPos()->GetY() + 5, 0, pTitle, GetFormatted() );
		pButton->Draw( CPos( GetAbsPos()->GetX() + GetWidth() - BUTTON_HEIGHT - 2, GetAbsPos()->GetY() + 2 ), BUTTON_HEIGHT, BUTTON_HEIGHT );
	}

	if( GetMaximized() )
	{
		if(m_bTitleVisible)
			gpGui->DrawOutlinedBox( GetAbsPos()->GetX(), GetAbsPos()->GetY() + TITLEBAR_HEIGHT, GetWidth(), GetHeight() - TITLEBAR_HEIGHT + 1,  pBodyInner->GetD3DCOLOR(), pBodyBorder->GetD3DCOLOR() );

		for( int iIndex = 0; iIndex < static_cast<int>( m_vElements.size() ); iIndex++ )
			m_vElements[iIndex]->Draw();
	}
}

void CWindow::PreDraw()
{
	GetString( true );

	if( GetMaximized() )
		for( int iIndex = 0; iIndex < static_cast<int>( m_vElements.size() ); iIndex++ )
		{
			m_vElements[ iIndex ]->PreDraw();
		}
}

void CWindow::MouseMove( CMouse * pMouse )
{
	if( GetDragging() )
	{
		if( !posDif.GetX() )
			posDif = *GetAbsPos() - pMouse->GetPos();
		else
		{
			CPos mPos = pMouse->GetPos();

			if( mPos.GetX() == -1 && mPos.GetY() == -1 )
				mPos = pMouse->GetSavedPos();

			SetAbsPos( mPos + posDif );
		}
	}

	if( GetCloseButton() && m_bTitleVisible )
		SetElementState( SetMouseOver( pMouse->InArea( GetAbsPos()->GetX() + GetWidth() - BUTTON_HEIGHT - 2, GetAbsPos()->GetY() + 2, BUTTON_HEIGHT, BUTTON_HEIGHT ) )?"MouseOver":"Norm", 1 );

	if( GetMaximized() )
		for( int iIndex = 0; iIndex < static_cast<int>( m_vElements.size() ); iIndex++ )
			m_vElements[ iIndex ]->MouseMove( pMouse );
}

void CWindow::KeyEvent( SKey sKey )
{
	CMouse * pMouse = gpGui->GetMouse();

	if( pMouse->GetLeftButton() )
	{
		SetFocussedElement( 0 );

		if( GetMouseOver() && m_bCloseButtonEnabled && m_bTitleVisible )
			this->SetVisible( false );
		else if( pMouse->InArea( GetAbsPos()->GetX(), GetAbsPos()->GetY(), GetWidth(), TITLEBAR_HEIGHT ) && m_bTitleVisible )
		{
			if( !pMouse->GetDragging() )
			{
				if( pMouse->GetLeftButton() == 1 )
				{
					gpGui->BringToTop( this );

					SetDragging( true );
					pMouse->SetDragging( this );

					SetElementState( "Dragging" );
				}
				else
				{
					SetMaximized( !GetMaximized() );

					SetElementState( GetMaximized()?"Norm":"Minimized" );

					gpGui->BringToTop( this );
				}
			}
		}
		else if(  pMouse->InArea( GetAbsPos()->GetX(), GetAbsPos()->GetY(), GetWidth(), GetHeight() ) )
			gpGui->BringToTop( this );
	}
	else
	{
		posDif.SetX( 0 );

		pMouse->SetDragging( 0 );
		SetDragging( false );

		SetElementState( GetMaximized()?"Norm":"Minimized" );
	}

	if( GetMaximized() )
		for( int iIndex = 0; iIndex < static_cast<int>( m_vElements.size() ); iIndex++ )
			m_vElements[ iIndex ]->KeyEvent( sKey );
}

void CWindow::SetMaximized( bool bMaximized )
{
	m_bMaximized = bMaximized;
}

bool CWindow::GetMaximized()
{
	return m_bMaximized;
}

void CWindow::SetVisible( bool bVisible )
{
	m_bVisible = bVisible;
}

bool CWindow::IsVisible()
{
	return m_bVisible;
}

void CWindow::SetDragging( bool bDragging )
{
	m_bDragging = bDragging;
}

bool CWindow::GetDragging()
{
	return m_bDragging;
}

void CWindow::SetCloseButton( bool bEnabled )
{
	m_bCloseButtonEnabled = bEnabled;

	if( GetCloseButton() )
		SetElementState( "Disabled", 1 );
}

bool CWindow::GetCloseButton()
{
	return m_bCloseButtonEnabled;
}

void CWindow::SetFocussedElement( CElement * pElement )
{
	m_pFocussedElement = pElement;

	if( pElement )
		BringToTop( pElement );
}

CElement * CWindow::GetFocussedElement()
{
	return m_pFocussedElement;
}

CElement * CWindow::GetNextElement( CElement * pElement )
{
	for( int i = 0; i < static_cast<int>( m_vElements.size() ); i++ )
		if( m_vElements[i] == pElement )
			return m_vElements[ i + 1 ];
	return *m_vElements.begin();
}

CElement * CWindow::GetElementByString( const char * pszString, int iIndex )
{
	for( int i = 0; i < static_cast<int>( m_vElements.size() ); i++ )
		if( m_vElements[ i ]->GetString( false, iIndex ) == pszString )
			return m_vElements[ i ];
	return 0;
}

void CWindow::BringToTop( CElement * pElement )
{
	for( int i = 0; i < static_cast<int>( m_vElements.size() ); i++ )
		if( m_vElements[i] == pElement )
			m_vElements.erase( m_vElements.begin() + i );
	m_vElements.insert(  m_vElements.end(), pElement );
}

void CWindow::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );
	if( !iIndex )
	{
		pTitle = pState->GetColor( "Title" );
		pBodyInner = pState->GetColor( "BodyInner" );
		pBodyBorder = pState->GetColor( "BodyBorder" );

		pTitlebar = pState->GetTexture( "Titlebar" );
	}
	else
		pButton = pState->GetTexture( "Button" );
}

void CWindow::SetTitleVisible( bool v )
{
	m_bTitleVisible = v;
}

bool CWindow::GetTitleVisible()
{
	return m_bTitleVisible;
}

void CWindow::LostFocus()
{
	if(GetAction())
		GetAction()("LF", this);
}