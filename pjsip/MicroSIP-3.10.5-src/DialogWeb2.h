#pragma once


// CDialogWeb2 dialog

class CDialogWeb2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogWeb2)

public:
	CDialogWeb2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogWeb2();

// Dialog Data
	enum { IDD = IDD_FORM_WEB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
