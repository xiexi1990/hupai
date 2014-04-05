// hupai.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "FontRepository.h"


// ChupaiApp:
// See hupai.cpp for the implementation of this class
//

class ChupaiApp : public CWinApp
{
public:
	ChupaiApp();

//	CFont m_Font18, m_Font120;
	FontRepository m_FontRepository;
// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern ChupaiApp theApp;