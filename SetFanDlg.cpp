// SetFanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hupai.h"
#include "SimpleParser.h"
#include "SetFanDlg.h"
#include "hupaiDlg.h"
#include "resource.h"


// SetFanDlg dialog

IMPLEMENT_DYNAMIC(SetFanDlg, CDialog)

SetFanDlg::SetFanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetFanDlg::IDD, pParent)
{
	m_PP = (MainWnd*)pParent;
	m_HuAppendFan = 0;
	m_IgnoreRichChange = 1;
	m_HuTypeLock = 0;
	memset(m_DianAppendFan, 0, sizeof m_DianAppendFan);
	memset(m_DianFinalFan, 0, sizeof m_DianFinalFan);
}

SetFanDlg::~SetFanDlg()
{
}

void SetFanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(SetFanDlg, CDialog)
//	ON_STN_CLICKED(IDC_STATIC2, &SetFanDlg::OnStnClickedStatic2)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_HUTYPE_START, IDC_BUTTON_HUTYPE_START+4, OnBnClickedHuType)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_RICHEDIT_DIANFINALFAN_START, IDC_RICHEDIT_DIANFINALFAN_START+MAXPLAYER, OnEnChangeRichDianFinalFan)
	ON_EN_CHANGE(IDC_RICHEDIT_BASEFAN, &SetFanDlg::OnEnChangeRicheditBasefan)
	ON_EN_CHANGE(IDC_RICHEDIT_APPENDFAN, &SetFanDlg::OnEnChangeRicheditAppendfan)
	ON_EN_CHANGE(IDC_RICHEDIT_FINALFAN, &SetFanDlg::OnEnChangeRicheditFinalfan)
	ON_BN_CLICKED(IDC_CHECK_FASTSET, &SetFanDlg::OnBnClickedCheckFastset)
	ON_BN_CLICKED(IDOK, &SetFanDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SetFanDlg message handlers

BOOL SetFanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here

	if(m_PP->m_NumPlayers != 4 && m_PP->m_NumPlayers != 3){
		AfxMessageBox(L"error: m_PP->m_NumPlayers != 4 && m_PP->m_NumPlayers != 3");
		OnCancel();
	}
	
	if(m_PP->m_CurStat.m_At == -1 || m_PP->m_CurStat.m_FirstClick == -1){
		AfxMessageBox(L"error: m_PP->m_CurStat.m_At == -1 || m_PP->m_CurStat.m_FirstClick == -1");
		OnCancel();
	}

	if(m_PP->m_CurStat.m_InnerAt != 0){
		AfxMessageBox(L"warning: m_PP->m_CurStat.m_InnerAt != 0");
	}
	

	CRect cr;
	this->GetClientRect(&cr);
	RECTARR rectlst;
	INTARR idlst;
	_GetLayout(cr, rectlst, idlst, m_PP->m_NumPlayers);
	int lstcnt = 0;

	for(int i = 0; i < 4; i++){
		this->m_BtnHuType[i].Create(0, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_BITMAP , rectlst[lstcnt], this, idlst[lstcnt]);
		lstcnt++;
	}
	m_BtnHuType[0].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINBASE)));
	m_BtnHuType[1].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINPENG)));
	m_BtnHuType[2].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQING)));
	m_BtnHuType[3].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQINGPENG)));
	
	for(; lstcnt <= 9; lstcnt++){
		GetDlgItem(idlst[lstcnt])->MoveWindow(&rectlst[lstcnt], false);
	}
	
	GetDlgItem(IDC_RICHEDIT_BASEFAN)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[4].Height()*0.9), false);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_BASEFAN))->SetEventMask(((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_BASEFAN))->GetEventMask() | ENM_CHANGE);	
	GetDlgItem(IDC_STATIC_PLUS)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[5].Height()), false);
	GetDlgItem(IDC_RICHEDIT_APPENDFAN)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[6].Height() * 0.16), false);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_APPENDFAN))->SetEventMask(((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_APPENDFAN))->GetEventMask() | ENM_CHANGE);
	GetDlgItem(IDC_STATIC_EQUAL)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[7].Height()), false);
	GetDlgItem(IDC_RICHEDIT_FINALFAN)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[8].Height()*0.9), false);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_FINALFAN))->SetEventMask(((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_FINALFAN))->GetEventMask() | ENM_CHANGE);
	GetDlgItem(IDC_STATIC_FAN)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[9].Height()), false);

	for(int i = 0; i < m_PP->m_NumPlayers; i++){
		this->m_StaticPlayerName[i].Create(m_PP->m_PlayersInfo[i].m_Name, SS_CENTER|WS_VISIBLE|WS_CHILD, rectlst[lstcnt], this, idlst[lstcnt]);
		m_StaticPlayerName[i].SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[lstcnt].Height()));
		lstcnt++;
	}
	for(int i = 0; i < m_PP->m_NumPlayers; i++){
	//	this->m_RichDianFinalFan[i].CreateEx(0, WS_BORDER|WS_VISIBLE|WS_CHILD|WS_VSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN, rectlst[lstcnt], this, idlst[lstcnt]);
		this->m_RichDianFinalFan[i].CreateEx(516, 1344344196 & ~ES_AUTOHSCROLL, rectlst[lstcnt], this, idlst[lstcnt]);
		this->m_RichDianFinalFan[i].ShowScrollBar(SB_VERT, 1);
		this->m_RichDianFinalFan[i].ShowScrollBar(SB_VERT, 0);
		m_RichDianFinalFan[i].SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[lstcnt].Height()*0.3));
		m_RichDianFinalFan[i].SetEventMask(m_RichDianFinalFan[i].GetEventMask() | ENM_CHANGE);
		lstcnt++;
	}
	for(int i = 0; i < 2; i++){
		GetDlgItem(idlst[lstcnt])->MoveWindow(&rectlst[lstcnt], false);
		lstcnt++;
	}

	////do some initialization with HuPai information
	if(m_PP->m_CurStat.m_At == m_PP->m_CurStat.m_FirstClick){
		this->m_HuPlayer = m_PP->m_CurStat.m_At;
		m_DianPlayer = -1;
		m_Zi = 1;
	}
	else{
		m_HuPlayer = m_PP->m_CurStat.m_At;
		m_DianPlayer = m_PP->m_CurStat.m_FirstClick;
		m_Zi = 0;
	}
