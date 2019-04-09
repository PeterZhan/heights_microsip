// DlgTop.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgTop.h"

#include "microsipDlg.h"

//#include "afxdialogex.h"

static CmicrosipDlg *microsipDlg;
// CDlgTop dialog

//IMPLEMENT_DYNAMIC(CDlgTop, CDialog)

CDlgTop::CDlgTop(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgTop::IDD, pParent)
{
	this->m_hWnd = NULL;
	microsipDlg = (CmicrosipDlg*)pParent;
	Create(IDD, pParent);
}

CDlgTop::~CDlgTop()
{
}

void CDlgTop::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPTOP, m_ietop);
}


BEGIN_MESSAGE_MAP(CDlgTop, CBaseDialog)
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgTop message handlers


void CDlgTop::OnMove(int x, int y)
{
	CBaseDialog::OnMove(x, y);

	
	CRect cRectTop;

	GetWindowRect(&cRectTop);
	int Mwidth = cRectTop.Width();

	CRect cRect;
	microsipDlg->GetWindowRect(&cRect);
	int MainWidth = cRect.Width();
	int MainHeight = cRect.Height();

	cRect.top = cRectTop.top + cRectTop.Height();
	cRect.left = cRectTop.left;// +cRect.Width();

	cRect.bottom = cRectTop.bottom + MainHeight;
	cRect.right = cRectTop.right;

	//	cRectMessage.top = cRect.top;
	//   cRectMessage.bottom = cRect.bottom;
	//	cRectMessage.left = cRect.left - Mwidth;
	//	cRectMessage.right = cRectMessage.left + Mwidth;

	if (microsipDlg->IsWindowVisible() && !microsipDlg->IsZoomed() && !microsipDlg->IsIconic())
	{
		microsipDlg->MoveWindow(&cRect);
	}




	

	// TODO: Add your message handler code here
}


void CDlgTop::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);


	if (!bShow) return;
	CRect cRectTop;

	GetWindowRect(&cRectTop);
	int Mwidth = cRectTop.Width();

	CRect cRect;
	microsipDlg->GetWindowRect(&cRect);

	cRectTop.top = cRect.top - cRectTop.Height();
	cRectTop.bottom = cRect.bottom - cRect.Height();
	cRectTop.left = cRect.left;
	cRectTop.right = cRect.right;

	//	if (IsWindowVisible() && !IsZoomed() && !IsIconic())
	MoveWindow(&cRectTop);


	// TODO: Add your message handler code here
}
