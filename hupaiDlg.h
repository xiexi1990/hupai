// hupaiDlg.h : header file
//

#pragma once


// ChupaiDlg dialog

#include "MainWnd.h"
#include "afxcmn.h"

class ChupaiDlg : public CDialog
{
// Construction
public:
	ChupaiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data

	MainWnd *m_pMainWnd;
	bool m_Inited, m_CtrlDownNonEdit;
	void RA(const CString& str, int nlf = 1);

	enum { IDD = IDD_HUPAI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	void _GetLayout(const CRect& whole, RECTARR& rectlst);
	void _GetLayout(const CRect& whole, RECTARR& rectlst, INTARR& idlst);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCheckFastsetmain();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_ListCtrl_HuRcd;
	CRichEditCtrl m_RichEdit_MsgRcd;
	afx_msg void OnBnClickedButtonReset();
};
