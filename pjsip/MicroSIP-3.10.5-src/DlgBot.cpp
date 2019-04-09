// DlgBot.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgBot.h"
#include "afxdialogex.h"
#include "microsipDlg.h"

// CDlgBot dialog
static CmicrosipDlg *microsipDlg;
//IMPLEMENT_DYNAMIC(CDlgBot, CDialog)

CDlgBot::CDlgBot(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgBot::IDD, pParent)
{
	microsipDlg = (CmicrosipDlg*)pParent;
	Create(IDD, pParent);
}

CDlgBot::~CDlgBot()
{
}

void CDlgBot::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPBOT, m_expbot);
}


BEGIN_MESSAGE_MAP(CDlgBot, CBaseDialog)
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgBot message handlers


void CDlgBot::OnMove(int x, int y)
{
	CBaseDialog::OnMove(x, y);

	
	CRect cRectBot;

	GetWindowRect(&cRectBot);
	//int Mwidth = cRectBot.Width();

	CRect cRect;
	microsipDlg->messagesDlg->GetWindowRect(&cRect);
	int MainWidth = cRect.Width();
	int MainHeight = cRect.Height();

	cRect.top = cRectBot.top - cRect.Height();
	cRect.left = cRectBot.left;// +cRect.Width();

	cRect.bottom = cRectBot.bottom - cRect.Height();
	cRect.right = cRectBot.right;

	//	cRectMessage.top = cRect.top;
	//   cRectMessage.bottom = cRect.bottom;
	//	cRectMessage.left = cRect.left - Mwidth;
	//	cRectMessage.right = cRectMessage.left + Mwidth;

	if (microsipDlg->messagesDlg->IsWindowVisible() && !microsipDlg->messagesDlg->IsZoomed() && !microsipDlg->messagesDlg->IsIconic())
	{
		microsipDlg->messagesDlg->MoveWindow(&cRect);
	}

	

	// TODO: Add your message handler code here
}


void CDlgBot::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);



	if (!bShow) return;
	CRect cRectBot;

	GetWindowRect(&cRectBot);
//	int Mwidth = cRectBot.Width();

	CRect cRect;
	microsipDlg->messagesDlg->GetWindowRect(&cRect);

	cRectBot.top = cRect.top + cRect.Height();
	cRectBot.bottom = cRect.bottom + cRect.Height();
	cRectBot.left = cRect.left;// -cRect.Width();
	cRectBot.right = cRect.right;

	//	if (IsWindowVisible() && !IsZoomed() && !IsIconic())
	MoveWindow(&cRectBot);
	// TODO: Add your message handler code here
}
