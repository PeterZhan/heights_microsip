// DlgIE.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgIE.h"
//#include "afxdialogex.h"
#include "microsipDlg.h"

// CDlgIE dialog

static CmicrosipDlg *microsipDlg;

//IMPLEMENT_DYNAMIC(CDlgIE, CDialog)

CDlgIE::CDlgIE(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgIE::IDD, pParent)
{
	this->m_hWnd = NULL;
	microsipDlg = (CmicrosipDlg*)pParent;
	Create(IDD, pParent);
}

CDlgIE::~CDlgIE()
{
}

void CDlgIE::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IE, m_ie);
}


BEGIN_MESSAGE_MAP(CDlgIE, CBaseDialog)
END_MESSAGE_MAP()


// CDlgIE message handlers
