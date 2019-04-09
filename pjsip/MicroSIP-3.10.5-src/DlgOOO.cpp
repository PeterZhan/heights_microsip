// DlgOOO.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgOOO.h"
#include "afxdialogex.h"
#include "microsipDlg.h"

// CDlgOOO dialog

static CmicrosipDlg *microsipDlg;

IMPLEMENT_DYNAMIC(CDlgOOO, CDialogEx)

CDlgOOO::CDlgOOO(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgOOO::IDD, pParent)
{

}

CDlgOOO::~CDlgOOO()
{
}

void CDlgOOO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOOO, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgOOO::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgOOO message handlers


void CDlgOOO::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	/*
	microsipDlg->ShowMessage(L"no");

	microsipDlg->isOOO =  microsipDlg->setooo(L"No");


	CString log = L"disabled out of office.";
	microsipDlg->setOperationLog(log);

	if (!microsipDlg->isOOO)
	{
		
		microsipDlg->ShowMessage(L"no");
		
		CDialogEx::OnOK();


	}

	*/
	//	this->CloseWindow();

	//   CDialogEx::OnOK();
}
