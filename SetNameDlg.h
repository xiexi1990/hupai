#pragma once


// SetNameDlg dialog

class SetNameDlg : public CDialog
{
	DECLARE_DYNAMIC(SetNameDlg)

public:
	SetNameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetNameDlg();

	CString m_Name;
// Dialog Data
	enum { IDD = IDD_SETNAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
