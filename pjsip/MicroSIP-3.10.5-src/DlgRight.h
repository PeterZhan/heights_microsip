#pragma once
#include "BaseDialog.h"
#include "afxwin.h"
#include "ACEdit.h"

// CDlgRight dialog

class CDlgRight : public CBaseDialog
{
	//DECLARE_DYNAMIC(CDlgRight)

public:
	CDlgRight(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRight();

	CString phoneNum;

	CString custDocid;

	CString superDocid;

	CString orgDocid;


	CToolTipCtrl m_ToolTip;
		
// Dialog Data
	enum { IDD = IDD_DLGRIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedBtncallsuper();
	CButton m_callSuper;
	afx_msg void OnClose();
	CButton m_sendLease;
	afx_msg void OnBnClickedBtnsendlease();
	CButton m_ebill;
	afx_msg void OnBnClickedBtnebill();

	void WriteHTML(const wchar_t* html);
	int ShowIE(CString url, CString tempStr);
	CButton m_textsuper;
	afx_msg void OnBnClickedBtntext();
	afx_msg void OnBnClickedBtntagcomp();
	CButton m_moveout;
	afx_msg void OnBnClickedBtnmove();
	afx_msg void OnBnClickedButton2();
	CButton m_rent;
	CButton m_image;
	afx_msg void OnBnClickedBtnrent();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CButton m_callrenew;
	CButton m_callrent;
	CButton m_callrentstate;
	afx_msg void OnBnClickedBtncallrenewal();
	afx_msg void OnBnClickedBtncallrent();
	afx_msg void OnBnClickedBtncallrentstate();
	virtual BOOL OnInitDialog();
	BOOL SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_complaint;
	int setItemIcon(int item, int icon);
	afx_msg void OnBnClickedBtnimage();
	CACEdit m_test;
	afx_msg void OnBnClickedBtncallrentstate2();
	CButton m_3DN;
	afx_msg void OnBnClickedBtncall3dn();
	afx_msg void OnBnClickedBtnprospect();
	CButton m_prospect;
	afx_msg void OnBnClickedBtnpreqcoi();
	CButton m_coi;
	afx_msg void OnBnClickedBtncharge();
	CButton m_charge;
};
