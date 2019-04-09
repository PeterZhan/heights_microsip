#pragma once

#include "resource.h"
#include "const.h"

class Transfer :
	public CDialog
{
public:
	int attended = 0;


	Transfer(CWnd* pParent = NULL);	// standard constructor
	~Transfer();
	enum { IDD = IDD_TRANSFER };
	
protected:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	void TransSave(CComboBox *combobox);
	void TransLoad();
	afx_msg void OnEnChangeNumber1();
};
