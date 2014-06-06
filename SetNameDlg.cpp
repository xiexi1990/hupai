// SetNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hupai.h"
#include "SetNameDlg.h"
#include "MainWnd.h"


// SetNameDlg dialog

IMPLEMENT_DYNAMIC(SetNameDlg, CDialog)

SetNameDlg::SetNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetNameDlg::IDD, pParent)
{

}

SetNameDlg::~SetNameDlg()
{
}

void SetNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SetNameDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_PICHUTYPE_START, IDC_BUTTON_PICHUTYPE_START+4, OnBnClickedPicHuType)
END_MESSAGE_MAP()


// SetNameDlg message handlers

BOOL SetNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect r(10, 180, 788, 400), picr[5], checkr[5];
	for(int i = 0; i < 5; i++){
		picr[i] = _getCenterRect(_setRect(r, i*0.2, 0, (i+1)*0.2, 1), 120, 120);
		checkr[i] = _getAlignRect(_setRect(r, i*0.2, 0, (i+1)*0.2, 1), 14, 14, _ALIGNLEFT);
	}

	for(int i = 0; i < 5; i++){
		this->m_BtnPicHuType[i].Create(0, WS_CHILD|WS_VISIBLE|BS_BITMAP, picr[i], this, i+IDC_BUTTON_PICHUTYPE_START);
		this->m_BtnCheckHuType[i].Create(0, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, checkr[i], this, i+IDC_BUTTON_CHECKHUTYPE_START);
	
	}
	m_BtnPicHuType[0].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINBASE_S)));
	m_BtnPicHuType[1].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINPENG_S)));
	m_BtnPicHuType[2].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQING_S)));
	m_BtnPicHuType[3].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_WINQINGPENG_S)));
	m_BtnPicHuType[4].SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_HUAZHU_S)));

	{
		int bithulst[2], n;
		if((n = MainWnd::CheckBitHu(this->m_Hu, bithulst)) > 0){
			for(int i = 0; i < n; i++){
				int srl = BIT2SRLHU(bithulst[i]);
				if(srl < 5 && srl >= 0){
					this->m_BtnCheckHuType[srl].SetCheck(1);
				}
			}
		}
	}

	GetDlgItem(IDC_RICHEDIT_NAME)->SetFont(theApp.m_FontRepository.AddGetHeight(100), 0);
	GetDlgItem(IDC_RICHEDIT_NAME)->SetWindowTextW(m_Name);
	GetDlgItem(IDC_RICHEDIT_NAME)->SetFocus();
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_NAME))->SetSel(0, m_Name.GetLength());
	return false;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void SetNameDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	GetDlgItem(IDC_RICHEDIT_NAME)->GetWindowTextW(m_Name);
	m_Hu = 0;
	for(int i = 0; i < 5; i++){
		if(this->m_BtnCheckHuType[i].GetCheck()){
			this->m_Hu |= SRL2BITHU(i);
		}
	}
	CDialog::OnOK();
}

void SetNameDlg::OnBnClickedPicHuType(UINT id)
{
	int checked = 0;
	for(int i = 0; i < 5; i++){
		if(m_BtnCheckHuType[i].GetCheck())
			checked++;
	}
	int k = id - IDC_BUTTON_PICHUTYPE_START;
	if(checked >= 2 && m_BtnCheckHuType[k].GetCheck() == 0){
		return;
	}
	m_BtnCheckHuType[k].SetCheck(!m_BtnCheckHuType[k].GetCheck());
}