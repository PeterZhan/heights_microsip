#pragma once
#include "BaseDialog.h"
#include "explorer.h"
#include "afxwin.h"

// CDlgText dialog

class CDlgText : public CBaseDialog
{
	//DECLARE_DYNAMIC(CDlgText)

public:
	CDlgText(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgText();

// Dialog Data
	enum { IDD = IDD_DLGTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_iesend;
	CString superDocid;
	CString vdrDocid;
	CString strType;
	
	//CEdit m_text;
	afx_msg void OnBnClickedBtnsend();
	CString m_text;
	CEdit m_ctext;
	afx_msg void OnClose();
};
