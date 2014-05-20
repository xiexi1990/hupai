#pragma once
#include "afxwin.h"

#define IDC_BUTTON_HUTYPE_START 11
#define IDC_STATIC_PLAYERNAME_START 21
#define IDC_RICHEDIT_DIANFINALFAN_START 25

#include"MainWnd.h"
// SetFanDlg dialog

class SetFanDlg : public CDialog
{
	DECLARE_DYNAMIC(SetFanDlg)

public:
	SetFanDlg(CWnd* pParent);   // standard constructor
	virtual ~SetFanDlg();
	
	MainWnd *m_PP;
	int m_HuPlayer, m_DianPlayer, m_HuAppendFan, m_DianAppendFan[MAXPLAYER], m_BaseFan, m_HuFinalFan, m_DianFinalFan[MAXPLAYER];
	int m_HuType;
	bool m_Zi, m_IgnoreRichChange, m_HuTypeLock;
	
// Dialog Data
	enum { IDD = IDD_SETFAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void _GetLayout(const CRect& whole, RECTARR& rectlst, int numplayers);
	void _GetLayout(const CRect& whole, RECTARR& rectlst, INTARR& idlst, int numplayers);
	int _GetNum(const CString& str, int& n);
//	int _SetNum(int nset, CString& str, bool showplussign);
	int _SetAppendNum(int nset, CString& str, bool showplussign);
	void SetHuType(int bittype);

	afx_msg void OnBnClickedHuType(UINT id);
	afx_msg void OnEnChangeRichDianFinalFan(UINT id);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnStnClickedStatic2();
	
	CButton m_BtnHuType[4];
	CRichEditCtrl m_RichDianFinalFan[MAXPLAYER];
	CStatic m_StaticPlayerName[MAXPLAYER];
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangeRicheditBasefan();
	afx_msg void OnEnChangeRicheditAppendfan();
	afx_msg void OnEnChangeRicheditFinalfan();
	afx_msg void OnBnClickedCheckFastset();
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnOK();
};
