#include "StdAfx.h"
#include "Dialer.h"
#include "global.h"
#include "settings.h"
#include "microsipDlg.h"
#include "microsip.h"
#include "Strsafe.h"
#include "dialogweb2.h"


#include <atlbase.h>
#include <msxml6.h>
#include <comutil.h> 
//#include "dialogtest2.h"
#include "DlgPhoneOOO.h"

#include "WinHttpClient.h"
#include <time.h>

static CmicrosipDlg *microsipDlg;



 CComPtr<IXMLHTTPRequest> request;

extern void Request(int Method, LPCSTR Host, LPCSTR url, LPCSTR header, LPSTR data);


DWORD WINAPI ThreadHttp(LPVOID p)
{

	//getContactByType(L"V2");
	//return 0;
	//::PostMessage(microsipDlg->m_hWnd, UM_ALLCONTACTS_DONE, NULL, NULL);
	//	CString parturl = L"cmd=GETALLCONTACTSLISTS&exten=";
	//	parturl.Append(accountSettings.account.username);
	//parturl.Append(L"&log=");
	//parturl.Append(log);
	CString * purl = (CString *)p;
		//getHttpResult(*purl, *result);

	WinHttpClient client((wstring)(*purl));

	// Send HTTP request, a GET request by default.
	client.SendHttpRequest();

	// The response header.
	wstring httpResponseHeader = client.GetResponseHeader();

	// The response content.
	wstring httpResponseContent = client.GetResponseContent();


	//result = httpResponseContent.c_str();

	delete purl;
}


void getHttpResult(CString parturl, CString& result)
{
	/*
	try{
	char URL[1024];
	char* geturi = GENERAL_URL;
	wsprintfA(URL, geturi, (LPCSTR)(CStringA)(parturl));


	CComBSTR data(1024 * 1024*8*8);

	gethttp(URL, &data.m_str);


	result.Append(CString((_com_util::ConvertBSTRToString(data.m_str))));
	}
	catch (...)
	{
	}

	//ShowMessage(result);
	*/


	CString url = _T(GENERAL_URL2);
	url += parturl;
    std:wstring wurl(url);
	WinHttpClient client(wurl);

	// Send HTTP request, a GET request by default.
	client.SendHttpRequest();

	// The response header.
	wstring httpResponseHeader = client.GetResponseHeader();

	// The response content.
	wstring httpResponseContent = client.GetResponseContent();


	result = httpResponseContent.c_str();
	//CString result(httpResponseContent);


	//microsipDlg->ShowMessage(httpResponseHeader.c_str());



}


void getHttpResult2(CString parturl, CString& result)
{
	try{

	//	WCHAR* geturi = _T(GENERAL_URL);
		//	wsprintfA(URL, geturi, (LPCSTR)(CStringA)(parturl));

	//	wstring URL = _T(GENERAL_URL);// +parturl;

	//	WCHAR URL[1024];

	//	wsprintf(URL, geturi, (LPCSTR)(CStringA)(parturl));

	//	URL += parturl;
	//	wsprintfA(URL, (LPCSTR)(CStringA)(parturl));

		CString URL = _T(GENERAL_URL2);
		URL += parturl;

	//	microsipDlg->ShowMessage(URL);

	//	ShellExecute(NULL, _T("open"), URL,
	//		NULL, NULL, SW_SHOWNORMAL);

		WinHttpClient client((wstring)URL);

		client.SendHttpRequest();

		/*
		CComBSTR data(1024 * 8);

		gethttp(URL, &data.m_str);


		result.Append(CString((_com_util::ConvertBSTRToString(data.m_str))));*/
	}
	catch (...)
	{
	}

	//ShowMessage(result);


}




Dialer::Dialer(CWnd* pParent /*=NULL*/)
: CBaseDialog(Dialer::IDD, pParent)
{
	microsipDlg = (CmicrosipDlg* ) AfxGetMainWnd();
	Create (IDD, pParent);
}

Dialer::~Dialer(void)
{
}

void Dialer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KEY_1, m_ButtonDialer1);
	DDX_Control(pDX, IDC_KEY_2, m_ButtonDialer2);
	DDX_Control(pDX, IDC_KEY_3, m_ButtonDialer3);
	DDX_Control(pDX, IDC_KEY_4, m_ButtonDialer4);
	DDX_Control(pDX, IDC_KEY_5, m_ButtonDialer5);
	DDX_Control(pDX, IDC_KEY_6, m_ButtonDialer6);
	DDX_Control(pDX, IDC_KEY_7, m_ButtonDialer7);
	DDX_Control(pDX, IDC_KEY_8, m_ButtonDialer8);
	DDX_Control(pDX, IDC_KEY_9, m_ButtonDialer9);
	DDX_Control(pDX, IDC_KEY_0, m_ButtonDialer0);
	DDX_Control(pDX, IDC_KEY_STAR, m_ButtonDialerStar);
	DDX_Control(pDX, IDC_KEY_GRATE, m_ButtonDialerGrate);
	DDX_Control(pDX, IDC_DELETE, m_ButtonDialerDelete);
	DDX_Control(pDX, IDC_KEY_PLUS, m_ButtonDialerPlus);
	DDX_Control(pDX, IDC_CLEAR, m_ButtonDialerClear);
	//DDX_Control(pDX, IDC_ANIMATE_GIF, m_animation_gif);
	//DDX_Control(pDX, IDC_ANIMATE1, m_a1);
	DDX_Control(pDX, IDC_BUTTON_MODE_SELECTION, m_btnMode);
	DDX_Control(pDX, IDC_VMCOUNT, m_btnVM);
	DDX_Control(pDX, IDC_FOP2, m_btnFOP2);
	DDX_Control(pDX, IDC_DONTDIST, m_btnDND);
	DDX_Control(pDX, IDC_BUTTON_MUTE_INPUT, m_btnMuteMic);
	DDX_Control(pDX, IDC_BUTTON_MUTE_OUTPUT, m_btnMuteSpe);
	DDX_Control(pDX, IDC_SHOWCALL, m_showCall);
	DDX_Control(pDX, IDC_HIDECALL, m_hideCall);
	DDX_Control(pDX, IDC_BUTTONOOO, m_btnOOO);
	DDX_Control(pDX, IDC_BUTTONINOTES, m_btnINotes);
	DDX_Control(pDX, IDC_BUTTONDAILY, m_btnDailyListener);
	DDX_Control(pDX, ID_BTNDOOR, m_btnDoor);
	DDX_Control(pDX, IDC_BTNUNLOCK, m_unlock);
	DDX_Control(pDX, IDC_HIDEBUTTONS, m_hidebuttons);
	DDX_Control(pDX, IDC_SHOWBUTTONS, m_showbuttons);
	DDX_Control(pDX, IDC_ACEDIT, m_acedit);
}

