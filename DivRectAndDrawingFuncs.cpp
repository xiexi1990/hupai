#include "stdafx.h"
#include "MainWnd.h"


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
	}
	return 0;
}

int MainWnd::DivPlayerRect(const CRect &playerrect, RECTARR &rectlst)
{
	rectlst.clear();
	const double 
		
		
		vline_nameleft = 0, hline_nametop = 0.1, vline_nameright = 1-vline_nameleft, hline_namebottom = 0.45,
		vline_menleft = 0.3, hline_mentop = 0.02, vline_menright = 1-vline_menleft, hline_menbottom = 0.15,
		vline_mingleft = 0.02, hline_mingtop = 0.05, vline_mingright = 0.25, hline_mingbottom = 0.6,
		vline_anleft = 1-vline_mingright, hline_antop = hline_mingtop, vline_anright = 1-vline_mingleft, hline_anbottom = hline_mingbottom, 
		vline_fanleft = 0, hline_fantop = 0.45, vline_fanright = 1-vline_fanleft, hline_fanbottom = 0.75,
		vline_huleft = 0.1, hline_hutop = 0.75, vline_huright = 1-vline_huleft, hline_hubottom = 1;	

	CRect _stock;
	CRect b;

#define OFFSET b.OffsetRect(playerrect.TopLeft())
#define SETRECT(x) b.left = W * vline_##x##left;b.right = W * vline_##x##right;b.top = H * hline_##x##top;b.bottom = H * hline_##x##bottom;OFFSET;
#define SETRECTPUSH(x) SETRECT(x) rectlst.push_back(b);
	

	rectlst.push_back(_setRect(playerrect,        0,      0.1,        1,     0.45)); //name
	rectlst.push_back(_setRect(playerrect,        0,     0.45,        1,     0.75)); //fan
	rectlst.push_back(_setRect(playerrect,      0.3,     0.02,      0.7,     0.15)); //men
	rectlst.push_back(_setRect(playerrect,      0.1,     0.75,      0.9,     0.45)); //hu
	rectlst.push_back(_setRect(playerrect,     0.02,     0.05,     0.25,      0.6)); //ming
	rectlst.push_back(_setRect(ming,              0,        0,        1,      0.5)); //ming word
	rectlst.push_back(_setRect(ming,              0,      0.5,        1,        1)); //ming cnt
	rectlst.push_back(_setRect(playerrect,     0.75,     0.05,     0.98,      0.6)); //an
	rectlst.push_back(_setRect(an,                0,        0,        1,      0.5)); //an word
	rectlst.push_back(_setRect(an,                0,      0.5,        1,        1)); //an cnt

	SETRECTPUSH(name)
	SETRECTPUSH(fan)
	SETRECTPUSH(men)
	SETRECTPUSH(hu)
	SETRECTPUSH(ming)

	///ming word
	_stock = b;
	b.bottom -= _stock.Height() / 2;
	rectlst.push_back(b);

	///ming cnt
	b = _stock;
	b.top += _stock.Height() / 2;
	rectlst.push_back(b);

	SETRECTPUSH(an)
	
	///an word
	_stock = b;
	b.bottom -= _stock.Height() / 2;
	rectlst.push_back(b);

	///an cnt
	b = _stock;
	b.top += _stock.Height() / 2;
	rectlst.push_back(b);

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