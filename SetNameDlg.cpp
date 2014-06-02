// SetNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hupai.h"
#include "SetNameDlg.h"


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
END_MESSAGE_MAP()


// SetNameDlg message handlers

BOOL SetNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
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
	CDialog::OnOK();
}