BOOL Dialer::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	

	//*******************************************************************************
	//Create the ToolTip control
	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls, either by hard coded string 
		// or using the string table resource
		m_ToolTip.AddTool(&m_btnMode, _T("Headset or Speaker?"));
		//m_ToolTip.AddTool(&m_btnVM, _T("New Voicemail:"));
		m_ToolTip.AddTool(&m_btnVM, LPSTR_TEXTCALLBACK);
		
		m_ToolTip.AddTool(&m_btnFOP2, _T("FOP2 Monitor"));
		m_ToolTip.AddTool(&m_btnDND, _T("Enable or Disable DO NOT DISTURB"));
		m_ToolTip.AddTool(&m_btnMuteMic, _T("Mute/Unmute Microphone"));
		m_ToolTip.AddTool(&m_btnMuteSpe, _T("Mute/Unmute Speaker"));

		m_ToolTip.AddTool(&m_showCall, _T("Show Call Dialog"));
		m_ToolTip.AddTool(&m_hideCall, _T("Hide Call Dialog"));

		m_ToolTip.AddTool(&m_btnOOO, _T("Out Of Office"));

		

		m_ToolTip.AddTool(&m_btnINotes, _T("iNotes"));


		m_ToolTip.AddTool(&m_btnDailyListener, _T("Daily Call Listener"));

		m_ToolTip.AddTool(&m_unlock, _T("Unlock the Door"));
	//	CButton m_btnINotes;
	//	CButton m_btnDailyListene
        
		//m_ToolTip.SetTipBkColor(RGB())
	
	//	m_ToolTip.AddTool(&m_ButtonDialer6, _T("Open Door"));

		m_ToolTip.Activate(TRUE);
	}





    //*********************************************************************************
	TranslateDialog(this->m_hWnd);

	DialedLoad();
	
	CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
	combobox->SetWindowPos(NULL,0,0,combobox->GetDroppedWidth(),400,SWP_NOZORDER|SWP_NOMOVE);

	CFont* font = this->GetFont();
	LOGFONT lf;
	font->GetLogFont(&lf);
	lf.lfHeight = 22;
	StringCchCopy(lf.lfFaceName,LF_FACESIZE,_T("Franklin Gothic Medium"));
	m_font.CreateFontIndirect(&lf);
	combobox->SetFont(&m_font);
	
#if defined _GLOBAL_BALANCE_GTN || defined _GLOBAL_BALANCE_BEE || defined _GLOBAL_BALANCE_PLAIN || defined _GLOBAL_BALANCE_OPTIONS
	font->GetLogFont(&lf);
	lf.lfHeight = 16;
	m_font_balance.CreateFontIndirect(&lf);
	GetDlgItem(IDC_BALANCE)->SetFont(&m_font_balance);
#endif


#ifdef _GLOBAL_LOGO_DIALER
	m_hIcon = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_LOGO),
		IMAGE_ICON, 0, 0, LR_SHARED );
	CRect rect;
	GetWindowRect(&rect);
	ICONINFO info;
	GetIconInfo(m_hIcon,&info);
	GetObject(info.hbmColor,sizeof(bm),&bm);
	logoRect.left = (rect.Width()-bm.bmWidth)/2;
	CButton* but = (CButton*)GetDlgItem(IDC_CALL);
	but->GetWindowRect(&rect);
	ScreenToClient(rect);
	logoRect.top = rect.bottom + 15;
#endif

	GetDlgItem(IDC_KEY_1)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_2)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_3)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_4)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_5)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_6)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_7)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_8)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_9)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_0)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_STAR)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_GRATE)->SetFont(&m_font);
	GetDlgItem(IDC_KEY_PLUS)->SetFont(&m_font);
	GetDlgItem(IDC_CLEAR)->SetFont(&m_font);
	GetDlgItem(IDC_DELETE)->SetFont(&m_font);

	muteOutput = FALSE;
	muteInput = FALSE;

	CSliderCtrl *sliderCtrl;
	sliderCtrl = (CSliderCtrl *)GetDlgItem(IDC_VOLUME_OUTPUT);
	sliderCtrl->SetRange(0,200);
	sliderCtrl->SetPos(200-accountSettings.volumeOutput);
	sliderCtrl = (CSliderCtrl *)GetDlgItem(IDC_VOLUME_INPUT);
	sliderCtrl->SetRange(0,200);
	sliderCtrl->SetPos(200-accountSettings.volumeInput);

	m_hIconMuteOutput = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_MUTE_OUTPUT),
		IMAGE_ICON, 0, 0, LR_SHARED );
	((CButton*)GetDlgItem(IDC_BUTTON_MUTE_OUTPUT))->SetIcon(m_hIconMuteOutput);
	m_hIconMutedOutput = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_MUTED_OUTPUT),
		IMAGE_ICON, 0, 0, LR_SHARED );
	
	m_hIconMuteInput = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_MUTE_INPUT),
		IMAGE_ICON, 0, 0, LR_SHARED );
	((CButton*)GetDlgItem(IDC_BUTTON_MUTE_INPUT))->SetIcon(m_hIconMuteInput);
	m_hIconMutedInput = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_MUTED_INPUT),
		IMAGE_ICON, 0, 0, LR_SHARED );


	m_hIconHeadSet = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_MODE_HEADSET),
		IMAGE_ICON, 0, 0, LR_SHARED);
	

	((CButton*)GetDlgItem(IDC_BUTTON_MODE_SELECTION))->SetIcon(m_hIconHeadSet);
	

	m_hIconSpeaker = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_MODE_SPEAKER),
		IMAGE_ICON, 0, 0, LR_SHARED);


	
	m_hIconVoicemail = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_VOICEMAIL),
		IMAGE_ICON, 0, 0, LR_SHARED);
	((CButton*)GetDlgItem(IDC_VMCOUNT))->SetIcon(m_hIconVoicemail);


   HICON	m_hIconFop2 = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_FOP2),
		IMAGE_ICON, 0, 0, LR_SHARED);
	((CButton*)GetDlgItem(IDC_FOP2))->SetIcon(m_hIconFop2);

#ifndef _GLOBAL_TWO_BUTTONS
	m_hIconHold = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_HOLD),
		IMAGE_ICON, 0, 0, LR_SHARED );
	((CButton*)GetDlgItem(IDC_HOLD))->SetIcon(m_hIconHold);
	m_hIconTransfer = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_TRANSFER),
		IMAGE_ICON, 0, 0, LR_SHARED );
	((CButton*)GetDlgItem(IDC_TRANSFER))->SetIcon(m_hIconTransfer);
#endif
#ifdef _GLOBAL_VIDEO
	m_hIconVideo = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_VIDEO),
		IMAGE_ICON, 0, 0, LR_SHARED );
	((CButton*)GetDlgItem(IDC_VIDEO_CALL))->SetIcon(m_hIconVideo);
#endif
#ifndef _GLOBAL_NO_MESSAGING
	m_hIconMessage = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_MESSAGE),
		IMAGE_ICON, 0, 0, LR_SHARED );
	((CButton*)GetDlgItem(IDC_MESSAGE))->SetIcon(m_hIconMessage);
#endif

	m_hIconIdle = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONIDLE),
		IMAGE_ICON, 0, 0, LR_SHARED);

	m_hIconUnlock = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_UNLOCK),
		IMAGE_ICON, 0, 0, LR_SHARED);

	((CButton*)GetDlgItem(IDC_BTNUNLOCK))->SetIcon(m_hIconUnlock);

	m_hIconDonot = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONNOT),
		IMAGE_ICON, 0, 0, LR_SHARED);;

	if (accountSettings.denyIncoming == _T("all"))
		((CButton*)GetDlgItem(IDC_DONTDIST))->SetIcon(m_hIconDonot);
	else
		((CButton*)GetDlgItem(IDC_DONTDIST))->SetIcon(m_hIconIdle);


	HICON m_hIconOOO = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONNEWOOO),
		IMAGE_ICON, 0, 0, LR_SHARED); 

	((CButton*)GetDlgItem(IDC_BUTTONOOO))->SetIcon(m_hIconOOO);




	HICON m_hIconINotes = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONNEWINOTS),
		IMAGE_ICON, 0, 0, LR_SHARED);

	((CButton*)GetDlgItem(IDC_BUTTONINOTES))->SetIcon(m_hIconINotes);

	
	HICON m_hIconListener = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONLISTENER),
		IMAGE_ICON, 0, 0, LR_SHARED);

	((CButton*)GetDlgItem(IDC_BUTTONDAILY))->SetIcon(m_hIconListener);

	

	/*CString partUrl = L"cmd=GETINOTSURL&exten=";
	partUrl += accountSettings.account.username;

	*/
