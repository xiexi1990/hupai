// hupaiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hupai.h"
#include "hupaiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ChupaiDlg dialog

ChupaiDlg::ChupaiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ChupaiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_pMainWnd = NULL;
	m_Inited = 0;
}

void ChupaiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ChupaiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ChupaiDlg message handlers

BOOL ChupaiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_pMainWnd = new MainWnd(this);
	CRect r(10, 10, 810, 610);
	
	GetDlgItem(IDC_STATIC_NUMPLAYERS)->SetWindowTextW(L"人数");
	GetDlgItem(IDC_BUTTON_SET)->SetWindowTextW(L"设定");
	GetDlgItem(IDC_STATIC_HURCD)->SetWindowTextW(L"和牌记录");
	GetDlgItem(IDC_STATIC_MSGRCD)->SetWindowTextW(L"消息记录");
	GetDlgItem(IDC_RICHEDIT_NUMPLAYERS)->SetFont(theApp.m_FontRepository.AddGetHeight(18), false);

	m_pMainWnd->CreateEx(0, NULL, NULL, WS_CHILD | WS_VISIBLE, r, this, 1);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_Inited = 1;
	CRect clientrect;
	this->GetClientRect(&clientrect);
	this->OnSize(0, clientrect.Width(), clientrect.Height());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ChupaiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ChupaiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ChupaiDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnOK();

	this->m_pMainWnd->Refresh();
//	ReleaseMutex(this->m_pMainWnd->m_hMutexLetGo);
}

void ChupaiDlg::_GetLayout(const CRect& whole, RECTARR& rectlst, INTARR& idlst)
{
	idlst.clear();
	_GetLayout(whole, rectlst);
	idlst.push_back(1); /// mainwnd
	idlst.push_back(IDC_STATIC_NUMPLAYERS);
	idlst.push_back(IDC_RICHEDIT_NUMPLAYERS);
	idlst.push_back(IDC_BUTTON_SET);
	idlst.push_back(IDC_STATIC_HURCD);
	idlst.push_back(IDC_LIST_HURCD);
	idlst.push_back(IDC_STATIC_MSGRCD);
	idlst.push_back(IDC_RICHEDIT_MSGRCD);
}

void ChupaiDlg::_GetLayout(const CRect& whole, RECTARR& rectlst)
{
	rectlst.clear();
	int margin = 10;
	CRect ir, mainwndrect, tmp;
	ir.SetRect(whole.left + margin, whole.top + margin, whole.right - margin, whole.bottom - margin);

	rectlst.push_back(mainwndrect = _setRect(ir, 0, 0, 0.85, 1));  /// mainwnd
	
	////redefine tmp {
	tmp.left = mainwndrect.right + 2;
	tmp.top = ir.top;
	tmp.right = tmp.left + 30;
	tmp.bottom = tmp.top + 30;
	rectlst.push_back(_getCenterRect(tmp, 24, 14)); /// IDC_STATIC_NUMPLAYERS

	tmp.left = tmp.right + ir.Width()*0.01;
	tmp.right = ir.right - 60;
	if(tmp.Width() < 30)
		tmp.right = tmp.left + 30;
	rectlst.push_back(tmp); /// IDC_RICHEDIT_NUMPLAYERS

	tmp.left = tmp.right + 4;
	tmp.right = ir.right;
	rectlst.push_back(tmp); /// IDC_BUTTON_SET

	//// } tmp

	////redefine tmp {
	tmp.left = mainwndrect.right + ir.Width()*0.01;
	tmp.right = ir.right;
	tmp.top = ir.top + 120;
	tmp.bottom = tmp.top + 16;
	rectlst.push_back(_getAlignRect(tmp, 48, 14, _ALIGNLEFT | _ALIGNTOP)); /// IDC_STATIC_HURCD

	tmp.top = tmp.bottom;
	tmp.bottom = tmp.top + (ir.bottom - tmp.top - 20) / 2;
	rectlst.push_back(tmp); /// IDC_LIST_HURCD

	tmp.top = tmp.bottom + 4;
	tmp.bottom = tmp.top + 16;
	rectlst.push_back(_getAlignRect(tmp, 48, 14, _ALIGNLEFT | _ALIGNTOP)); /// IDC_STATIC_MSGRCD

	tmp.top = tmp.bottom;
	tmp.bottom = ir.bottom;
	rectlst.push_back(tmp);  /// IDC_RICHEDIT_MSGRCD

	//// } tmp
}

void ChupaiDlg::OnSize(UINT nType, int cx, int cy)
{
//	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(m_Inited == 0)
		return;
	if(nType != SIZE_MINIMIZED){
		CRect r(0, 0, cx, cy);
		INTARR idlst;
		RECTARR rectlst;
		_GetLayout(r, rectlst, idlst);
		if(m_pMainWnd && IsWindow(m_pMainWnd->m_hWnd)){
			this->m_pMainWnd->MoveWindow(&rectlst[0], false);
		}
		for(int i = 1; i < idlst.size(); i++){
			GetDlgItem(idlst[i])->MoveWindow(&rectlst[i], false);
		}

		Invalidate();
	}
}
