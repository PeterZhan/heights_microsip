#pragma once
#include "explorer.h"
#include "BaseDialog.h"


// CDlgBot dialog

class CDlgBot : public CBaseDialog
{
	//DECLARE_DYNAMIC(CDlgBot)

public:
	CDlgBot(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBot();

// Dialog Data
	enum { IDD = IDD_DLGBOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_expbot;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
