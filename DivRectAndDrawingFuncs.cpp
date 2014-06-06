#include "stdafx.h"
#include "MainWnd.h"


CRect _setRect(const CRect &whole, double rl, double rt, double rr, double rb)
{
	CRect r;
	r.left = whole.left + whole.Width() * rl;
	r.top = whole.top + whole.Height() * rt;
	r.right = whole.left + whole.Width() * rr;
	r.bottom = whole.top + whole.Height() * rb;
	return r;
}

CRect _getAlignRect(const CRect& whole, int x, int y, int align)
{
	CRect r;
	if((align & _ALIGNLEFT) == (align & _ALIGNRIGHT)){
		r.left = whole.left + (whole.Width() - x) / 2;
	}
	else if(align & _ALIGNLEFT){
		r.left = whole.left;
	}
	else{
		r.left = whole.right - x;
	}

	if((align & _ALIGNTOP) == (align & _ALIGNBOTTOM)){
		r.top = whole.top + (whole.Height() - y) / 2;
	}
	else if(align & _ALIGNTOP){
		r.top = whole.top;
	}
	else{
		r.top = whole.bottom - y;
	}
	r.right = r.left + x;
	r.bottom = r.top + y;
	return r;
}

CRect _getCenterRect(const CRect& whole, int x, int y)
{
	return _getAlignRect(whole, x, y, 0);
}

int MainWnd::DivWhole(const CRect &whole, int num, RECTARR &rectlst)
{
	if(num == 3 || num == 4){
		rectlst.clear();
		////ry > rx
		double rx = 0.33, ry = 0.45;
		int bwidth = whole.Width()*rx, bheight = whole.Height()*ry;
		CRect base;
		base.left = 0; base.top = 0; base.right = bwidth; base.bottom = bheight;
		CPoint mvto;
		mvto.x = whole.left; mvto.y = whole.top + ((whole.Height() - bheight)/2);
		base.MoveToXY(mvto);
		rectlst.push_back(base);
		mvto.x = whole.left + ((whole.Width() - bwidth)/2); mvto.y = whole.top;
		base.MoveToXY(mvto);
		rectlst.push_back(base);
		mvto.x = whole.right - bwidth; mvto.y = whole.top + ((whole.Height() - bheight)/2);
		base.MoveToXY(mvto);
		rectlst.push_back(base);
		mvto.x = whole.left + ((whole.Width() - bwidth)/2); mvto.y = whole.bottom - bheight;
		base.MoveToXY(mvto);
		rectlst.push_back(base);
		////5th rect: go previous
		rectlst.push_back(_setRect(whole, 0.79, 0.01, 0.99, 0.1));
		////6th rect: go next
		rectlst.push_back(_setRect(whole, 0.79, 0.11, 0.99, 0.2));
		////7th rect: newgame
		rectlst.push_back(_setRect(whole, 0.01, 0.01, 0.21, 0.1));
		////8th rect: cha
		rectlst.push_back(_setRect(whole, 0.01, 0.11, 0.21, 0.2));
	}
	return 0;
}

int MainWnd::DivPlayerRect(const CRect &playerrect, RECTARR &rectlst)
{
	rectlst.clear();
	CRect ming, an, hu;

	rectlst.push_back(_setRect(playerrect,        0,      0.1,        1,     0.45)); //name
	rectlst.push_back(_setRect(playerrect,        0,     0.35,        1,     0.65)); //fan
	rectlst.push_back(_setRect(playerrect,      0.3,     0.02,      0.7,     0.15)); //men
	rectlst.push_back(hu = _setRect(playerrect,      0.1,     0.6,      0.9,     0.95)); //hu
	rectlst.push_back(_setRect(hu,                0,        0,     0.49,        1)); //hu left
	rectlst.push_back(_setRect(hu,             0.51,        0,        1,        1)); //hu right
	rectlst.push_back(ming = _setRect(playerrect,     0.02,     0.05,     0.25,      0.6)); //ming
	rectlst.push_back(_setRect(ming,              0,        0,        1,      0.5)); //ming word
	rectlst.push_back(_setRect(ming,              0,      0.5,        1,        1)); //ming cnt
	rectlst.push_back(an = _setRect(playerrect,     0.75,     0.05,     0.98,      0.6)); //an
	rectlst.push_back(_setRect(an,                0,        0,        1,      0.5)); //an word
	rectlst.push_back(_setRect(an,                0,      0.5,        1,        1)); //an cnt

	return 0;
}


void MainWnd::DrawFrame(CDC *pdc, const CRect &r, unsigned int color)
{
	pdc->FillSolidRect(r.left, r.top+1, 1, r.Height()-2, color);
	pdc->FillSolidRect(r.left, r.top, r.Width(), 1, color);
	pdc->FillSolidRect(r.right-1, r.top+1, 1, r.Height()-2, color);
	pdc->FillSolidRect(r.left, r.bottom-1, r.Width(), 1, color);
}

void MainWnd::DrawPenFrame(CDC *pdc, const CRect &r, unsigned int color)
{

	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, 1, color);
	oldpen = pdc->SelectObject(&pen);
	pdc->MoveTo(r.TopLeft());
	pdc->LineTo(r.right, r.top);
	pdc->LineTo(r.right, r.bottom);
	pdc->LineTo(r.left, r.bottom);
	pdc->LineTo(r.left, r.top);

	pdc->SelectObject(oldpen);
	pen.DeleteObject();
}


void MainWnd::DrawGradFrame(CDC *pdc, const CRect &outerrect, int width, unsigned int outercolor, unsigned int innercolor)
{
	_drawGradFrame(pdc, outerrect, width, outercolor, innercolor, 0);
}

void MainWnd::DrawGradPenFrame(CDC *pdc, const CRect &outerrect, int width, unsigned int outercolor, unsigned int innercolor)
{
	_drawGradFrame(pdc, outerrect, width, outercolor, innercolor, 1);
}

void MainWnd::_drawGradFrame(CDC *pdc, const CRect &outerrect, int width, unsigned int outercolor, unsigned int innercolor, bool usepen)
{
	unsigned int outr, outg, outb, inr, ing, inb;
	outr = (outercolor & 0xff0000) >> 16;
	outg = (outercolor & 0xff00) >> 8;
	outb = (outercolor & 0xff) ;
	inr = (innercolor & 0xff0000) >> 16;
	ing = (innercolor & 0xff00) >> 8;
	inb = (innercolor & 0xff) ;
	CRect rect = outerrect;

	int inc;
	if(width > 0)
		inc = 1;
	else
		inc = -1;
#define Get(a, b) ((int)a+((int)b - (int)a)*i/(width - inc))
	for(int i = 0; i != width; i += inc){
		if(usepen)
			DrawPenFrame(pdc, rect, RGB(Get(outr, inr), Get(outg, ing), Get(outb, inb)));
		else
			DrawFrame(pdc, rect, RGB(Get(outr, inr), Get(outg, ing), Get(outb, inb)));
		rect.left += inc;
		rect.top += inc;
		rect.right -= inc;
		rect.bottom -= inc;
	}
#undef Get
}