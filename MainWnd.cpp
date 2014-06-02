// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "hupai.h"
#include "hupaiDlg.h"
#include "SetFanDlg.h"
#include "SetNameDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MainWnd

IMPLEMENT_DYNAMIC(MainWnd, CWnd)


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
	m_TypeFan[0] = BASEFAN;
	m_TypeFan[1] = PENGFAN;
	m_TypeFan[2] = QINGFAN;
	m_TypeFan[3] = QINGPENGFAN;
	m_PlusFan = PLUSFAN;
	m_MingFan = MINGFAN;
	m_AnFan = ANFAN;
	m_FastSet = 0;
	m_MenZiFan = MENZIFAN;
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
		//	pinfo.m_Name = L"½âÎö";
			pinfo.m_Sum = 0;
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
	this->SetFocus();
	this->GetPointAt(point, m_CurStat.m_At, m_CurStat.m_InnerAt);
	Refresh();
//	this->Invalidate(0);
}

void MainWnd::GetPointAt(const CPoint &point, int &at, int &innerat) const
{
	at = -1;
	innerat = 0;
	if(this->m_GoPrevRect.PtInRect(point)){
		at = 10;
	}
	else if(m_GoNextRect.PtInRect(point)){
		at = 11;
	}
	else if(m_NewGameRect.PtInRect(point)){
		at = 12;
	}
	else{
		for(int i = 0; i < this->m_NumPlayers; i++){
			if(this->m_PlayersInfo[i].m_WholeRect.PtInRect(point)){
				at = i;
				if(m_PlayersInfo[i].m_MingGangWrdRect.PtInRect(point)){
					innerat = 1;
				}
				else if(m_PlayersInfo[i].m_AnGangWrdRect.PtInRect(point)){
					innerat = 2;
				}
				else if(m_PlayersInfo[i].m_MenRect.PtInRect(point)){
					innerat = 3;
				}
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
	m_CurStat.m_At = at;
	m_CurStat.m_InnerAt = innerat;
	if(at == -1){
		m_CurStat.m_FirstClick = -1;
		return;
	}
	else if(at == 10){
		m_CurStat.m_FirstClick = -1;
		if(GoPrev() == 1){
			this->Invalidate(0);
		}
		return;
	}
	else if(at == 11){
		m_CurStat.m_FirstClick = -1;
		if(GoNext() == 1){
			this->Invalidate(0);
		}
		return;
	}
	else if(at == 12){
		m_CurStat.m_FirstClick = -1;
		NewGame();
		return;
	}
	else if(at >= 0 && at < this->m_NumPlayers){
		if(innerat != 0){
			RecordNode node;
			Operation op;
			op.m_OprType = Operation::OT_CHGPLAYERINFO;
			op.m_OprData[0] = at;
			if(innerat == 1){
				op.m_OprData[1] = 1;
				op.m_OprData[2] = 1;
				op.m_OprData[3] = m_PlayersInfo[at].m_MingGangCnt;
			}
			else if(innerat == 2){
				op.m_OprData[1] = 2;
				op.m_OprData[2] = 1;
				op.m_OprData[3] = m_PlayersInfo[at].m_AnGangCnt;
			}
			else if(innerat == 3){
				op.m_OprData[1] = 3;
				op.m_OprData[2] = m_PlayersInfo[at].m_MenQing ^ 1;
				op.m_OprData[3] = m_PlayersInfo[at].m_MenQing;
			}
			else{
				return;
			}
			DoOperation(op, 0);
			node.m_OprLst.push_back(op);
			m_Rcder.PushRcdNode(node);
			m_PP->RA(AnnounceRcdNode(node));
			
		}
		else{
			if(this->m_CurStat.m_FirstClick == -1){	
				m_CurStat.m_FirstClick = at;			
			}
			else{
				SetFanDlg dlg(this);
				if(dlg.DoModal() == IDOK){
					RecordNode node;
					Operation op;
					op.m_OprType = Operation::OT_DIAN;
					for(int i = 0; i < m_NumPlayers; i++){
						op.m_OprData[i] = dlg.m_DianFinalFan[i];
					}
					op.m_ExtraData.push_back(dlg.m_HuType);
					op.m_ExtraData.push_back(dlg.m_HuFinalFan);
		
					DoOperation(op, 0);
					node.m_OprLst.push_back(op);		
					this->m_Rcder.PushRcdNode(node);
					m_PP->RA(AnnounceRcdNode(node));
				}
				m_CurStat.m_FirstClick = -1;
			}
		}
	}
	else{
		//// error
	}

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
	this->SetFocus();
	Invalidate(0);
}

void MainWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(!m_RButtonDown)
		return;
	m_RButtonDown = 0;
	int at, innerat;
	GetPointAt(point, at, innerat);
	if(at == -1){
		return;
	}
	else if(at == 10){
		if(GoPrev() == 1){
			this->Invalidate(0);
		}
		return;
	}
	else if(at == 11){
		if(GoNext() == 1){
			this->Invalidate(0);
		}
		return;
	}
	else if(at == 12){
		NewGame();
		return;
	}
	else if(at >= 0 && at < this->m_NumPlayers){
		if(innerat != 0){
			RecordNode node;
			Operation op;
			op.m_OprType = Operation::OT_CHGPLAYERINFO;
			op.m_OprData[0] = at;
			if(innerat == 1){
				if(m_PlayersInfo[at].m_MingGangCnt == 0){
					this->Invalidate(0);
					return;
				}
				op.m_OprData[1] = 1;
				op.m_OprData[2] = -1;
				op.m_OprData[3] = m_PlayersInfo[at].m_MingGangCnt;
			}
			else if(innerat == 2){
				if(m_PlayersInfo[at].m_AnGangCnt == 0){
					this->Invalidate(0);
					return;
				}
				op.m_OprData[1] = 2;
				op.m_OprData[2] = -1;
				op.m_OprData[3] = m_PlayersInfo[at].m_AnGangCnt;
			}
			else if(innerat == 3){
				op.m_OprData[1] = 3;
				op.m_OprData[2] = m_PlayersInfo[at].m_MenQing ^ 1;
				op.m_OprData[3] = m_PlayersInfo[at].m_MenQing;
			}
			else{
				return;
			}
			DoOperation(op, 0);
			node.m_OprLst.push_back(op);
			m_Rcder.PushRcdNode(node);
			m_PP->RA(AnnounceRcdNode(node));
		}
		else{
			SetNameDlg dlg;
			dlg.m_Name = this->m_PlayersInfo[at].m_Name;
			if(dlg.DoModal() == IDOK){
				m_PlayersInfo[at].m_Name = dlg.m_Name;
				LVCOLUMN col;
				col.mask = LVCF_TEXT;
				col.pszText = (LPWSTR)dlg.m_Name.GetString();
				m_PP->m_ListCtrl_HuRcd.SetColumn(at + 1, &col);
			}
			if(this->m_CurStat.m_FirstClick == -1){
			}
			else{
			}
		}
	}
	else{
		////error
	}

__ret:
	this->Refresh();
	return;
}

void MainWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	OnRButtonDown(nFlags, point);
}

int MainWnd::GoPrev()
{
	RecordNode node = this->m_Rcder.GoPrev();
	if(node.m_NodeType == RecordNode::NT_NULL){
		return 1;
	}
	OPRLST::iterator it;
	for(it = node.m_OprLst.begin(); it != node.m_OprLst.end(); it++){
		UndoOperation(*it);
	}
	CString str = L"³·Ïú[";
	str += AnnounceRcdNode(node) + L"]";
	m_PP->RA(str);
	return 0;
}

int MainWnd::GoNext()
{
	RecordNode node = this->m_Rcder.GoNext();
	if(node.m_NodeType == RecordNode::NT_NULL){
		return 1;
	}
	OPRLST::iterator it;
	for(it = node.m_OprLst.begin(); it != node.m_OprLst.end(); it++){
		DoOperation(*it);
	}
	CString str = L"ÖØ¸´[";
	str += AnnounceRcdNode(node) + L"]";
	m_PP->RA(str);
	return 0;
}

CString MainWnd::AnnounceRcdNode(const RecordNode &node)
{
	CString s;
	if(node.m_NodeType == RecordNode::NT_NORMAL){
		OPRLST::const_iterator it;
		for(it = node.m_OprLst.begin(); it != node.m_OprLst.end(); it++){
			s += AnnounceOperation(*it) + L"; ";
		}
		if(!s.IsEmpty()){
			s.Delete(s.GetLength() - 2, 2);
		}
	}
	return s;
}

void MainWnd::NewGame()
{
	RecordNode node;
	Operation op;
	op.m_OprType = Operation::OT_NEWGAME;
	for(int i = 0; i < m_NumPlayers; i++){
		op.m_OprData[i] = 0;
		op.m_OprData[i] |= m_PlayersInfo[i].m_MingGangCnt & 7;
		op.m_OprData[i] |= (m_PlayersInfo[i].m_AnGangCnt & 7) << 3;
		op.m_OprData[i] |= (m_PlayersInfo[i].m_MenQing & 1) << 6;
	}
	DoOperation(op);
	node.m_OprLst.push_back(op);
	this->m_Rcder.PushRcdNode(node);
	m_PP->RA(AnnounceRcdNode(node));
}