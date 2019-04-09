#pragma once
#include "explorer.h"


// CDialogWeb dialog

class CDialogWeb : public CDialog
{
	//DECLARE_DYNAMIC(CDialogWeb)

public:
	CDialogWeb(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogWeb();

// Dialog Data
	enum { IDD = IDD_FORM_WEB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_web;
	virtual BOOL OnInitDialog();
};
