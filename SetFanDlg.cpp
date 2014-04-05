// SetFanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hupai.h"
#include "SetFanDlg.h"
#include "MainWnd.h"
#include "resource.h"


// SetFanDlg dialog

IMPLEMENT_DYNAMIC(SetFanDlg, CDialog)

SetFanDlg::SetFanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetFanDlg::IDD, pParent)
{
	m_PP = (MainWnd*)pParent;
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
END_MESSAGE_MAP()


// SetFanDlg message handlers

BOOL SetFanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here

	if(m_PP->m_NumPlayers != 4 && m_PP->m_NumPlayers != 3){
		AfxMessageBox(L"error: can only deal with 4 or 3 players ");
		OnCancel();
	}
	CRect cr;
	this->GetClientRect(&cr);
	RECTARR rectlst;
	INTARR idlst;
	_GetLayout(cr, rectlst, idlst, m_PP->m_NumPlayers);
	int lstcnt = 0;

	wchar_t s[] = L"54321";

	for(int i = 0; i < 4; i++){
		this->m_BtnHuType[i].Create(LPCTSTR(s+i), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_BITMAP , rectlst[lstcnt], this, idlst[lstcnt]);
		lstcnt++;
	}
	m_BtnHuType[0].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINBASE)));
	m_BtnHuType[1].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINPENG)));
	m_BtnHuType[2].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQING)));
	m_BtnHuType[3].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQINGPENG)));
	
	for(; lstcnt <= 9; lstcnt++){
		GetDlgItem(idlst[lstcnt])->MoveWindow(&rectlst[lstcnt], false);
	}
	GetDlgItem(IDC_STATIC_PLUS)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[5].Height()), false);
	GetDlgItem(IDC_STATIC_EQUAL)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[7].Height()), false);
	GetDlgItem(IDC_STATIC_FAN)->SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[9].Height()), false);

	for(int i = 0; i < m_PP->m_NumPlayers; i++){
		this->m_StaticPlayerName[i].Create(m_PP->m_PlayersInfo[i].m_Name, SS_CENTER|WS_VISIBLE|WS_CHILD, rectlst[lstcnt], this, idlst[lstcnt]);
		m_StaticPlayerName[i].SetFont(theApp.m_FontRepository.AddGetHeight(rectlst[lstcnt].Height()));
		lstcnt++;
	}
	for(int i = 0; i < m_PP->m_NumPlayers; i++){
	//	this->m_RichPlayerAppend[i].CreateEx(0, WS_BORDER|WS_VISIBLE|WS_CHILD|WS_VSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN, rectlst[lstcnt], this, idlst[lstcnt]);
		this->m_RichPlayerAppend[i].CreateEx(516, 1344344196, rectlst[lstcnt], this, idlst[lstcnt]);
		this->m_RichPlayerAppend[i].ShowScrollBar(SB_VERT, 1);
		this->m_RichPlayerAppend[i].ShowScrollBar(SB_VERT, 0);
		lstcnt++;
	}
	for(int i = 0; i < 2; i++){
		GetDlgItem(idlst[lstcnt])->MoveWindow(&rectlst[lstcnt], false);
		lstcnt++;
	}

	Invalidate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SetFanDlg::_GetLayout(const CRect &whole, RECTARR &rectlst, INTARR& idlst, int numplayers)
{
	_GetLayout(whole, rectlst, numplayers);
	idlst.clear();
	for(int i = 11; i <= 14; i++){
		idlst.push_back(i);
	}
	idlst.push_back(IDC_RICHEDIT_BASEFAN);
	idlst.push_back(IDC_STATIC_PLUS);
	idlst.push_back(IDC_RICHEDIT_APPENDFAN);
	idlst.push_back(IDC_STATIC_EQUAL);
	idlst.push_back(IDC_RICHEDIT_FINALFAN);
	idlst.push_back(IDC_STATIC_FAN);
	for(int i = 21; i <= 20 + numplayers; i++){
		idlst.push_back(i);
	}
	for(int i = 25; i <= 24 + numplayers; i++){
		idlst.push_back(i);
	}
	idlst.push_back(IDOK);
	idlst.push_back(IDCANCEL);
}

void SetFanDlg::_GetLayout(const CRect &whole, RECTARR &rectlst, int numplayers)
{
	rectlst.clear();
	CRect upperrect = sR(whole, 0, 0, 1, 0.36);
	for(int i = 0; i < 4; i++){  ///upper 4 rects, from left to right
		rectlst.push_back(sR(sR(upperrect, 0.25*i, 0, 0.25*(i+1), 1), 0.05, 0.05, 0.95, 0.95));
	}
	CRect midrect = sR(whole, 0.02, 0.4, 0.98, 0.7);
	rectlst.push_back(sR(midrect, 0, 0, 0.15, 1)); /// IDC_RICHEDIT_BASEFAN
	rectlst.push_back(_getCenterRect(sR(midrect, 0.16, 0, 0.25, 1), 60, 120)); /// IDC_STATIC_PLUS
	rectlst.push_back(sR(midrect, 0.26, 0, 0.41, 1)); /// IDC_RICHEDIT_APPENDFAN
	rectlst.push_back(_getCenterRect(sR(midrect, 0.42, 0, 0.51, 1), 60, 120)); /// IDC_STATIC_EQUAL
	rectlst.push_back(sR(midrect, 0.52, 0, 0.67, 1)); /// IDC_RICHEDIT_FINALFAN
	rectlst.push_back(_getCenterRect(sR(midrect, 0.68, 0, 0.79, 1), 120, 120)); /// IDC_STATIC_FAN
	CRect lowerrect = sR(whole, 0.01, 0.74, 0.8, 0.98);
	CRect lowerrect_up = sR(lowerrect, 0, 0.05, 1, 0.4), lowerrect_down = sR(lowerrect, 0, 0.45, 1, 1);
	for(int i = 0; i < numplayers; i++){
		rectlst.push_back(sR(sR(lowerrect_up, 0.25*i, 0, 0.25*(i+1), 1), 0.05, 0, 0.95, 1)); /// m_StaticPlayerName
	}
	for(int i = 0; i < numplayers; i++){
		rectlst.push_back(sR(sR(lowerrect_down, 0.25*i, 0, 0.25*(i+1), 1), 0.05, 0, 0.95, 1)); /// m_RichPlayerAppend
	}
	rectlst.push_back(_getAlignRect(sR(whole, 0.75, 0.44, 0.98, 0.67), 160, 160, _ALIGNRIGHT)); ///IDOK
	rectlst.push_back(_getAlignRect(sR(whole, 0.75, 0.74, 0.98, 0.97), 160, 160, _ALIGNRIGHT)); ///IDCANCEL
}