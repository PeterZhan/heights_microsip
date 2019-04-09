#pragma once
#include "explorer.h"
#include "BaseDialog.h"

// CDlgIE dialog

class CDlgIE : public CBaseDialog
{
//	DECLARE_DYNAMIC(CDlgIE)

public:
	CDlgIE(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgIE();

// Dialog Data
	enum { IDD = IDD_DIALOGIE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_ie;
};
