#pragma once

#define IDC_BUTTON_PICHUTYPE_START 1500
#define IDC_BUTTON_CHECKHUTYPE_START 1510

// SetNameDlg dialog

class SetNameDlg : public CDialog
{
	DECLARE_DYNAMIC(SetNameDlg)

public:
	SetNameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetNameDlg();

	CString m_Name;
	int m_Hu;
	CButton m_BtnPicHuType[5];
	CButton m_BtnCheckHuType[5];
// Dialog Data
	enum { IDD = IDD_SETNAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnBnClickedPicHuType(UINT id);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
