#pragma once
#include "explorer.h"
#include "BaseDialog.h"

// CDlgFop dialog

class CDlgFop : public CBaseDialog
{
//	DECLARE_DYNAMIC(CDlgFop)

public:
	CDlgFop(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFop();

// Dialog Data
	enum { IDD = IDD_FOP2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_webbrowser;
};
