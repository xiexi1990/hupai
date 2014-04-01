// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "hupai.h"
#include "MainWnd.h"
#include "hupaiDlg.h"
#include "SetFanDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MainWnd

IMPLEMENT_DYNAMIC(MainWnd, CWnd)

MainWnd::MainWnd()
{
	AfxMessageBox(L"you cant use MainWnd() !");
	throw;
}

MainWnd::~MainWnd()
{
}

MainWnd::MainWnd(ChupaiDlg* pp)
{
	m_PP = pp;
	this->m_FirstOnShow = 1;
	MAXX = GetSystemMetrics(SM_CXSCREEN);
	MAXY = GetSystemMetrics(SM_CYSCREEN);
	m_WndRect.SetRectEmpty();
	m_NeedCalPlayerRects = 0;
	m_LButtonDown = 0;
	m_RButtonDown = 0;
}

BEGIN_MESSAGE_MAP(MainWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()



// MainWnd message handlers



void MainWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(this->m_FirstOnShow){
		m_FirstOnShow = 0;
		///initial DCs
		CDC *pdc = this->GetDC();
		this->m_dcmCrit.CreateCompatibleDC(pdc);
		this->m_dcmDraw1.CreateCompatibleDC(pdc);
		this->m_dcmPreDraw.CreateCompatibleDC(pdc);
		this->m_bmpCrit.CreateCompatibleBitmap(pdc, MAXX, MAXY);
		this->m_bmpDraw1.CreateCompatibleBitmap(pdc, MAXX, MAXY);
		this->m_bmpPreDraw.CreateCompatibleBitmap(pdc, MAXX, MAXY);
		this->m_defbmpCrit = m_dcmCrit.SelectObject(&m_bmpCrit);
		this->m_defbmpDraw1 = m_dcmDraw1.SelectObject(&m_bmpDraw1);
		this->m_defbmpPreDraw = m_dcmPreDraw.SelectObject(&m_bmpPreDraw);

		this->ReleaseDC(pdc);

		///end initial DCs

		InitializeCriticalSection(&m_csCheck);
		InitializeCriticalSection(&m_csCrit);
		InitializeCriticalSection(&m_csLoadImportantData);
		InitializeCriticalSection(&m_csLoadData);
		
		this->m_NumPlayers = 4;
	//	this->m_CurStat.m_At = -1;
		PlayerInfo pinfo;
	
		for(int i = 0; i < m_NumPlayers; i++){
			pinfo.m_Srl = i;
			pinfo.m_Name = char('A'+i);
			pinfo.m_Name = L"½âÎö";
			pinfo.m_Sum = 1000;
			pinfo.m_DefFan = 0;
			pinfo.m_MingGangCnt = pinfo.m_AnGangCnt = 0;
			this->m_PlayersInfo.push_back(pinfo);
		}

	//	m_NeedCalPlayerRects = 1;

		m_kGoing = false;
		m_Released = false;
		m_KDrawThread = AfxBeginThread(KDraw, (void*)this, 0, 0, CREATE_SUSPENDED);
		int a = m_KDrawThread->ResumeThread(), b = 1;
		TRACE("\nresume %d = %d", b++, a);
		this->Refresh();

	}

}

void MainWnd::Refresh()
{
//	bool kgoing;

	EnterCriticalSection(&m_csCheck);
	if(m_kGoing || m_Released){
		m_NeedRedraw = 1;
	}
	else{
		m_Released = 1;
		int a = 0;
		while(this->m_KDrawThread->ResumeThread() == 0){
			TRACE("\nEnter Here %d !!\n", ++a);
			Sleep(1);
		}
	
	}
	LeaveCriticalSection(&m_csCheck);

	
}


void MainWnd::OnSize(UINT nType, int cx, int cy)
{
//	CWnd::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(this->m_FirstOnShow)
		return;
	if(nType != SIZE_MINIMIZED){
		m_WndRect.right = cx;
		m_WndRect.bottom = cy;
		EnterCriticalSection(&this->m_csLoadImportantData);
		this->m_NeedCalPlayerRects = 1;
		LeaveCriticalSection(&this->m_csLoadImportantData);
		this->Refresh();
	}
}



void MainWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	int at, innerat;
	this->GetPointAt(point, at, innerat);
	if(innerat!= m_CurStat.m_InnerAt || at != m_CurStat.m_At){
		m_CurStat.m_At = at;
		m_CurStat.m_InnerAt = innerat;
		this->Invalidate(0);
	}
}

void MainWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_LButtonDown = 1;
	Refresh();
//	this->Invalidate(0);
}

void MainWnd::GetPointAt(const CPoint &point, int &at, int &innerat)
{
	at = -1;
	innerat = 0;
	for(int i = 0; i < this->m_NumPlayers; i++){
		if(this->m_PlayersInfo[i].m_WholeRect.PtInRect(point)){
			at = i;
			if(m_PlayersInfo[i].m_MingGangRect.PtInRect(point)){
				innerat = 1;
			}
			else if(m_PlayersInfo[i].m_AnGangRect.PtInRect(point)){
				innerat = 2;
			}
			else if(m_PlayersInfo[i].m_MenRect.PtInRect(point)){
				innerat = 3;
			}
		}
	}
}
void MainWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_LButtonDown)
		return;
	m_LButtonDown = 0;
	int at, innerat;
	GetPointAt(point, at, innerat);
	if(this->m_CurStat.m_FirstClick == -1){
		if(at == -1){
			return;
		//	goto __ret;
		}
		else{
			if(innerat == 1){
				this->m_PlayersInfo[at].m_MingGangCnt ++;
			}
			else if(innerat == 2){
				this->m_PlayersInfo[at].m_AnGangCnt ++;
			}
			else if(innerat == 3){
				this->m_PlayersInfo[at].m_MenQing ^= 1;
			}
			else{
				m_CurStat.m_FirstClick = at;
			}
		}
	}
	else{
		if(at == -1){
			m_CurStat.m_FirstClick = -1;
		}
		else{
			if(innerat == 1){
				this->m_PlayersInfo[at].m_MingGangCnt ++;
			}
			else if(innerat == 2){
				this->m_PlayersInfo[at].m_AnGangCnt ++;
			}
			else if(innerat == 3){
				this->m_PlayersInfo[at].m_MenQing ^= 1;
			}
			else{
				SetFanDlg dlg(this);
				dlg.DoModal();
				m_CurStat.m_FirstClick = at;
			}
		}
	}
	
//	this->Invalidate(0);
__ret:
	this->Refresh();
	return;
}

void MainWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OnLButtonDown(nFlags, point);
}

void MainWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_RButtonDown = 1;
	Invalidate(0);
}

void MainWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(!m_RButtonDown)
		return;
	m_RButtonDown = 0;
	int at, innerat;
	GetPointAt(point, at, innerat);
	if(this->m_CurStat.m_FirstClick == -1){
		if(at == -1){
			return;
	//		goto __ret;
		}
		else{
			if(innerat == 1){
				if(m_PlayersInfo[at].m_MingGangCnt == 0)
					goto __ret;
				this->m_PlayersInfo[at].m_MingGangCnt --;
			}
			else if(innerat == 2){
				if(m_PlayersInfo[at].m_AnGangCnt == 0)
					goto __ret;
				this->m_PlayersInfo[at].m_AnGangCnt --;
			}
			else if(innerat == 3){
				this->m_PlayersInfo[at].m_MenQing ^= 1;
			}
			else{
			}
		}
	}
	else{
		if(at == -1){
		}
		else{
			if(innerat == 1){
				if(m_PlayersInfo[at].m_MingGangCnt == 0)
					goto __ret;
				this->m_PlayersInfo[at].m_MingGangCnt --;
			}
			else if(innerat == 2){
				if(m_PlayersInfo[at].m_AnGangCnt == 0)
					goto __ret;
				this->m_PlayersInfo[at].m_AnGangCnt --;
			}
			else if(innerat == 3){
				this->m_PlayersInfo[at].m_MenQing ^= 1;
			}
			else{
			}
		}
	}
	
//	this->Invalidate(0);
__ret:
	this->Refresh();
	return;
}

void MainWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	OnRButtonDown(nFlags, point);
}

