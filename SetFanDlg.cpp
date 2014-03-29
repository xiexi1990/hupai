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
	DDX_Control(pDX, IDC_BUTTON1, m_BtnBase);
}


BEGIN_MESSAGE_MAP(SetFanDlg, CDialog)
//	ON_STN_CLICKED(IDC_STATIC2, &SetFanDlg::OnStnClickedStatic2)
END_MESSAGE_MAP()


// SetFanDlg message handlers

BOOL SetFanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	static CFont font1, font2;
	LOGFONT logfont;
	memset(&logfont, 0, sizeof LOGFONT);
	wcscpy(logfont.lfFaceName, L"Arial");
	logfont.lfWeight = FW_HEAVY;
	logfont.lfHeight = 50;
	font1.CreateFontIndirectW(&logfont);
	logfont.lfHeight = 250;
	font2.CreateFontIndirectW(&logfont);
	GetDlgItem(IDC_STATIC1)->SetFont(&font1);
	GetDlgItem(IDC_STATIC2)->SetFont(&font2);

	m_BtnBase.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