//	microsipDlg->getHttpResult(partUrl, iNotesUrl);
	
	m_acedit.Init();
	//m_acedit.SetFont(&m_font);
	//m_acedit.m_Liste.m_List.m.SetFont(&m_font);
	m_acedit.SetMode(_MODE_STANDARD_ | _MODE_FIND_ALL_);
	
	
	///SetFocus(m_acedit);
	//m_acedit.AddSearchString(L"1234567");

	GotoDlgCtrl(GetDlgItem(IDC_ACEDIT));
	getCurrentDateTime();

	return TRUE;
}

void Dialer::PostNcDestroy()
{
	CBaseDialog::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(Dialer, CBaseDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CALL, OnBnClickedCall)
	ON_BN_CLICKED(IDC_VMCOUNT, OnBnClickedVoicemail)
#ifdef _GLOBAL_VIDEO
	ON_BN_CLICKED(IDC_VIDEO_CALL, OnBnClickedVideoCall)
#endif
#ifndef _GLOBAL_NO_MESSAGING
	ON_BN_CLICKED(IDC_MESSAGE, OnBnClickedMessage)
#endif
#ifndef _GLOBAL_TWO_BUTTONS
	ON_BN_CLICKED(IDC_HOLD, OnBnClickedHold)
	ON_BN_CLICKED(IDC_TRANSFER, OnBnClickedTransfer)
#endif
	ON_BN_CLICKED(IDC_END, OnBnClickedEnd)
#ifdef _GLOBAL_CALL_TO_SKYPE
	ON_BN_CLICKED(IDC_CALL_SKYPE, OnBnClickedCallSkype)
#endif
	ON_BN_CLICKED(IDC_BUTTON_MUTE_OUTPUT, &Dialer::OnBnClickedMuteOutput)
	
	ON_BN_CLICKED(IDC_BUTTON_MUTE_INPUT, &Dialer::OnBnClickedMuteInput)

	ON_BN_CLICKED(IDC_BUTTON_MODE_SELECTION, &Dialer::OnBnClickedModeSel)

	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &Dialer::SetTipText)
///	ON_CBN_EDITCHANGE(IDC_NUMBER, &Dialer::OnCbnEditchangeComboAddr)
	ON_CBN_SELCHANGE(IDC_NUMBER, &Dialer::OnCbnSelchangeComboAddr)
	ON_BN_CLICKED(IDC_KEY_1, &Dialer::OnBnClickedKey1)
	ON_BN_CLICKED(IDC_KEY_2, &Dialer::OnBnClickedKey2)
	ON_BN_CLICKED(IDC_KEY_3, &Dialer::OnBnClickedKey3)
	ON_BN_CLICKED(IDC_KEY_4, &Dialer::OnBnClickedKey4)
	ON_BN_CLICKED(IDC_KEY_5, &Dialer::OnBnClickedKey5)
	ON_BN_CLICKED(IDC_KEY_6, &Dialer::OnBnClickedKey6)
	ON_BN_CLICKED(IDC_KEY_7, &Dialer::OnBnClickedKey7)
	ON_BN_CLICKED(IDC_KEY_8, &Dialer::OnBnClickedKey8)
	ON_BN_CLICKED(IDC_KEY_9, &Dialer::OnBnClickedKey9)
	ON_BN_CLICKED(IDC_KEY_STAR, &Dialer::OnBnClickedKeyStar)
	ON_BN_CLICKED(IDC_KEY_0, &Dialer::OnBnClickedKey0)
	ON_BN_CLICKED(IDC_KEY_GRATE, &Dialer::OnBnClickedKeyGrate)
	ON_BN_CLICKED(IDC_KEY_PLUS, &Dialer::OnBnClickedKeyPlus)
	ON_BN_CLICKED(IDC_CLEAR, &Dialer::OnBnClickedClear)
	ON_BN_CLICKED(IDC_DELETE, &Dialer::OnBnClickedDelete)
	ON_WM_VSCROLL()
#ifdef _GLOBAL_LOGO_DIALER
	ON_WM_PAINT()
#endif
	ON_BN_CLICKED(IDC_DONTDIST, &Dialer::OnBnClickedDontdist)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VOLUME_OUTPUT, &Dialer::OnNMCustomdrawVolumeOutput)
	ON_BN_CLICKED(IDC_FOP2, &Dialer::OnBnClickedFop2)
	ON_BN_CLICKED(IDC_SHOWCALL, &Dialer::OnBnClickedShowcall)
	ON_BN_CLICKED(IDC_HIDECALL, &Dialer::OnBnClickedHidecall)
	ON_BN_CLICKED(IDC_BUTTON2, &Dialer::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Dialer::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTONDAILY, &Dialer::OnBnClickedButtondaily)
	ON_BN_CLICKED(IDC_BUTTONINOTES, &Dialer::OnBnClickedButtoninotes)
	ON_BN_CLICKED(IDC_BUTTONOOO, &Dialer::OnBnClickedButtonooo)
	ON_BN_CLICKED(ID_BTNDOOR, &Dialer::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTNUNLOCK, &Dialer::OnBnClickedBtnunlock)
	ON_BN_CLICKED(IDC_HIDEBUTTONS, &Dialer::OnBnClickedHidebuttons)
	ON_BN_CLICKED(IDC_SHOWBUTTONS, &Dialer::OnBnClickedShowbuttons)
	ON_CBN_EDITCHANGE(IDC_NUMBER, &Dialer::OnCbnEditchangeNumber)
	ON_EN_CHANGE(IDC_ACEDIT, &Dialer::OnEnChangeAcedit)
	ON_EN_UPDATE(IDC_ACEDIT, &Dialer::OnEnUpdateAcedit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VOLUME_INPUT, &Dialer::OnNMCustomdrawVolumeInput)
END_MESSAGE_MAP()


BOOL Dialer::SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pResult)
{
	BOOL ret = FALSE;
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;
	UINT nID = pTTTStruct->idFrom; //得到相应窗口ID，有可能是HWND 
	if (pTTT->uFlags & TTF_IDISHWND) //表明nID是否为HWND 
	{
		nID = ::GetDlgCtrlID((HWND)nID);//从HWND得到ID值，当然你也可以通过HWND值来判断 
		switch (nID){
		case(IDC_VMCOUNT) :
			pTTT->lpszText = microsipDlg->vmcount.GetBuffer(1024);
			microsipDlg->vmcount.ReleaseBuffer();
			// :strcpy(pTTT->lpszText, microsipDlg->vmcount);//设置 
			ret = TRUE;
			break;
		}
	}

	return(ret);
}



