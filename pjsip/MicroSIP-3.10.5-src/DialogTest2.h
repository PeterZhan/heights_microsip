#pragma once
#include "explorer.h"


// CDialogTest2 dialog

class CDialogTest2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTest2)

public:
	CDialogTest2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogTest2();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_ie;
};
