#include "CGUI.h"

CLine::CLine( CGUI *Gui, int sX, int sY, int eX, int eY, int size, CColor *color, CWindow *eP )
{
	pGui = Gui;
	this->sX = sX;
	this->sY = sY;
	this->eX = eX;
	this->eY = eY;
	this->Size = size;
	this->color = color;
	wParent = eP;
}

void CLine::Draw()
{
	CPos Pos = new CPos(0,0);
	if(wParent)
		Pos = wParent->GetAbsPos();

	pGui->DrawLine(Pos.GetX()+sX, Pos.GetY()+sY, Pos.GetX()+eX, Pos.GetY()+eY, Size, color->GetD3DCOLOR());
}

CBox::CBox( CGUI *Gui, int iX, int iY, int iWidth, int iHeight, CColor *In, CColor *Bo, CWindow *eP )
{
	pGui = Gui;
	X = iX; Y = iY; Width = iWidth; Height = iHeight;
	pInner = In;
	pBorder = Bo;
	wParent = eP;
}

void CBox::Draw()
{
	CPos Pos = new CPos(0,0);
	if(wParent)
		Pos = wParent->GetAbsPos();

	pGui->DrawOutlinedBox(Pos.GetX()+X, Pos.GetY()+Y, Width, Height, pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR());
}

CImage::CImage( CGUI *Gui, int iX, int iY, int iWidth, int iHeight, CTexture *cTxt, CWindow *eP )
{
	pGui = Gui;
	X = iX; Y= iY; Width = iWidth; Height = iHeight;
	pImg = cTxt;
	wParent = eP;
}

void CImage::Draw()
{
	CPos Pos = new CPos(0,0);
	if(wParent)
		Pos = wParent->GetAbsPos();

	pImg->Draw(X+Pos.GetX(), Y+Pos.GetY(), Width, Height);
}