BOOL Dialer::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);

	BOOL catched = FALSE;
	BOOL isEdit = FALSE;
	CEdit* edit = NULL;
	if (pMsg->message == WM_CHAR || (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)) {
		CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
		edit = (CEdit*)&m_acedit;// FindWindowEx(combobox->m_hWnd, NULL, _T("EDIT"), NULL);
		isEdit = !edit || edit == GetFocus();
	}
	if (pMsg->message == WM_CHAR)
	{
		if (pMsg->wParam == 48)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_0));
				OnBnClickedKey0();
				catched = TRUE;
			} else {
				DTMF(_T("0"));
			}
		} else if (pMsg->wParam == 49)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_1));
				OnBnClickedKey1();
				catched = TRUE;
			} else {
				DTMF(_T("1"));
			}
		} else if (pMsg->wParam == 50)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_2));
				OnBnClickedKey2();
				catched = TRUE;
			} else {
				DTMF(_T("2"));
			}
		} else if (pMsg->wParam == 51)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_3));
				OnBnClickedKey3();
				catched = TRUE;
			} else {
				DTMF(_T("3"));
			}
		} else if (pMsg->wParam == 52)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_4));
				OnBnClickedKey4();
				catched = TRUE;
			} else {
				DTMF(_T("4"));
			}
		} else if (pMsg->wParam == 53)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_5));
				OnBnClickedKey5();
				catched = TRUE;
			} else {
				DTMF(_T("5"));
			}
		} else if (pMsg->wParam == 54)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_6));
				OnBnClickedKey6();
				catched = TRUE;
			} else {
				DTMF(_T("6"));
			}
		} else if (pMsg->wParam == 55)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_7));
				OnBnClickedKey7();
				catched = TRUE;
			} else {
				DTMF(_T("7"));
			}
		} else if (pMsg->wParam == 56)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_8));
				OnBnClickedKey8();
				catched = TRUE;
			} else {
				DTMF(_T("8"));
			}
		} else if (pMsg->wParam == 57)
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_9));
				OnBnClickedKey9();
				catched = TRUE;
			} else {
				DTMF(_T("9"));
			}
		} else if (pMsg->wParam == 35 || pMsg->wParam == 47 )
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_GRATE));
				OnBnClickedKeyGrate();
				catched = TRUE;
			} else {
				DTMF(_T("#"));
			}
		} else if (pMsg->wParam == 42 )
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_STAR));
				OnBnClickedKeyStar();
				catched = TRUE;
			} else {
				DTMF(_T("*"));
			}
		} else if (pMsg->wParam == 43 )
		{
			if (!isEdit) {
				GotoDlgCtrl(GetDlgItem(IDC_KEY_PLUS));
				OnBnClickedKeyPlus();
				catched = TRUE;
			}
		} else if (pMsg->wParam == 8 || pMsg->wParam == 45 )
		{
			if (!isEdit)
			{
				GotoDlgCtrl(GetDlgItem(IDC_DELETE));
				OnBnClickedDelete();
				catched = TRUE;
			}
		} else if (pMsg->wParam == 46 )
		{
			if (!isEdit)
			{
				Input(_T("."), TRUE);
				catched = TRUE;
			}
		}
	} else if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {
			if (!isEdit) {
				//GotoDlgCtrl(GetDlgItem(IDC_NUMBER)); 
				GotoDlgCtrl(GetDlgItem(IDC_ACEDIT));
				catched = TRUE;
			}
			CString str;
			edit->GetWindowText(str);
			if (!str.IsEmpty()) {
				Clear();
				catched = TRUE;
			}
		}
	}
	if (!catched)
	{
		return CBaseDialog::PreTranslateMessage(pMsg);
	} else {
		return TRUE;
	}
}

void Dialer::OnBnClickedOk()
{
#ifndef _GLOBAL_TWO_BUTTONS
	if (accountSettings.singleMode && GetDlgItem(IDC_END)->IsWindowVisible()) {
#else
	if (accountSettings.singleMode && GetDlgItem(IDC_END)->IsWindowEnabled()) {
#endif
		OnBnClickedEnd();
	} else {
		OnBnClickedCall();
	}
}

void Dialer::OnBnClickedCancel()
{
	microsipDlg->ShowWindow(SW_HIDE);
}


void Dialer::Action(DialerActions action)
{
	CString number;
	CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
	combobox->GetWindowText(number);
	if (!number.IsEmpty()) {
		number.Trim();
#ifdef _GLOBAL_CONFERENCE_DIALOG
		microsipDlg->lastCallNumber = number;
#endif
		CString numberFormated = FormatNumber(number);
		pj_status_t pj_status = pjsua_verify_sip_url(StrToPj(numberFormated));
		if (pj_status==PJ_SUCCESS) {
			int pos = combobox->FindStringExact(-1,number);
			if (pos==CB_ERR || pos>0) {
				if (pos>0) {
					combobox->DeleteString(pos);
				} else if (combobox->GetCount()>=10)
				{
					combobox->DeleteString(combobox->GetCount()-1);
				}
				combobox->InsertString(0,number);
				combobox->SetCurSel(0);
			}
			DialedSave(combobox);
			if (!accountSettings.singleMode) {
				Clear();
			}
			microsipDlg->messagesDlg->AddTab(numberFormated, _T(""), TRUE, NULL, accountSettings.singleMode && action != ACTION_MESSAGE);
			if (action!=ACTION_MESSAGE) {
				microsipDlg->messagesDlg->Call(action==ACTION_VIDEO_CALL);
			}
		} else {
			ShowErrorMessage(pj_status);
		}
	}
}

void Dialer::OnBnClickedCall()
{
	/*CDialogTest2 * dlg = new CDialogTest2();
	dlg->Create(IDD_DIALOG3, this);
	dlg->ShowWindow(SW_NORMAL);

	CDialogWeb2 * dlg2 = new CDialogWeb2();
	dlg2->Create(IDD_FORM_WEB, this);
	dlg2->ShowWindow(SW_NORMAL);*/

	CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_NUMBER);
	CEdit* edit = (CEdit*)FindWindowEx(combobox->m_hWnd, NULL, _T("EDIT"), NULL);
	CString text;
	CString phonenum;
	m_acedit.GetWindowTextW(text);

	int p1 = text.Find(L"<");
	int p2 = text.Find(L">");

	if (p1 != -1 && p2 != -1)
	{
		phonenum = text.Mid(p1 + 1, p2 - p1 - 1);
		if (phonenum.Find(L"2123171423,") != -1){
			phonenum.Replace(L"2123171423,", L"");
		}
		
	}
	else
	{
		trimNoneNums(text, phonenum);

	}

	edit->SetWindowTextW(phonenum);
	Action(ACTION_CALL);
}


void Dialer::OnBnClickedVoicemail()
{
	//CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_NUMBER);
	m_acedit.SetWindowText(_T("*97"));
	OnBnClickedCall();

	CString log = L"dialed *97 for voicemail.";
	microsipDlg->setOperationLog(log);

}
#ifdef _GLOBAL_VIDEO
void Dialer::OnBnClickedVideoCall()
{
	Action(ACTION_VIDEO_CALL);
}
#endif

#ifndef _GLOBAL_NO_MESSAGING
void Dialer::OnBnClickedMessage()
{
	Action(ACTION_MESSAGE);
}
#endif

void Dialer::OnBnClickedHold()
{
	microsipDlg->messagesDlg->OnBnClickedHold();
}

void Dialer::OnBnClickedTransfer()
{
	if (!microsipDlg->transferDlg) {
		microsipDlg->transferDlg = new Transfer(this);
	}
	microsipDlg->transferDlg->SetForegroundWindow();
}

