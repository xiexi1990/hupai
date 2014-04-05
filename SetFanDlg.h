#pragma once
#include "afxwin.h"

class MainWnd;
// SetFanDlg dialog

class SetFanDlg : public CDialog
{
	DECLARE_DYNAMIC(SetFanDlg)

public:
	SetFanDlg(CWnd* pParent);   // standard constructor
	virtual ~SetFanDlg();
	
	MainWnd *m_PP;
// Dialog Data
	enum { IDD = IDD_SETFAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void _GetLayout(const CRect& whole, RECTARR& rectlst, int numplayers);
	void _GetLayout(const CRect& whole, RECTARR& rectlst, INTARR& idlst, int numplayers);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnStnClickedStatic2();
	
	CButton m_BtnHuType[4];
	CRichEditCtrl m_RichPlayerAppend[4], m_RichScore[4];
	CStatic m_StaticPlayerName[4];
};
