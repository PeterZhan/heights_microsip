#pragma once
#include "afxcmn.h"


// CDlgPhoneOOO dialog

class CDlgPhoneOOO : public CDialog
{
	DECLARE_DYNAMIC(CDlgPhoneOOO)

public:
	CDlgPhoneOOO(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPhoneOOO();

// Dialog Data
	enum { IDD = IDD_DLGPHONEOOO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_PhoneList;
	CString phone;
	CString phonelist;
	int setPhone(CString phoneList, CString phone);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnHdnItemclickListphone(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListphone(NMHDR *pNMHDR, LRESULT *pResult);
};
