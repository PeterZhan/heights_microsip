#pragma once


// CDialogTest dialog

class CDialogTest : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTest)

public:
	CDialogTest(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogTest();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
