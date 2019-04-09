// DialogWeb.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DialogWeb.h"



// CDialogWeb dialog

//IMPLEMENT_DYNAMIC(CDialogWeb, CDialog)

CDialogWeb::CDialogWeb(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWeb::IDD, pParent)
{
	//this->m_hWnd = NULL;
	Create(IDD, pParent);
}

CDialogWeb::~CDialogWeb()
{
}

void CDialogWeb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER, m_web);
}


BEGIN_MESSAGE_MAP(CDialogWeb, CDialog)
END_MESSAGE_MAP()


// CDialogWeb message handlers


BOOL CDialogWeb::OnInitDialog()
{
	CDialog::OnInitDialog();

	//TranslateDialog(this->m_hWnd);


	//ShowWindow(SW_SHOWNORMAL);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
