#include "StdAfx.h"
#include "microsip.h"
#include "RinginDlg.h"
#include "langpack.h"
#include "microsipDlg.h"


static CmicrosipDlg *microsipDlg;

RinginDlg::~RinginDlg(void)
{
}

RinginDlg::RinginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RinginDlg::IDD, pParent)
{
	microsipDlg = (CmicrosipDlg* )pParent;
	Create (IDD, pParent);
}

void RinginDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

BOOL RinginDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	TranslateDialog(this->m_hWnd);

	CFont* font = this->GetFont();
	LOGFONT lf;
	font->GetLogFont(&lf);

	lf.lfHeight = 12;
	m_font_ignore.CreateFontIndirect(&lf);
	GetDlgItem(IDC_IGNORE)->SetFont(&m_font_ignore);
	
	lf.lfHeight = 24;
	lf.lfWeight = FW_BOLD;
	m_font.CreateFontIndirect(&lf);
	GetDlgItem(IDC_CALLER_NAME)->SetFont(&m_font);

	GetDlgItem(IDC_IGNORE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CALLER_ADDR)->ModifyStyleEx(WS_EX_CLIENTEDGE,0,SWP_NOSIZE|SWP_FRAMECHANGED);
	int x,y;
	if (microsipDlg->ringinDlgs.GetCount())
	{
		CRect rect;
		microsipDlg->ringinDlgs.GetAt(microsipDlg->ringinDlgs.GetCount()-1)->GetWindowRect(&rect);
		x=rect.left+22;
		y=rect.top+22;
	} else {
		CRect ringinRect;
		GetWindowRect(&ringinRect);
		CRect primaryScreenRect;
		SystemParametersInfo(SPI_GETWORKAREA,0,&primaryScreenRect,0);
		//x = (primaryScreenRect.Width()-ringinRect.Width())/2;
		//y = (primaryScreenRect.Height()-ringinRect.Height())/2;
		x = (primaryScreenRect.Width() - ringinRect.Width());
		y = (primaryScreenRect.Height() - ringinRect.Height());
	}

	if (microsipDlg->IsWindowVisible()) {
		if (microsipDlg->IsIconic()) {
			microsipDlg->ShowWindow(SW_RESTORE);
		} else {
			microsipDlg->ShowWindow(SW_HIDE);
			microsipDlg->ShowWindow(SW_MINIMIZE);
			microsipDlg->ShowWindow(SW_RESTORE);
		}
	}

	HICON m_hIcon = theApp.LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);

//	m_Animator.Open(L"ringing.avi");
//	m_Animator.Play(0, -1, -1);
	

	::SetWindowPos(m_hWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);
	ShowWindow(SW_SHOWNORMAL);

	if (m_picture.Load(MAKEINTRESOURCE(IDR_RINGING), _T("GIF")))
		m_picture.Draw(); 

//	m_a.Open(_T("c:\\ringing.avi"));
//	m_animation_gif.Play(0, -1, -1);

	//m_hWndTop = ::GetForegroundWindow();

	//SetWindowPos(HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//::SetWindowPos(HWND_TOPMOST, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//::SetWindowPos(m_hWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(RinginDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &RinginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &RinginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ANSWER, &RinginDlg::OnBnClickedAudio)
	ON_BN_CLICKED(IDC_DECLINE, &RinginDlg::OnBnClickedDecline)
	ON_BN_CLICKED(IDC_VIDEO, &RinginDlg::OnBnClickedVideo)
END_MESSAGE_MAP()

void RinginDlg::OnClose() 
{

	//::SetWindowPos(messagesDlg->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
	Close();
}

void RinginDlg::Close(BOOL accept)
{
	int count = microsipDlg->ringinDlgs.GetCount();
	for (int i = 0; i < count; i++ )
	{
		if ( call_id == microsipDlg->ringinDlgs.GetAt(i)->call_id)
		{
			if (!accept) {
				microsipDlg->UpdateWindowText(_T("-"));
			}
			if (count==1) {
				microsipDlg->PlayerStop();//RingIn();
			}
			microsipDlg->ringinDlgs.RemoveAt(i);
			DestroyWindow();
			break;
		}
	}
}

void RinginDlg::OnBnClickedOk()
{
}

void RinginDlg::OnBnClickedCancel()
{
	Close();
}

void RinginDlg::OnBnClickedDecline()
{
	pjsua_call_info call_info;
	pjsua_call_get_info(call_id,&call_info);
	if (pjsua_call_hangup(call_id, 0, NULL, NULL) == PJ_SUCCESS) {
		microsipDlg->callIdImcomingIgnore = PjToStr(&call_info.call_id);
	}
	Close();
}

void RinginDlg::OnBnClickedAudio() 
{
	CallAccept();
}

void RinginDlg::OnBnClickedVideo()
{
	CallAccept(TRUE);
}

void RinginDlg::CallAccept(BOOL hasVideo)
{
	microsipDlg->onCallAnswer((WPARAM)call_id, (LPARAM)hasVideo);
	//Close(TRUE);
}

void RinginDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	SetTimer(IDT_TIMER_INIT_RINGIN, 1000, NULL);
	//if (microsipDlg->messagesDlg->tab->GetCurSel() != -1)
    microsipDlg->messagesDlg->ShowWindow(SW_SHOWNORMAL);
//	pjsua_call_info call_info;
//	pjsua_call_get_info(call_id, &call_info);
//if (microsipDlg->messagesDlg->tab->GetCurSel() != -1)
//	microsipDlg->messagesDlg->OnChangeTab();
}

void RinginDlg::OnTimer (UINT TimerVal)
{
	if (TimerVal == IDT_TIMER_INIT_RINGIN)
	{

		//SetForegroundWindow();
		//BringWindowToTop();

		//ShowWindow(SW_SHOWNORMAL);

		if (microsipDlg->IsIconic()) {
//			microsipDlg->ShowWindow(SW_RESTORE);
		}
		
		//::SetWindowPos(microsipDlg->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		//::SetFocus(microsipDlg->m_hWnd);
		
		//BringWindowToTop();
		//SetForegroundWindow();

		//microsipDlg->SetForegroundWindow();


		KillTimer(IDT_TIMER_INIT_RINGIN);
	}
}

void RinginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);



	//DDX_Control(pDX, IDC_ANIMATE_GIF, m_animation_gif);

	//DDX_Control(pDX, IDC_ANIMATE3, m_Animator);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	//DDX_Control(pDX, IDC_IE, m_ie);
}