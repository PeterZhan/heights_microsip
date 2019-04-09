#pragma once
#include "explorer.h"
#include "BaseDialog.h"


// CDlgTop dialog

class CDlgTop : public CBaseDialog
{
	//DECLARE_DYNAMIC(CDlgTop)

public:
	CDlgTop(CWnd* pParent = NULL);   // standard constructor
	 ~CDlgTop();

// Dialog Data
	enum { IDD = IDD_DLGTOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_ietop;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
