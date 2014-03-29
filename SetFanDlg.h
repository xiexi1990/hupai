#pragma once
#include "afxwin.h"


// SetFanDlg dialog

class SetFanDlg : public CDialog
{
	DECLARE_DYNAMIC(SetFanDlg)

public:
	SetFanDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetFanDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnStnClickedStatic2();
	CButton m_BtnBase;
};