void Dialer::OnBnClickedEnd()
{
	MessagesContact*  messagesContact = microsipDlg->messagesDlg->GetMessageContact();
	if (messagesContact && messagesContact->callId != -1 ) {
		call_hangup_fast(messagesContact->callId);
	} else {
		call_hangup_all_noincoming();
	}
}

void Dialer::DTMF(CString digits, BOOL noLocalDTMF)
{
	BOOL simulate = TRUE;
	MessagesContact*  messagesContact = microsipDlg->messagesDlg->GetMessageContact();
	if (messagesContact && messagesContact->callId != -1 )
	{
		pjsua_call_info call_info;
		pjsua_call_get_info(messagesContact->callId, &call_info);
		if (call_info.media_status == PJSUA_CALL_MEDIA_ACTIVE)
		{
			pj_str_t pj_digits = StrToPjStr ( digits );
			if (pjsua_call_dial_dtmf(messagesContact->callId, &pj_digits) != PJ_SUCCESS) {
				simulate = !call_play_digit(messagesContact->callId, StrToPj(digits));
			}

			CString number = messagesContact->number;
			int pos = number.Find(_T("@"));

			if (pos != -1)
				number = number.Left(pos);

			if ((number == "301" || number == "1301") && digits == "6"){
				CString log = L"unlock the door.";
				microsipDlg->setOperationLog(log);
			}

		}
	}
	if (simulate && accountSettings.localDTMF && !noLocalDTMF) {
	//	::MessageBox(m_hWnd, _T(""), _T(""), MB_OK);
	//	microsipDlg->SetSoundDevice(microsipDlg->audio_output);
		call_play_digit(-1, StrToPj(digits));
	}
}

void Dialer::Input(CString digits, BOOL disableDTMF)
{
	if (!disableDTMF) {
		DTMF(digits);
	}
	//CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
	//CEdit* edit = (CEdit*)FindWindowEx(combobox->m_hWnd,NULL,_T("EDIT"),NULL);

//	CEdit* edit = (CEdit*)FindWindowEx(m_acedit.m_hWnd, NULL, _T("EDIT"), NULL);
//	if (edit) {
	int nLength = m_acedit.GetWindowTextLength();
	m_acedit.SetSel(nLength, nLength);
	m_acedit.ReplaceSel(digits);
//	}
//	CString text;
//	edit->GetWindowTextW(text);

	//m_acedit.SetWindowTextW(text);
}

void Dialer::DialedClear()
{
	//CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
	//combobox->ResetContent();
	//combobox->Clear();

	//m_acedit.SetWindowTextW(L"");

	m_acedit.SetWindowTextW(L"");

}
void Dialer::DialedLoad()
{
	CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
	CString key;
	CString val;
	LPTSTR ptr = val.GetBuffer(255);
	int i=0;
	while (TRUE) {
		key.Format(_T("%d"),i);
		if (GetPrivateProfileString(_T("Dialed"), key, NULL, ptr, 256, accountSettings.iniFile)) {
			combobox->AddString(ptr);
		} else {
			break;
		}
		i++;
	}
}

void Dialer::DialedSave(CComboBox *combobox)
{
	CString key;
	CString val;
	WritePrivateProfileString(_T("Dialed"), NULL, NULL, accountSettings.iniFile);
	for (int i=0;i < combobox->GetCount();i++)
	{
		int n = combobox->GetLBTextLen( i );
		combobox->GetLBText( i, val.GetBuffer(n) );
		val.ReleaseBuffer();

		key.Format(_T("%d"),i);
		WritePrivateProfileString(_T("Dialed"), key, val, accountSettings.iniFile);
	}
}


void Dialer::OnBnClickedKey1()
{
	Input(_T("1"));
}

void Dialer::OnBnClickedKey2()
{
	Input(_T("2"));
}

void Dialer::OnBnClickedKey3()
{
	Input(_T("3"));
}

void Dialer::OnBnClickedKey4()
{
	Input(_T("4"));
}

void Dialer::OnBnClickedKey5()
{
	Input(_T("5"));
}

void Dialer::OnBnClickedKey6()
{
	Input(_T("6"));
}

void Dialer::OnBnClickedKey7()
{
	Input(_T("7"));
}

void Dialer::OnBnClickedKey8()
{
	Input(_T("8"));
}

void Dialer::OnBnClickedKey9()
{
	Input(_T("9"));
}

void Dialer::OnBnClickedKeyStar()
{
	Input(_T("*"));
}

void Dialer::OnBnClickedKey0()
{
	Input(_T("0"));
}

void Dialer::OnBnClickedKeyGrate()
{
	Input(_T("#"));
}

void Dialer::OnBnClickedKeyPlus()
{
	Input(_T("+"), TRUE);
}

void Dialer::OnBnClickedClear()
{
	Clear();
}

void Dialer::Clear(bool update)
{
//	CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
//	combobox->SetCurSel(-1);
	if (update) {
		UpdateCallButton();
	}

	m_acedit.SetWindowTextW(L"");
}

void Dialer::OnBnClickedDelete()
{
//	CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
//	CEdit* edit = (CEdit*)FindWindowEx(combobox->m_hWnd,NULL,_T("EDIT"),NULL);
	CEdit* edit = (CEdit*)&m_acedit;// FindWindowEx(m_acedit.m_hWnd, NULL, _T("EDIT"), NULL);
	if (edit) {
		int nLength = edit->GetWindowTextLength();
		edit->SetSel(nLength-1,nLength);
		edit->ReplaceSel(_T(""));
	}

	////CString text;
	//edit->GetWindowTextW(text);

	//m_acedit.SetWindowTextW(text);
}

void Dialer::UpdateCallButton(BOOL forse, int callsCount)
{
#ifdef _GLOBAL_TWO_BUTTONS
	bool buttonDisabled = false;
#endif
	int len;
	if (!forse)	{
		//CComboBox *combobox= (CComboBox*)GetDlgItem(IDC_NUMBER);
		len = m_acedit.GetWindowTextLengthW();
	} else {
		len = 1;
	}
	CButton *button = (CButton *)GetDlgItem(IDC_CALL);
	bool state = false;
	if (accountSettings.singleMode)	{
		if (callsCount == -1) {
			callsCount = call_get_count_noincoming();
		}
#ifndef _GLOBAL_TWO_BUTTONS
		if (callsCount) {
			if (!GetDlgItem(IDC_END)->IsWindowVisible()) {
				button->ShowWindow(SW_HIDE);
#ifdef _GLOBAL_VIDEO
				GetDlgItem(IDC_VIDEO_CALL)->ShowWindow(SW_HIDE);
#endif
#ifndef _GLOBAL_NO_MESSAGING
				GetDlgItem(IDC_MESSAGE)->ShowWindow(SW_HIDE);
#endif
				GetDlgItem(IDC_HOLD)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_TRANSFER)->ShowWindow(SW_SHOW); 
				GetDlgItem(IDC_ATTDTRANSFER)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_CONFERENCE)->ShowWindow(SW_SHOW); 

				
				GetDlgItem(IDC_OPENDOOR)->ShowWindow(SW_SHOW);


				GetDlgItem(IDC_TOCELL)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_MERGE)->ShowWindow(SW_SHOW);
				
				GetDlgItem(IDC_END)->ShowWindow(SW_SHOW);
				GotoDlgCtrl(GetDlgItem(IDC_END));
			}
		} else {
			if (GetDlgItem(IDC_END)->IsWindowVisible()) {
				GetDlgItem(IDC_HOLD)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_TRANSFER)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_ATTDTRANSFER)->ShowWindow(SW_HIDE); 
				GetDlgItem(IDC_CONFERENCE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_OPENDOOR)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_TOCELL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_MERGE)->ShowWindow(SW_HIDE);
				
				GetDlgItem(IDC_END)->ShowWindow(SW_HIDE);
				button->ShowWindow(SW_SHOW);
