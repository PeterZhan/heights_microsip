#pragma once


// CDlgOOO dialog

class CDlgOOO : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOOO)

public:
	CDlgOOO(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOOO();

// Dialog Data
	enum { IDD = IDD_DIALOGOOO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
