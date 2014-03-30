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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
