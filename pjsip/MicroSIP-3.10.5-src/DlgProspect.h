#pragma once


// CDlgProspect dialog

class CDlgProspect : public CDialog
{
	DECLARE_DYNAMIC(CDlgProspect)

public:
	CDlgProspect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProspect();

// Dialog Data
	enum { IDD = IDD_DIALOGTAGPROSP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_code;
	CString m_aptno;
};
