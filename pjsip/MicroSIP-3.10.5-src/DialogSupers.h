#pragma once
#include "BaseDialog.h"
#include "explorer.h"

// CDialogSupers dialog

class CDialogSupers : public CBaseDialog
{
	//DECLARE_DYNAMIC(CDialogSupers)

public:
	CDialogSupers(CWnd* pParent = NULL);   // standard constructor
	~CDialogSupers();

// Dialog Data
	enum { IDD = IDD_DLGSUPERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_exp;
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void BeforeNavigate2Exp(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	void NavigateComplete2Exp(LPDISPATCH pDisp, VARIANT* URL);
};