///// initialize FastSet
	((CButton*)GetDlgItem(IDC_CHECK_FASTSET))->SetCheck(m_PP->m_FastSet);
////////
	CString str, str2;
	str.Empty();

///// initialize HuAppendFan
	m_HuAppendFan = 0;
	if(m_PP->m_PlayersInfo[m_HuPlayer].m_MenQing && m_Zi){
		str += L"门清自摸，";
		m_HuAppendFan += 1;
	}
	if(m_PP->m_PlayersInfo[m_HuPlayer].m_MingGangCnt > 0){
		str2.Format(L"%d×明杠，", m_PP->m_PlayersInfo[m_HuPlayer].m_MingGangCnt);
		str += str2;
		m_HuAppendFan += m_PP->m_PlayersInfo[m_HuPlayer].m_MingGangCnt;
	}
	if(m_PP->m_PlayersInfo[m_HuPlayer].m_AnGangCnt > 0){
		str2.Format(L"%d×暗杠，", m_PP->m_PlayersInfo[m_HuPlayer].m_AnGangCnt);
		str += str2;
		m_HuAppendFan += m_PP->m_PlayersInfo[m_HuPlayer].m_AnGangCnt * 2;
	}
	if(m_HuAppendFan > 0){
		str.Delete(str.GetLength() - 1, 1);
		str2.Format(L"%d", m_HuAppendFan);
		str = L"（" + str + L"）\n" + str2;
	}
	else{
		str = L"0";
	}
	GetDlgItem(IDC_RICHEDIT_APPENDFAN)->SetWindowTextW(str);