#ifdef _GLOBAL_VIDEO
				GetDlgItem(IDC_VIDEO_CALL)->ShowWindow(SW_SHOW);
#endif
#ifndef _GLOBAL_NO_MESSAGING
				GetDlgItem(IDC_MESSAGE)->ShowWindow(SW_SHOW);
#endif
			}
		}
#else
		if (callsCount) {
			buttonDisabled = true;
			if (!GetDlgItem(IDC_END)->IsWindowEnabled()) {
				button->EnableWindow(FALSE);
				GetDlgItem(IDC_END)->EnableWindow(TRUE);
				GotoDlgCtrl(GetDlgItem(IDC_END));
			}
		} else {
			if (GetDlgItem(IDC_END)->IsWindowEnabled()) {
				GetDlgItem(IDC_END)->EnableWindow(FALSE);
				button->EnableWindow(TRUE);
			}
		}
#endif
		state = callsCount||len?true:false;
#ifdef _GLOBAL_PAGE_BUTTONS
		microsipDlg->pageButtons->GetDlgItem(IDC_CALL)->SetWindowText(callsCount?Translate(_T("End call")):callCaption);
		//		((CButton*)microsipDlg->pageButtons->GetDlgItem(IDC_CALL))->EnableWindow(callsCount||len?TRUE:FALSE);
#endif

	} else {
		state = len?true:false;
#ifdef _GLOBAL_PAGE_BUTTONS
		//		((CButton*)microsipDlg->pageButtons->GetDlgItem(IDC_CALL))->EnableWindow(len?TRUE:FALSE);
#endif
	}
	if (state==false && !GetFocus()) {
		//GotoDlgCtrl(GetDlgItem(IDC_NUMBER));
		GotoDlgCtrl(GetDlgItem(IDC_ACEDIT));
	}
#ifndef _GLOBAL_TWO_BUTTONS
	button->EnableWindow(state);
#else
	button->EnableWindow(!buttonDisabled && state);
#endif

#ifdef _GLOBAL_VIDEO
				GetDlgItem(IDC_VIDEO_CALL)->EnableWindow(state);
#endif
#ifndef _GLOBAL_NO_MESSAGING
				GetDlgItem(IDC_MESSAGE)->EnableWindow(state);
#endif

			//	CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_NUMBER);
			//	CEdit* edit = (CEdit*)FindWindowEx(combobox->m_hWnd, NULL, _T("EDIT"), NULL);
			//	CString text;
			//	edit->GetWindowTextW(text);

			//	m_acedit.SetWindowTextW(text);
}

void Dialer::SetNumber(CString  number)
{
	//CComboBox *combobox = &m_acedit;// (CComboBox*)GetDlgItem(IDC_NUMBER);
	CString old;
	m_acedit.GetWindowText(old);
	if (old.IsEmpty() || number.Find(old)!=0) {
		m_acedit.SetWindowText(number);
	}
	UpdateCallButton();
}

//void Dialer::OnCbnEditchangeComboAddr()
//{
//	UpdateCallButton();
//}

void Dialer::OnCbnSelchangeComboAddr()
{	
	UpdateCallButton(TRUE);
}

#ifdef _GLOBAL_LOGO_DIALER
void Dialer::OnPaint() 
{
	if (!IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		DrawIconEx(dc, logoRect.left, logoRect.top, m_hIcon, 0, 0, NULL, NULL, DI_MASK | DI_IMAGE);
	}
	else
	{
		CBaseDialog::OnPaint();
	}
}
#endif

void Dialer::OnLButtonUp( UINT nFlags, CPoint pt ) 
{
#ifndef _GLOBAL_LOGO_WEBSITE
	OnRButtonUp( nFlags, pt );
#else
	OnRButtonUp( 1, pt );
#endif
}

void Dialer::OnRButtonUp( UINT nFlags, CPoint pt )
{
#if defined _GLOBAL_LOGO_DIALER && defined _GLOBAL_MENU_WEBSITE
	if (pt.y>=logoRect.top && pt.y<=logoRect.top+bm.bmHeight
		&& pt.x>=logoRect.left && pt.x<=logoRect.left+bm.bmWidth
		) {
			if (!nFlags) {
				microsipDlg->MainPopupMenu();
			} else {
				microsipDlg->OnMenuWebsite();
			}
	}
#endif
}

void Dialer::OnMouseMove(UINT nFlags, CPoint pt )
{
#if defined _GLOBAL_LOGO_DIALER && defined _GLOBAL_MENU_WEBSITE && defined _GLOBAL_LOGO_WEBSITE
	if (pt.y>=logoRect.top && pt.y<=logoRect.top+bm.bmHeight
		&& pt.x>=logoRect.left && pt.x<=logoRect.left+bm.bmWidth
		) {
			::SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649))); 
	}
#endif
}

void Dialer::OnVScroll( UINT, UINT, CScrollBar* sender)
{
	if (pj_ready) {
		CSliderCtrl *sliderCtrl;
		int pos;
		int val;
		sliderCtrl = (CSliderCtrl *)GetDlgItem(IDC_VOLUME_OUTPUT);
		if (!sender || sender == (CScrollBar*)sliderCtrl)  {
			if (sender && muteOutput) {
				CButton *button = (CButton*)GetDlgItem(IDC_BUTTON_MUTE_OUTPUT);
				button->SetCheck(BST_UNCHECKED);
				OnBnClickedMuteOutput();
				return;
			}
			pos = muteOutput?0:200-sliderCtrl->GetPos();
			if (pos>100 && pos<110 || pos<100 && pos>90) {
				pos = 100;
				sliderCtrl->SetPos(pos);
			}
			val = pos<=100 ? pos : 100;
			pj_status_t status = 
				pjsua_snd_set_setting(
				PJMEDIA_AUD_DEV_CAP_OUTPUT_VOLUME_SETTING,
				&val, PJ_TRUE);
			if (status != PJ_SUCCESS) {
				pjsua_conf_adjust_tx_level(0, (float)val/100);
			}
			if (!muteOutput) {
				val = pos>100 ? 100+(pos-100)*3 : 100;
				pjsua_call_id call_ids[PJSUA_MAX_CALLS];
				unsigned calls_count = PJSUA_MAX_CALLS;
				if (pjsua_enum_calls ( call_ids, &calls_count)==PJ_SUCCESS)  {
					for (unsigned i = 0; i < calls_count; ++i) {
						pjsua_call_info call_info;
						pjsua_call_get_info(call_ids[i], &call_info);
						if (call_info.media_status == PJSUA_CALL_MEDIA_ACTIVE
							|| call_info.media_status == PJSUA_CALL_MEDIA_REMOTE_HOLD
							) {
								pjsua_conf_adjust_rx_level(call_info.conf_slot, (float)val/100);
						}
					}
				}
				accountSettings.volumeOutput = pos;
				microsipDlg->AccountSettingsPendingSave();
			}
		}
		sliderCtrl = (CSliderCtrl *)GetDlgItem(IDC_VOLUME_INPUT);
		if (!sender || sender == (CScrollBar*)sliderCtrl)  {
			if (sender && muteInput) {
				CButton *button = (CButton*)GetDlgItem(IDC_BUTTON_MUTE_INPUT);
				button->SetCheck(BST_UNCHECKED);
				OnBnClickedMuteInput();
				return;
			}
			pos = muteInput?0:200-sliderCtrl->GetPos();
			if (pos>100 && pos<110 || pos<100 && pos>90) {
				pos = 100;
				sliderCtrl->SetPos(pos);
			}
			pjsua_conf_adjust_rx_level(0, (pos>100?(100+pow((float)pos-100,1.4f)):(float)pos)/100);
			if (!muteInput) {
				accountSettings.volumeInput = pos;
				microsipDlg->AccountSettingsPendingSave();
			}
		}
	}
}

