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
		this->m_dcmBitmap.CreateCompatibleDC(pdc);

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
	else if(m_ChaRect.PtInRect(point)){
		at = 13;
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
				else{
					int bithulst[2], n;
					n = CheckBitHu(m_PlayersInfo[i].m_Hu, bithulst);
					if(n == 1 && m_PlayersInfo[i].m_HuRect.PtInRect(point)){
						innerat = 4;
					}
					else if(n == 2 && m_PlayersInfo[i].m_HuLeftRect.PtInRect(point)){
						innerat = 5;
					}
					else if(n == 2 && m_PlayersInfo[i].m_HuRightRect.PtInRect(point)){
						innerat = 6;
					}
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
	else if(at == 13){
		m_CurStat.m_FirstClick = -1;
		Cha();
		NewGame();
		return;
	}
	else if(at >= 0 && at < this->m_NumPlayers){
		RecordNode node;
		bool dorcd = 0;
		if(innerat == 1 || innerat == 2 || innerat == 3){	
			Operation op;
			op.m_OprType = Operation::OT_CHGPLAYERINFO;
			op.m_OprData[0] = at;
			if(innerat == 1){
				op.m_OprData[1] = 1;
				op.m_OprData[2] = 1;
				op.m_OprData[3] = m_PlayersInfo[at].m_MingGangCnt;
				node.m_OprLst.push_back(op);
				if(m_PlayersInfo[at].m_MenQing == 1){
					op.m_OprData[1] = 3;
					op.m_OprData[2] = 0;
					op.m_OprData[3] = m_PlayersInfo[at].m_MenQing;
					node.m_OprLst.push_back(op);
				}
			}
			else if(innerat == 2){
				op.m_OprData[1] = 2;
				op.m_OprData[2] = 1;
				op.m_OprData[3] = m_PlayersInfo[at].m_AnGangCnt;
				node.m_OprLst.push_back(op);
			}
			else if(innerat == 3){
				op.m_OprData[1] = 3;
				op.m_OprData[2] = m_PlayersInfo[at].m_MenQing ^ 1;
				op.m_OprData[3] = m_PlayersInfo[at].m_MenQing;
				node.m_OprLst.push_back(op);
			}
			else{
				return;
			}
			
			dorcd = 1;
		}
		else{
			if(this->m_CurStat.m_FirstClick == -1){	
				m_CurStat.m_FirstClick = at;			
			}
			else{
				if(innerat == 4 || innerat == 5 || innerat == 6){
					Operation op;
					op.m_OprType = Operation::OT_DIAN;
					int numhus, bithulst[2], srlhutype, hufinalfan, dianfinalfan[4], huplayer, dianplayer;
					numhus = CheckBitHu(m_PlayersInfo[at].m_Hu, bithulst);
					if(innerat == 4 || innerat == 5){
						srlhutype = BIT2SRLHU(bithulst[0]);
					}
					else
						srlhutype = BIT2SRLHU(bithulst[1]);
					huplayer = at;
					if(m_CurStat.m_FirstClick == at){
						dianplayer = -1;
					}
					else{
						dianplayer = m_CurStat.m_FirstClick;
					}
					for(int i = 0; i < m_NumPlayers; i++){
						dianfinalfan[i] = 0;
					}
					if(!(srlhutype >= 0 && srlhutype <= 3)){
						hufinalfan = 0;
					}
					else{
						hufinalfan = this->m_TypeFan[srlhutype];
						hufinalfan += m_PlayersInfo[huplayer].m_AnGangCnt * m_AnFan + m_PlayersInfo[huplayer].m_MingGangCnt * m_MingFan;
						if(dianplayer == -1 && m_PlayersInfo[huplayer].m_MenQing){
							hufinalfan += m_MenZiFan;
						}
						hufinalfan += m_PlusFan;
						for(int i = 0; i < m_NumPlayers; i++){
							if(i == huplayer || i != dianplayer && dianplayer != -1)
								continue;
							dianfinalfan[i] -= m_PlayersInfo[i].m_AnGangCnt * m_AnFan + m_PlayersInfo[i].m_MingGangCnt * m_MingFan;
						}
						if(dianplayer != -1){
							dianfinalfan[dianplayer] -= hufinalfan;
							dianfinalfan[huplayer] = -dianfinalfan[dianplayer];
						}
						else{
							for(int i = 0; i < m_NumPlayers; i++){
								if(i == huplayer)
									continue;
								dianfinalfan[i] -= hufinalfan;
								dianfinalfan[huplayer] -= dianfinalfan[i];
							}
						}
					}

					for(int i = 0; i < m_NumPlayers; i++){
						op.m_OprData[i] = dianfinalfan[i];
					}
					op.m_ExtraData.push_back(SRL2BITHU(srlhutype));
					op.m_ExtraData.push_back(hufinalfan);
					node.m_OprLst.push_back(op);
					dorcd = 1;
				}
				else{
					SetFanDlg dlg(this);
					if(dlg.DoModal() == IDOK){
						Operation op;
						op.m_OprType = Operation::OT_DIAN;
						for(int i = 0; i < m_NumPlayers; i++){
							op.m_OprData[i] = dlg.m_DianFinalFan[i];
						}
						op.m_ExtraData.push_back(dlg.m_HuType);
						op.m_ExtraData.push_back(dlg.m_HuFinalFan); 
						node.m_OprLst.push_back(op);
						dorcd = 1;
						if(dlg.m_HuType != USERDEFHU && dlg.m_HuType != ERRBITHU){
							int bithulst[2];
							if(CheckBitHu(m_PlayersInfo[dlg.m_HuPlayer].m_Hu, bithulst) < 2){
								Operation op;
								op.m_OprType = Operation::OT_CHGPLAYERINFO;
								op.m_OprData[0] = dlg.m_HuPlayer;
								op.m_OprData[1] = 7;
								op.m_OprData[2] = m_PlayersInfo[dlg.m_HuPlayer].m_Hu | dlg.m_HuType;
								op.m_OprData[3] = m_PlayersInfo[dlg.m_HuPlayer].m_Hu;
								node.m_OprLst.push_back(op);
							}
						}
					}				
				}
				m_CurStat.m_FirstClick = -1;
			}
		}
		if(dorcd){
			DoRcdNode(node, 0);
			m_Rcder.PushRcdNode(node);
			m_PP->RA(AnnounceRcdNode(node));
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
	else if(at == 13){
		Cha();
		NewGame();
		return;
	}
	else if(at >= 0 && at < this->m_NumPlayers){
		if(innerat == 1 || innerat == 2 || innerat == 3){
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
			node.m_OprLst.push_back(op);
			DoRcdNode(node, 0);
			m_Rcder.PushRcdNode(node);
			m_PP->RA(AnnounceRcdNode(node));
		}
		else{
			SetNameDlg dlg;
			dlg.m_Name = this->m_PlayersInfo[at].m_Name;
			dlg.m_Hu = this->m_PlayersInfo[at].m_Hu;
			if(dlg.DoModal() == IDOK){
				if(dlg.m_Name == m_PlayersInfo[at].m_Name && dlg.m_Hu == m_PlayersInfo[at].m_Hu){
					return;
				}
				if(dlg.m_Name != m_PlayersInfo[at].m_Name){
					m_PlayersInfo[at].m_Name = dlg.m_Name;
					LVCOLUMN col;
					col.mask = LVCF_TEXT;
					col.pszText = (LPWSTR)dlg.m_Name.GetString();
					m_PP->m_ListCtrl_HuRcd.SetColumn(at + 1, &col);
				}
				if(dlg.m_Hu != m_PlayersInfo[at].m_Hu){
					RecordNode node;
					Operation op;
					op.m_OprType = Operation::OT_CHGPLAYERINFO;
					op.m_OprData[0] = at;
					op.m_OprData[1] = 7;
					op.m_OprData[2] = dlg.m_Hu;
					op.m_OprData[3] = m_PlayersInfo[at].m_Hu;
					node.m_OprLst.push_back(op);
					DoRcdNode(node, 0);
					m_Rcder.PushRcdNode(node);
					m_PP->RA(AnnounceRcdNode(node));
				}
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
	UndoRcdNode(node);
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
	DoRcdNode(node);
	CString str = L"ÖØ¸´[";
	str += AnnounceRcdNode(node) + L"]";
	m_PP->RA(str);
	return 0;
}

int MainWnd::UndoRcdNode(const RecordNode& node, bool refresh)
{
	OPRLST::const_iterator it;
	for(it = node.m_OprLst.begin(); it != node.m_OprLst.end(); it++){
		UndoOperation(*it, refresh);
	}
	return 0;
}

int MainWnd::DoRcdNode(const RecordNode &node, bool refresh)
{
	OPRLST::const_iterator it;
	for(it = node.m_OprLst.begin(); it != node.m_OprLst.end(); it++){
		DoOperation(*it, refresh);
	}
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
		op.m_OprData[i] |= (m_PlayersInfo[i].m_Hu & 127) << 7;
	}
	node.m_OprLst.push_back(op);
	DoRcdNode(node);
	this->m_Rcder.PushRcdNode(node);
	m_PP->RA(AnnounceRcdNode(node));
}

void MainWnd::Reset()
{
	RecordNode node;
	Operation op;
	op.m_OprType = Operation::OT_NEWGAME;
	for(int i = 0; i < m_NumPlayers; i++){
		op.m_OprData[i] = 0;
		op.m_OprData[i] |= m_PlayersInfo[i].m_MingGangCnt & 7;
		op.m_OprData[i] |= (m_PlayersInfo[i].m_AnGangCnt & 7) << 3;
		op.m_OprData[i] |= (m_PlayersInfo[i].m_MenQing & 1) << 6;
		op.m_OprData[i] |= (m_PlayersInfo[i].m_Hu & 127) << 7;
	}
	node.m_OprLst.push_back(op);
	op.m_OprType = Operation::OT_CHGPLAYERINFO;
	for(int i = 0; i < m_NumPlayers; i++){
		op.m_OprData[0] = i;
		op.m_OprData[1] = 6;
		op.m_OprData[2] = 0;
		op.m_OprData[3] = m_PlayersInfo[i].m_Sum;
		node.m_OprLst.push_back(op);
	}
	DoRcdNode(node);
	this->m_Rcder.PushRcdNode(node);
	m_PP->RA(AnnounceRcdNode(node));
}

int MainWnd::CheckBitHu(int bithu, int bithulst[])
{
	int n = 0;
	for(int k = 1; k <= 32; k <<=1){
		if(bithu & k){
			if(n <2){
				bithulst[n] = k;
			}
			n++;
		}
	}
	return n;
}

void MainWnd::Cha()
{
	int bigesthu[4], punishfan[4] = {0};
	for(int i = 0; i < m_NumPlayers; i++){
		int k;
		for(k = ERRBITHU; k >= BASEHU; k >>= 1){
			if(m_PlayersInfo[i].m_Hu & k)
				break;
		}
		bigesthu[i] = k;
	}
	for(int i = 0; i < m_NumPlayers; i++){
		if(bigesthu[i] < BASEHU || bigesthu[i] == HUAZHU){
			for(int j = 0; j < m_NumPlayers; j++){
				if(j == i)
					continue;
				if(bigesthu[j] >= BASEHU && bigesthu[j] <= QINGPENGHU){
					int punish = m_TypeFan[BIT2SRLHU(bigesthu[j])];
					punish += (m_PlayersInfo[j].m_AnGangCnt + m_PlayersInfo[i].m_AnGangCnt)*m_AnFan + (m_PlayersInfo[j].m_MingGangCnt + m_PlayersInfo[i].m_MingGangCnt)*m_MingFan + m_PlayersInfo[j].m_MenQing*m_MenZiFan;
					if(bigesthu[i] == HUAZHU){
						punish *= 2;
					}
					punishfan[i] -= punish;
					punishfan[j] += punish;
				}
			}
		}
	}
	RecordNode node;
	Operation op;
	op.m_OprType = Operation::OT_CHA;
	for(int i = 0; i < m_NumPlayers; i++){
		op.m_OprData[i] = punishfan[i];
	}
	node.m_OprLst.push_back(op);
	DoRcdNode(node);
	this->m_Rcder.PushRcdNode(node);
	m_PP->RA(AnnounceRcdNode(node));
}