///// fix DianAppendFan
	for(int i = 0; i < m_PP->m_NumPlayers; i++){
		m_DianAppendFan[i] = 0;
	}

	for(int i = 0; i < m_PP->m_NumPlayers; i++){
		if(i == m_HuPlayer || i != m_DianPlayer && m_Zi == 0)
			continue;
		str.Empty();
		if(m_PP->m_PlayersInfo[i].m_MingGangCnt > 0){
			str2.Format(L"%d×明杠，", m_PP->m_PlayersInfo[i].m_MingGangCnt);
			str += str2;
			m_DianAppendFan[i] += m_PP->m_PlayersInfo[i].m_MingGangCnt;
		}
		if(m_PP->m_PlayersInfo[i].m_AnGangCnt > 0){
			str2.Format(L"%d×暗杠，", m_PP->m_PlayersInfo[i].m_AnGangCnt);
			str += str2;
			m_DianAppendFan[i] += 2*m_PP->m_PlayersInfo[i].m_AnGangCnt;
		}
		if(m_DianAppendFan[i] > 0){
			str.Delete(str.GetLength() -1);
			str = L"（" + str + L"）\n";
		}
		this->m_RichDianFinalFan[i].SetWindowTextW(str);
	}

	Invalidate();
	m_IgnoreRichChange = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SetFanDlg::_GetLayout(const CRect &whole, RECTARR &rectlst, INTARR& idlst, int numplayers) const
{
	_GetLayout(whole, rectlst, numplayers);
	idlst.clear();
	for(int i = IDC_BUTTON_HUTYPE_START; i < IDC_BUTTON_HUTYPE_START+4; i++){
		idlst.push_back(i);
	}
	idlst.push_back(IDC_RICHEDIT_BASEFAN);
	idlst.push_back(IDC_STATIC_PLUS);
	idlst.push_back(IDC_RICHEDIT_APPENDFAN);
	idlst.push_back(IDC_STATIC_EQUAL);
	idlst.push_back(IDC_RICHEDIT_FINALFAN);
	idlst.push_back(IDC_STATIC_FAN);
	for(int i = IDC_STATIC_PLAYERNAME_START; i < IDC_STATIC_PLAYERNAME_START + numplayers; i++){
		idlst.push_back(i);
	}
	for(int i = IDC_RICHEDIT_DIANFINALFAN_START; i < IDC_RICHEDIT_DIANFINALFAN_START + numplayers; i++){
		idlst.push_back(i);
	}
	idlst.push_back(IDOK);
	idlst.push_back(IDCANCEL);

	////check
	for(int i = 0; i < idlst.size(); i++){
		if(idlst[i] >=  IDC_RICHEDIT_DIANFINALFAN_START + numplayers && idlst[i] < IDC_RICHEDIT_DIANFINALFAN_START + MAXPLAYER){
			AfxMessageBox(L"error: idlst[i] >=  IDC_RICHEDIT_DIANFINALFAN_START + numplayers && idlst[i] < IDC_RICHEDIT_DIANFINALFAN_START + MAXPLAYER");
			throw;
		}
	}
}

