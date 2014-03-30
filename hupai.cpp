// hupai.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "hupai.h"
#include "hupaiDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ChupaiApp

BEGIN_MESSAGE_MAP(ChupaiApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// ChupaiApp construction

ChupaiApp::ChupaiApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	LOGFONT logfont;
	memset(&logfont, 0, sizeof LOGFONT);
	wcscpy(logfont.lfFaceName, L"ËÎÌו");
	logfont.lfWeight = FW_HEAVY;
	logfont.lfHeight = 18;
	m_Font18.CreateFontIndirectW(&logfont);
	
}


// The one and only ChupaiApp object

ChupaiApp theApp;


// ChupaiApp initialization

BOOL ChupaiApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxInitRichEdit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	ChupaiDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
