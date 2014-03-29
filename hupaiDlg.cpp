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

void ChupaiDlg::OnSize(UINT nType, int cx, int cy)
{
//	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(m_Inited == 0)
		return;
	if(nType != SIZE_MINIMIZED){
		int margin = 10;
		double vline1 = 0.9, vline2 = 0.9;
		CRect dst;
		if(m_pMainWnd && IsWindow(m_pMainWnd->m_hWnd)){
			dst.left = margin;
			dst.top = margin;
			dst.right = margin + (cx - margin*2)*vline1;
			dst.bottom = cy - margin;
			this->m_pMainWnd->MoveWindow(&dst, false);
		}
		dst.bottom = cy - margin;
		dst.top = dst.bottom - 30;
		dst.right = cx - margin;
		dst.left = margin + (cx - margin*2)*vline2;
		if(dst.Width() > 80){
			dst.left = dst.right - 80;
		}
		GetDlgItem(IDCANCEL)->MoveWindow(&dst, false);
		dst.OffsetRect(0, -50);
		GetDlgItem(IDOK)->MoveWindow(&dst, false);
		Invalidate();
	}
}