void Dialer::OnBnClickedMuteOutput()
{
	CButton *button = (CButton*)GetDlgItem(IDC_BUTTON_MUTE_OUTPUT);
	if (button->GetCheck() == BST_CHECKED) {
		button->SetIcon(m_hIconMutedOutput);
		muteOutput = TRUE;
		OnVScroll( 0, 0, NULL);
	} else {
		button->SetIcon(m_hIconMuteOutput);
		muteOutput = FALSE;
		OnVScroll( 0, 0, NULL);
	}
}

void Dialer::OnBnClickedModeSel()
{
	CButton *button = (CButton*)GetDlgItem(IDC_BUTTON_MODE_SELECTION);
	if (button->GetCheck() == BST_CHECKED) {
	//	button->SetWindowText(_T("S"));
		button->SetIcon(m_hIconSpeaker);
		accountSettings.s = 1;

		microsipDlg->m_bar.GetStatusBarCtrl().SetIcon(1, m_hIconSpeaker);
		microsipDlg->m_bar.SetPaneText(1, _T("Speaker"));


		microsipDlg->CURLOG = L"changed to speaker mode.";
		microsipDlg->setOperationLogInThread();

	}		
	else {
	//	button->SetWindowText(_T("H"));
		button->SetIcon(m_hIconHeadSet);
		accountSettings.s = 0;

		microsipDlg->m_bar.GetStatusBarCtrl().SetIcon(1, m_hIconHeadSet);
		microsipDlg->m_bar.SetPaneText(1, _T("Headset"));

		microsipDlg->CURLOG = L"changed to headset mode.";
		microsipDlg->setOperationLogInThread();
		
	}
	accountSettings.ModeChange();
	microsipDlg->UpdateSoundDevicesIds();
	
	microsipDlg->SetSoundDevice(microsipDlg->audio_output);
	OnVScroll(0, 0, NULL);
	//pjmedia_aud_dev_refresh();

}

void Dialer::OnBnClickedMuteInput()
{
	CButton *button = (CButton*)GetDlgItem(IDC_BUTTON_MUTE_INPUT);
	if (button->GetCheck() == BST_CHECKED) {
		button->SetIcon(m_hIconMutedInput);
		muteInput = TRUE;
		OnVScroll( 0, 0, NULL);
	} else {
		button->SetIcon(m_hIconMuteInput);
		muteInput = FALSE;
		OnVScroll( 0, 0, NULL);
	}
}

#ifdef _GLOBAL_CALL_TO_SKYPE
#include _GLOBAL_CALL_TO_SKYPE
void Dialer::OnBnClickedCallSkype()
{
	CallToSkype();
}
#endif


void Dialer::OnBnClickedDontdist()
{
	// TODO: Add your control notification handler code here
	CString msg = L"Are you sure to disable DO NOT DISTURB?";

	if (accountSettings.denyIncoming == _T("all"))
	{

		if (::MessageBox(
			this->m_hWnd,
			msg,
			(LPCWSTR)L"DO NOT DISTURB",
			MB_OKCANCEL
			) != IDOK) return;




		accountSettings.denyIncoming = _T("");
		CString log = L"disabled DO NOT DISTURB.";
		microsipDlg->setOperationLog(log);
	}
	else
	{

		msg = L"Are you sure to enable DO NOT DISTURB?";
		if (::MessageBox(
			this->m_hWnd,
			msg,
			(LPCWSTR)L"DO NOT DISTURB",
			MB_OKCANCEL
			) != IDOK) return;


		accountSettings.denyIncoming = _T("all");
		CString log = L"enabled DO NOT DISTURB.";
		microsipDlg->setOperationLog(log);

	}

	accountSettings.SettingsSave();




	if (accountSettings.denyIncoming == _T("all"))
		((CButton*)GetDlgItem(IDC_DONTDIST))->SetIcon(m_hIconDonot);
	else
		((CButton*)GetDlgItem(IDC_DONTDIST))->SetIcon(m_hIconIdle);


}


void Dialer::OnNMCustomdrawVolumeOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void Dialer::OnBnClickedFop2()
{
	// TODO: Add your control notification handler code here
//	::MessageBox(m_hWnd, _T("test"), _T("Captain"), MB_OK);
//	microsipDlg->dlgFop->ShowWindow(SW_SHOWNORMAL);
//	microsipDlg->dlgFop->SetWindowText(_T("FOP2"));
//	microsipDlg->dlgFop->m_webbrowser.Stop();
//	microsipDlg->dlgFop->m_webbrowser.Navigate(_T("http://asterisk.heightsre.com/fop2"), NULL,NULL, NULL, NULL);

	ShellExecute(NULL, _T("open"), _T("http://asterisk.heightsre.com/fop2"),
		NULL, NULL, SW_SHOWNORMAL);

	CString log = L"opened fop2 monitor web page.";
	microsipDlg->setOperationLog(log);

}


void Dialer::OnBnClickedShowcall()
{
	// TODO: Add your control notification handler code here
	
	if (microsipDlg->messagesDlg->tab->GetItemCount()>0)
	microsipDlg->messagesDlg->ShowWindow(SW_SHOWNORMAL);



	microsipDlg->pageDialer->m_showCall.ShowWindow(SW_HIDE);
	microsipDlg->pageDialer->m_hideCall.ShowWindow(SW_SHOW);









}


void Dialer::OnBnClickedHidecall()
{
	// TODO: Add your control notification handler code here

	microsipDlg->messagesDlg->ShowWindow(SW_HIDE);

	microsipDlg->pageDialer->m_showCall.ShowWindow(SW_SHOW);
	microsipDlg->pageDialer->m_hideCall.ShowWindow(SW_HIDE);
}

boolean gethttp(const char* url, BSTR* result)
{
	HRESULT hr;
	try{
		
		//	CComPtr<IXMLHTTPRequest> request;


		//	hr = request.CoCreateInstance(CLSID_XMLHTTP60);
		hr = request->open(
			_bstr_t("GET"),
			_bstr_t(url),
			_variant_t(VARIANT_FALSE),
			_variant_t(),
			_variant_t());

		hr = request->setRequestHeader(L"If-Modified-Since", L"0");

		hr = request->send(_variant_t());

		// get status - 200 if succuss
		long status;
		hr = request->get_status(&status);

		// load image data (if url points to an image)
		VARIANT responseVariant;


		//CString* result = new CString();
		//request->get_responseText
		//hr = request->get_responseStream(&responseVariant);
		hr = request->get_responseText(result);
		//	IStream* stream = (IStream*)responseVariant.punkVal;
		//	CImage *image = new CImage();
		//	image->Load(stream);
		//	stream->Release();


		//return result;

		

	}
	catch (...)
	{
	}

	return hr;

}





