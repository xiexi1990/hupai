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

//	GetDlgItem(IDC_STATIC1)->SetFont(&font1);
//	GetDlgItem(IDC_STATIC2)->SetFont(&font2);

//	m_BtnBase.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)));
//	m_BtnHuType
	if(m_PP->m_NumPlayers != 4 && m_PP->m_NumPlayers != 3){
		AfxMessageBox(L"error: can only deal with 4 or 3 players ");
		OnCancel();
	}
	CRect cr;
	this->GetClientRect(&cr);
	RECTARR rectlst;
	INTARR idlst;
	_GetLayout(cr, rectlst, idlst, m_PP->m_NumPlayers);

	wchar_t s[] = L"54321";

	for(int i = 0; i < 4; i++){
		this->m_BtnHuType[i].Create(LPCTSTR(s+i), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_BITMAP , rectlst[i], this, idlst[i]);
	}
	m_BtnHuType[0].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINBASE)));
	m_BtnHuType[1].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINPENG)));
	m_BtnHuType[2].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQING)));
	m_BtnHuType[3].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQINGPENG)));
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SetFanDlg::_GetLayout(const CRect &whole, RECTARR &rectlst, INTARR& idlst, int numplayers)
{
	_GetLayout(whole, rectlst, numplayers);
	idlst.clear();
	for(int i = 11; i <= 14; i++)
		idlst.push_back(i);
}

void SetFanDlg::_GetLayout(const CRect &whole, RECTARR &rectlst, int numplayers)
{
	rectlst.clear();
	CRect upperrect = _setRect(whole, 0, 0, 1, 0.36);
	for(int i = 0; i < 4; i++){  ///upper 4 rects, from left to right
		rectlst.push_back(_setRect(_setRect(upperrect, 0.25*i, 0, 0.25*(i+1), 1), 0.05, 0.05, 0.95, 0.95));
	//	rectlst.push_back(_setRect(_setRect(upperrect, 0.25*i, 0, 0.25*(i+1), 1), 0.15, 0.15, 0.85, 0.85));
	}

}