void SetFanDlg::_GetLayout(const CRect &whole, RECTARR &rectlst, int numplayers) const
{
	rectlst.clear();
	CRect upperrect = sR(whole, 0, 0, 1, 0.36);
	for(int i = 0; i < 4; i++){  ///upper 4 rects, from left to right
		rectlst.push_back(sR(sR(upperrect, 0.25*i, 0, 0.25*(i+1), 1), 0.05, 0.05, 0.95, 0.95));
	}
	CRect midrect = sR(whole, 0.02, 0.4, 0.98, 0.7);
	rectlst.push_back(sR(midrect, 0, 0.1, 0.15, 0.9)); /// IDC_RICHEDIT_BASEFAN
	rectlst.push_back(_getCenterRect(sR(midrect, 0.16, 0, 0.25, 1), 60, 120)); /// IDC_STATIC_PLUS
	rectlst.push_back(sR(midrect, 0.26, 0.1, 0.41, 0.9)); /// IDC_RICHEDIT_APPENDFAN
	rectlst.push_back(_getCenterRect(sR(midrect, 0.42, 0, 0.51, 1), 60, 120)); /// IDC_STATIC_EQUAL
	rectlst.push_back(sR(midrect, 0.52, 0.1, 0.67, 0.9)); /// IDC_RICHEDIT_FINALFAN
	rectlst.push_back(_getCenterRect(sR(midrect, 0.68, 0, 0.79, 1), 120, 120)); /// IDC_STATIC_FAN
	CRect lowerrect = sR(whole, 0.01, 0.74, 0.8, 0.98);
	CRect lowerrect_up = sR(lowerrect, 0, 0.05, 1, 0.4), lowerrect_down = sR(lowerrect, 0, 0.45, 1, 1);
	for(int i = 0; i < numplayers; i++){
		rectlst.push_back(sR(sR(lowerrect_up, 0.25*i, 0, 0.25*(i+1), 1), 0.05, 0, 0.95, 1)); /// m_StaticPlayerName
	}
	for(int i = 0; i < numplayers; i++){
		rectlst.push_back(sR(sR(lowerrect_down, 0.25*i, 0, 0.25*(i+1), 1), 0.05, 0, 0.95, 1)); /// m_RichDianFinalFan
	}
	rectlst.push_back(_getAlignRect(sR(whole, 0.75, 0.44, 0.98, 0.67), 160, 160, _ALIGNRIGHT)); ///IDOK
	rectlst.push_back(_getAlignRect(sR(whole, 0.75, 0.74, 0.98, 0.97), 160, 160, _ALIGNRIGHT)); ///IDCANCEL
}
BOOL SetFanDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_PP->m_FastSet){
		if(pMsg->message == WM_MOUSEMOVE){
			for(int i = 0; i < 4; i++){
				if(this->m_BtnHuType[i].GetSafeHwnd() == pMsg->hwnd){
					if(m_HuType != SRL2BITHU(i)){
						SetHuType(SRL2BITHU(i));
					}
					return CDialog::PreTranslateMessage(pMsg);
				}
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void SetFanDlg::OnBnClickedHuType(UINT id)
{
	int k = id - IDC_BUTTON_HUTYPE_START;
	if(!this->m_PP->m_FastSet){
		SetHuType(SRL2BITHU(k));
	}
	else{
		OnOK();
	}
	
}

void SetFanDlg::SetHuType(int bittype)
{
	m_HuTypeLock = 1;
	CString str;
	m_HuType = bittype;
	this->m_BaseFan = m_PP->m_TypeFan[BIT2SRLHU(bittype)];
	str.Format(L"%d", m_BaseFan);
	GetDlgItem(IDC_RICHEDIT_BASEFAN)->SetWindowTextW(str);

	this->SetWindowTextW(BIT2STRHU(m_HuType));
	m_HuTypeLock = 0;
}

void SetFanDlg::OnEnChangeRicheditBasefan()
{
	// TODO:  Add your control notification handler code here
	if(m_IgnoreRichChange)
		return;
	if(!m_HuTypeLock){
		m_HuType = USERDEFHU;
		this->SetWindowTextW(BIT2STRHU(m_HuType));
	}

	CString s;
	GetDlgItem(IDC_RICHEDIT_BASEFAN)->GetWindowTextW(s);
	if(_GetNum(s, this->m_BaseFan) != 0)
		return;
	m_HuFinalFan = m_BaseFan + m_HuAppendFan;
	GetDlgItem(IDC_RICHEDIT_FINALFAN)->GetWindowTextW(s);
	if(_SetAppendNum(m_HuFinalFan, s, false) == 0)
		GetDlgItem(IDC_RICHEDIT_FINALFAN)->SetWindowTextW(s);
	
}

void SetFanDlg::OnEnChangeRichDianFinalFan(UINT id)
{
	if(m_IgnoreRichChange)
		return;
	if(!m_HuTypeLock){
		m_HuType = USERDEFHU;
		this->SetWindowTextW(BIT2STRHU(m_HuType));
	}
//	int k = id - IDC_RICHEDIT_DIANFINALFAN_START;
}

int SetFanDlg::_GetNum(const CString &str, int &n)
{
	SimpleParser p;
	return p.GetNum(str, n);
}


int SetFanDlg::_SetAppendNum(int nset, CString &str, bool showplussign)
{
	SimpleParser p;
	int r = p.SetNum(nset, str, showplussign);
	if(r == 0)
		return 0;
	else if(r == 3){
		CString s;
		if(showplussign){
			s.Format(L"%+d", nset);
		}
		else{
			s.Format(L"%d", nset);
		}
		str += s;
		return 0;
	}
	else
		return r;
}

void SetFanDlg::OnEnChangeRicheditAppendfan()
{
	// TODO:  Add your control notification handler code here
	if(m_IgnoreRichChange)
		return;
	CString s;
	GetDlgItem(IDC_RICHEDIT_APPENDFAN)->GetWindowTextW(s);
	int n;
	if(_GetNum(s, n) != 0)
		return;
	m_HuAppendFan = n;
	m_HuTypeLock = 1;
	this->OnEnChangeRicheditBasefan();
	m_HuTypeLock = 0;
}

void SetFanDlg::OnEnChangeRicheditFinalfan()
{
	// TODO:  Add your control notification handler code here
	if(m_IgnoreRichChange)
		return;
	if(!m_HuTypeLock){
		m_HuType = USERDEFHU;
		this->SetWindowTextW(BIT2STRHU(m_HuType));
	}
	CString s;
	GetDlgItem(IDC_RICHEDIT_FINALFAN)->GetWindowTextW(s);
	int n;
	if(_GetNum(s, n) != 0)
		return;
	m_HuFinalFan = n;
	for(int i = 0; i < m_PP->m_NumPlayers; i++)
		m_DianFinalFan[i] = 0;

	m_IgnoreRichChange = 1;
	for(int i = 0; i < m_PP->m_NumPlayers; i++){
		if(i == m_HuPlayer || i != m_DianPlayer && m_Zi == 0)
			continue;
		m_DianFinalFan[i] = - m_HuFinalFan - m_DianAppendFan[i];
		m_DianFinalFan[m_HuPlayer] -= m_DianFinalFan[i];
	
		this->m_RichDianFinalFan[i].GetWindowTextW(s);
		if(_SetAppendNum(m_DianFinalFan[i], s, 1) == 0)
			this->m_RichDianFinalFan[i].SetWindowTextW(s);
	}
	this->m_RichDianFinalFan[m_HuPlayer].GetWindowTextW(s);
	if(_SetAppendNum(m_DianFinalFan[m_HuPlayer], s, 1) == 0)
		this->m_RichDianFinalFan[m_HuPlayer].SetWindowTextW(s);
	m_IgnoreRichChange = 0;
//	this->m_DianAppendFan
}

void SetFanDlg::OnBnClickedCheckFastset()
{
	// TODO: Add your control notification handler code here
	m_PP->m_FastSet = ((CButton*)GetDlgItem(IDC_CHECK_FASTSET))->GetCheck();
	((CButton*)m_PP->m_PP->GetDlgItem(IDC_CHECK_FASTSETMAIN))->SetCheck(m_PP->m_FastSet);
}

void SetFanDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void SetFanDlg::OnOK()
{
	CString err;
	err.Empty();
	for(int i = 0; i < m_PP->m_NumPlayers; i++){
		if(m_Zi == 0 && !(i == m_HuPlayer || i == m_DianPlayer))
			continue;
		CString s;
		this->m_RichDianFinalFan[i].GetWindowTextW(s);
		if(_GetNum(s, m_DianFinalFan[i]) != 0)
			err = err + L"玩家" + m_PP->m_PlayersInfo[i].m_Name + L"的番数无法识别\n";
	}
	if(err.IsEmpty()){
		int sum = 0;
		for(int i = 0; i < m_PP->m_NumPlayers; i++){
			sum += m_DianFinalFan[i];
		}
		if(sum != 0){
			AfxMessageBox(L"番数和不等于0");
			return;
		}
		CDialog::OnOK();
	}
	else{
		AfxMessageBox(err);
	}
}
