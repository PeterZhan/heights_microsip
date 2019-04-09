// DlgProspect.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgProspect.h"
#include "afxdialogex.h"


// CDlgProspect dialog

IMPLEMENT_DYNAMIC(CDlgProspect, CDialog)

CDlgProspect::CDlgProspect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProspect::IDD, pParent)
	, m_code(_T(""))
	, m_aptno(_T(""))
{

}

CDlgProspect::~CDlgProspect()
{
}

void CDlgProspect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITCODE, m_code);
	DDX_Text(pDX, IDC_EDITAPTNO, m_aptno);
}


BEGIN_MESSAGE_MAP(CDlgProspect, CDialog)
END_MESSAGE_MAP()


// CDlgProspect message handlers
