// DlgFop.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgFop.h"
#include "afxdialogex.h"


// CDlgFop dialog

//IMPLEMENT_DYNAMIC(CDlgFop, CBaseDialog)

CDlgFop::CDlgFop(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgFop::IDD, pParent)
{
	this->m_hWnd = NULL;
	
	Create(IDD, pParent);

}

CDlgFop::~CDlgFop()
{
}

void CDlgFop::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_WEBBROWFOP2, m_webbrowser);
}


BEGIN_MESSAGE_MAP(CDlgFop, CBaseDialog)
END_MESSAGE_MAP()


// CDlgFop message handlers