void Dialer::OnBnClickedButton2()
{

	WinHttpClient client(L"https://www.heightsre.com/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=GETALLCONTACTSLISTS&type=V2");

	// Send HTTP request, a GET request by default.
	client.SendHttpRequest();

	// The response header.
//	wstring httpResponseHeader = client.GetResponseHeader();

	// The response content.
//	wstring httpResponseContent = client.GetResponseContent();

	//CString result(httpResponseContent);

	client.SaveResponseToFile(L"response.dat");
	//microsipDlg->ShowMessage(httpResponseHeader.c_str());




	/*
	CComBSTR data(1024 * 1024);

	gethttp("https://www.heightsre.com/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=GETALLCONTACTSLISTS", &data.m_str);

	CString result;
	result.Append(CString((_com_util::ConvertBSTRToString(data.m_str))));
	

	
	microsipDlg->ShowMessage(result);
	
	
	*/
	
	
	// TODO: Add your control notification handler code here
#if 0
	char URL[1024];
	char* geturi = "https://www.heightsre.com/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=CHECKNEWVERSION&version=%s";
	wsprintfA(URL, geturi, (LPCSTR)(CStringA)(_GLOBAL_VERSION));

//	char data[1024];
//	Request(GET, "www.heightsre.com", URL, NULL, data);

	CString * data = new CString();
	//gethttp(URL, data);

	//::MessageBox(m_hWnd, *data, _T("url"), MB_OK);

	delete data;
#endif
	/*
	if (microsipDlg->getooo())
		microsipDlg->ShowMessage(L"Yes ooo");
	else
		microsipDlg->ShowMessage(L"No ooo");



	if (microsipDlg->setooo(L"Yes"))
		microsipDlg->ShowMessage(L"Yes ooo");
	else
		microsipDlg->ShowMessage(L"No ooo");


	CString phone = L"5193124762";

    microsipDlg->setooophone(phone);

	microsipDlg->ShowMessage(L"phone saved");

	phone = L"";
	*/
	/*CString phone = L"";
	microsipDlg->getooophone(phone);

	microsipDlg->ShowMessage(phone);

	phone = L"";
	microsipDlg->getooophonelist(phone);

	microsipDlg->ShowMessage(phone);


	*/

}


void Dialer::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here




	CDlgPhoneOOO dlgPhone(this);

	dlgPhone.setPhone(microsipDlg->PhoneList, microsipDlg->PhoneTrans);

	if (dlgPhone.DoModal() != IDOK) return;


	microsipDlg->PhoneTrans = dlgPhone.phone;

	microsipDlg->setooophone(microsipDlg->PhoneTrans);
	microsipDlg->isOOO = microsipDlg->setooo(L"Yes");

	CString log = L"enabled out of office.";
	microsipDlg->setOperationLog(log);


	while (microsipDlg->isOOO){
		//microsipDlg->ShowMessage(L"Yes"), 
	//	microsipDlg->SetTimer(IDT_TIMER_OOO, 1000 * 1, NULL);
		CDialog dlg(IDD_DIALOGOOO, this);
		if (dlg.DoModal() == IDOK){
			microsipDlg->isOOO = microsipDlg->setooo(L"No");
			//if (microsipDlg->isOOO)
			//	microsipDlg->SetTimer(IDT_TIMER_OOO, 1000 * 1, NULL);


			CString log = L"disabled out of office.";
			microsipDlg->setOperationLog(log);




		}




	}



}


void Dialer::OnBnClickedButtondaily()
{
	// TODO: Add your control notification handler code here


	//CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_NUMBER);
	m_acedit.SetWindowText(_T("324"));
	OnBnClickedCall();

	CString log = L"dial 324 for daily listener.";
	microsipDlg->setOperationLog(log);
}


void Dialer::OnBnClickedButtoninotes()
{
	// TODO: Add your control notification handler code here

	
	ShellExecute(NULL, _T("open"), microsipDlg->iNotesUrl,
		NULL, NULL, SW_SHOWNORMAL);

	CString log = L"opened iNotes web page.";
	microsipDlg->setOperationLog(log);
	
}


void Dialer::OnBnClickedButtonooo()
{
	// TODO: Add your control notification handler code here
	OnBnClickedButton3();


}


void Dialer::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here


	//CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_NUMBER);
	m_acedit.SetWindowText(_T("1301"));
	OnBnClickedCall();

	CString log = L"dialed 1301 to connect to door phone.";
	microsipDlg->setOperationLog(log);


}


int Dialer::setKey6ShowOpenDoor()
{
	m_ToolTip.AddTool(&m_ButtonDialer6, _T("Open Door"));
	
	m_ToolTip.Activate(TRUE);
	return 0;
}


int Dialer::clearKeyShowOpenDoor()
{

	m_ToolTip.DelTool(&m_ButtonDialer6);
	m_ToolTip.Activate(TRUE);
	return 0;
}


void Dialer::OnBnClickedBtnunlock()
{
	// TODO: Add your control notification handler code here
	DTMF(L"0");
}


void Dialer::OnBnClickedHidebuttons()
{
	// TODO: Add your control notification handler code here


	microsipDlg->dlgRigh->ShowWindow(SW_HIDE);

	microsipDlg->pageDialer->m_showbuttons.ShowWindow(SW_SHOW);
	microsipDlg->pageDialer->m_hidebuttons.ShowWindow(SW_HIDE);







}


void Dialer::OnBnClickedShowbuttons()
{
	microsipDlg->dlgRigh->ShowWindow(SW_SHOWNORMAL);

	microsipDlg->pageDialer->m_showbuttons.ShowWindow(SW_HIDE);
	microsipDlg->pageDialer->m_hidebuttons.ShowWindow(SW_SHOW);
	// TODO: Add your control notification handler code here

	
}


void Dialer::trimNoneNums(CString& s1, CString & s2)
{
	//CString s1(s);
	s2 = L"";
	int i = 0;
	WCHAR c = s1.GetAt(i);
	while (c){
		if ((c >= '0' && c <= '9') || c=='*' || c=='#')
		{
			s2.AppendChar(c);
		}
		i++;
		c = s1.GetAt(i);
	}


	if (s2.Find(L"2123171423,") != -1){
		s2.Replace(L"2123171423,", L"");
	}
	
	
}


void Dialer::OnCbnEditchangeNumber()
{
	UpdateCallButton();
	// TODO: Add your control notification handler code here
}


void Dialer::OnEnChangeAcedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateCallButton();
}


void Dialer::OnEnUpdateAcedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	UpdateCallButton();
}


void Dialer::OnNMCustomdrawVolumeInput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


int Dialer::getCurrentDateTime()
{

	time_t rawtime;
	struct tm * timeinfo;
	wchar_t bufferDate[40];
	wchar_t bufferTime[40];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//printf("Current local time and date: %s", asctime(timeinfo));
	//printf("Current local time and date: %s", ctime(&rawtime));

	wcsftime(bufferDate, sizeof(bufferDate), L"%m/%d/%Y", timeinfo);
	wcsftime(bufferTime, sizeof(bufferTime), L"%I:%M %p", timeinfo);
	

	//CString strDT(buffer);// = L"";
	//strDT.Format(L"%s", _wasctime(timeinfo));

	GetDlgItem(IDC_TEXT_DATE)->SetWindowText(bufferDate);
	GetDlgItem(IDC_TEXT_TIME)->SetWindowText(bufferTime);
	

	return 0;
}
