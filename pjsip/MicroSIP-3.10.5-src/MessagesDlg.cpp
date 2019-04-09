#include "StdAfx.h"
#include "MessagesDlg.h"
#include "microsip.h"
#include "microsipDlg.h"
#include "settings.h"
#include "Transfer.h"





static CmicrosipDlg *microsipDlg;
extern void Request(int Method, LPCSTR Host, LPCSTR url, LPCSTR header, LPSTR data);

static UINT BASED_CODE indicators[] =
{
	ID_STATUSBAR
};

static DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString sThisWrite;
	sThisWrite.GetBufferSetLength(cb);

	CString *psBuffer = (CString *)dwCookie;

	for (int i=0;i<cb;i++)
	{
		sThisWrite.SetAt(i,*(pbBuff+i));
	}

	*psBuffer += sThisWrite;

	*pcb = sThisWrite.GetLength();
	sThisWrite.ReleaseBuffer();
	return 0;
}

static DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
    CString *psBuffer = (CString*)dwCookie;
 
    if (cb > psBuffer->GetLength()) cb = psBuffer->GetLength();
 
    for (int i = 0; i < cb; i++)
    {
        *(pbBuff + i) = psBuffer->GetAt(i);
    }
 
    *pcb = cb;
    *psBuffer = psBuffer->Mid(cb);
 
    return 0;
}

MessagesDlg::MessagesDlg(CWnd* pParent /*=NULL*/)
: CBaseDialog(MessagesDlg::IDD, pParent)
{
	this->m_hWnd = NULL;
	microsipDlg = (CmicrosipDlg* )pParent;
	Create (IDD, pParent);
}

MessagesDlg::~MessagesDlg(void)
{
}

void MessagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, tabComponent);
	DDX_Control(pDX, IDC_EXPLORER1, m_webBrowser);
	DDX_Control(pDX, IDC_LISTDAILY, m_listDaily);
}

BOOL MessagesDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

#if 0
	AutoMove(IDC_TAB, 0, 0, 100, 0);
	AutoMove(IDC_LAST_CALL, 100, 0, 0, 0);
	AutoMove(IDC_CLOSE_ALL, 100, 0, 0, 0);
	AutoMove(IDC_TRANSFER, 100, 0, 0, 0);
	AutoMove(IDC_HOLD, 100, 0, 0, 0);
	AutoMove(IDC_END, 100, 0, 0, 0);
	AutoMove(IDC_LIST, 0, 0, 100, 80);

	AutoMove(IDC_ATTDTRANSFER, 100, 0, 0, 0);
	AutoMove(IDC_CONFERENCE, 100, 0, 0, 0);
	AutoMove(IDC_TOCELL, 100, 0, 0, 0);
	AutoMove(IDC_MERGE, 100, 0, 0, 0);
#endif // 0


//	GetDlgItem(IDC_MESSAGE)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_LAST_CALL)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_CLOSE_ALL)->ShowWindow(SW_HIDE);

#ifdef _GLOBAL_NO_MESSAGING
	GetDlgItem(IDC_MESSAGE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LAST_CALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CLOSE_ALL)->ShowWindow(SW_HIDE);
#else
	AutoMove(IDC_MESSAGE, 0, 80, 100, 20);
#endif
#if 0
	HICON m_hIconHomePage = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_HOMEPAGE),
		IMAGE_ICON, 0, 0, LR_SHARED);
	((CButton*)GetDlgItem(IDC_HOMEPAGE))->SetIcon(m_hIconHomePage);
#endif // 0


	lastCall = NULL;
	tab = &tabComponent;
	
	HICON m_hIcon = theApp.LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, FALSE);
	
	TranslateDialog(this->m_hWnd);

#ifndef _GLOBAL_VIDEO
	GetDlgItem(IDC_VIDEO_CALL)->ShowWindow(SW_HIDE);
#endif


	CRichEditCtrl* richEdit = (CRichEditCtrl*)GetDlgItem(IDC_MESSAGE);
	richEdit->SetEventMask(richEdit->GetEventMask() | ENM_KEYEVENTS);


	CRichEditCtrl* richEditList = (CRichEditCtrl*)GetDlgItem(IDC_LIST);
    richEditList->SetEventMask(richEdit->GetEventMask() | ENM_MOUSEEVENTS);

	CFont* font = this->GetFont();
	LOGFONT lf;
	font->GetLogFont(&lf);
	lf.lfHeight = 16;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	fontList.CreateFontIndirect(&lf);
	richEditList->SetFont(&fontList);
	lf.lfHeight = 18;
	fontMessage.CreateFontIndirect(&lf);
	richEdit->SetFont(&fontMessage);


	para.cbSize=sizeof(PARAFORMAT2);
	para.dwMask = PFM_STARTINDENT | PFM_LINESPACING | PFM_SPACEBEFORE | PFM_SPACEAFTER;
	para.dxStartIndent=100;
	para.dySpaceBefore=100;
	para.dySpaceAfter=0;
	para.bLineSpacingRule = 5;
	para.dyLineSpacing = 22;
	richEditList->SetParaFormat(para);


	//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);


	CFont SFont;
	SFont.CreateFont(15, 7, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Times New Roman");
	GetDlgItem(IDC_END)->SetFont(&SFont);




	m_bar.Create(this);
//	m_bar.set
	m_bar.SetIndicators(indicators, 1);
//	m_bar.SetPaneInfo(0, ID_STATUSBAR, SBPS_NORMAL, PAGE_WIDTH*1.2);
	m_bar.SetPaneInfo(0, ID_STATUSBAR, SBPS_STRETCH, 0);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_STATUSBAR);

//	m_bar.SetPaneText(0, _T(""));
//	m_bar.SetPaneText(1, _T(_GLOBAL_VERSION));

	//m_webBrowser.Navigate(URL, NULL, NULL, NULL, NULL);
	menu.CreatePopupMenu();     //动态创建弹出式菜单对象   
	menu.AppendMenu(MF_STRING, IDC_LISTITEM_COPY, _T("Copy")); // 第一项菜单项 

	m_webBrowser.Stop();
	m_webBrowser.Navigate(URL, NULL, NULL, NULL, NULL);

	return TRUE;
}


void MessagesDlg::PostNcDestroy()
{
	CBaseDialog::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(MessagesDlg, CBaseDialog)
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_CLOSEALLTABS,OnCloseAllTabs)
	ON_COMMAND(IDC_LISTITEM_COPY, CopyListItem)
//	ON_COMMAND(ID_GOTOLASTTAB,OnGoToLastTab)
//	ON_COMMAND(ID_COPY,OnCopy)
//	ON_COMMAND(ID_SELECT_ALL,OnSelectAll)
//	ON_BN_CLICKED(IDCANCEL, &MessagesDlg::OnBnClickedCancel)
//	ON_BN_CLICKED(IDOK, &MessagesDlg::OnBnClickedOk)
//	ON_NOTIFY(EN_MSGFILTER, IDC_MESSAGE, &MessagesDlg::OnEnMsgfilterMessage)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &MessagesDlg::OnTcnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, &MessagesDlg::OnTcnSelchangingTab)
//	ON_MESSAGE(WM_CONTEXTMENU,OnContextMenu)
//	ON_MESSAGE(UM_CLOSETAB, &MessagesDlg::OnCloseTab)
	ON_BN_CLICKED(IDC_CALL_END, &MessagesDlg::OnBnClickedCallEnd)
	ON_BN_CLICKED(IDC_VIDEO_CALL, &MessagesDlg::OnBnClickedVideoCall)
	ON_BN_CLICKED(IDC_HOLD, &MessagesDlg::OnBnClickedHold)
	ON_BN_CLICKED(IDC_TRANSFER, &MessagesDlg::OnBnClickedTransfer)
	ON_BN_CLICKED(IDC_ATTDTRANSFER, &MessagesDlg::OnBnClickedAttdTransfer)
	ON_BN_CLICKED(IDC_CONFERENCE, &MessagesDlg::OnBnClickedConference)
	ON_BN_CLICKED(IDC_TOCELL, &MessagesDlg::OnBnClickedToCell)
	ON_BN_CLICKED(IDC_MERGE, &MessagesDlg::OnBnClickedMerge)
	ON_BN_CLICKED(IDC_END, &MessagesDlg::OnBnClickedEnd)
	ON_BN_CLICKED(IDC_CLOSE_ALL, &MessagesDlg::OnBnClickedCloseAll)
//	ON_BN_CLICKED(IDC_LAST_CALL, &MessagesDlg::OnBnClickedLastCall)
//	ON_STN_CLICKED(IDC_LOGO, &MessagesDlg::OnStnClickedLogo)
	ON_BN_CLICKED(IDC_TABCLOSE, &MessagesDlg::OnBnClickedTabclose)
	ON_COMMAND(ID__CLOSE, &MessagesDlg::OnTabClose)
	//ON_COMMAND(ID__UPDATE, &MessagesDlg::onUpdateNewVersion)
//	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_OPENDOOR, &MessagesDlg::OnBnClickedOpendoor)
	ON_WM_TIMER()
	ON_MESSAGE(MYWM_ADD_MESSAGE, &MessagesDlg::OnMywmAddMessage)
END_MESSAGE_MAP()

LRESULT MessagesDlg::OnContextMenu(WPARAM wParam,LPARAM lParam)
{
	int x = GET_X_LPARAM(lParam); 
	int y = GET_Y_LPARAM(lParam); 
	POINT pt = { x, y };
	RECT rc;
	if (x!=-1 || y!=-1) {
		ScreenToClient(&pt);
		GetClientRect(&rc); 
		if (!PtInRect(&rc, pt)) {
			x = y = -1;
		} 
	} else {
		::ClientToScreen((HWND)wParam, &pt);
		x = 10+pt.x;
		y = 10+pt.y;
	}
	if (x!=-1 || y!=-1) {
			CMenu menu;
			menu.LoadMenu(IDR_MENU_TABS);
			CMenu* tracker = menu.GetSubMenu(0);
			TranslateMenu(tracker->m_hMenu);
			tracker->TrackPopupMenu( 0, x, y, this );
			return TRUE;
	}
	return DefWindowProc(WM_CONTEXTMENU,wParam,lParam);
}

void MessagesDlg::OnClose() 
{
	if (tab->GetItemCount() > 0)
	{
		int n = ::MessageBox(m_hWnd, _T("Are you sure to close all tabs and end all calls?"), _T("Notice"), MB_OKCANCEL);

		if (n != IDOK) return;
	}


	call_hangup_all_noincoming();
	OnCloseAllTabs();
	
//	microsipDlg->dlgTop->CloseWindow();

	microsipDlg->dlgTop->ShowWindow(SW_HIDE);

//	microsipDlg->dlgBot->CloseWindow();
	microsipDlg->dlgBot->ShowWindow(SW_HIDE);

//	microsipDlg->dlgRigh->CloseWindow();
	microsipDlg->dlgRigh->ShowWindow(SW_HIDE);

	microsipDlg->dlgRigh->m_callSuper.ShowWindow(SW_HIDE);

	this->ShowWindow(SW_HIDE);
	//this->CloseWindow();
	microsipDlg->isMessgeShown = false;
	
}

void MessagesDlg::OnMove(int x, int y)
{
	CRect cRectMessage;

	GetWindowRect(&cRectMessage);
	int Mwidth = cRectMessage.Width();

	CRect cRect;
	microsipDlg->GetWindowRect(&cRect);
	int MainWidth = cRect.Width();
	int MainHeight = cRect.Height();

	cRect.top = cRectMessage.top;
	cRect.left = cRectMessage.left + Mwidth;

	cRect.bottom = cRect.top + MainHeight;
	cRect.right = cRect.left + MainWidth;

//	cRectMessage.top = cRect.top;
 //   cRectMessage.bottom = cRect.bottom;
//	cRectMessage.left = cRect.left - Mwidth;
//	cRectMessage.right = cRectMessage.left + Mwidth;

	if (microsipDlg->IsWindowVisible() && !microsipDlg->IsZoomed() && !microsipDlg->IsIconic())
	{
		//microsipDlg->ShowMessage(L"moved from message!");
		if (firstTimeMove)
		{
			CRect cRectFirst;

			CRect primaryScreenRect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryScreenRect, 0);
			cRectFirst.left = primaryScreenRect.Width() / 2 - cRect.Width()/2;
			cRectFirst.top = primaryScreenRect.Height() / 2 - cRect.Height() / 2;


			firstTimeMove = false;
			microsipDlg->MoveWindow(&cRectFirst);
		}else
		microsipDlg->MoveWindow(&cRect);
	}
	//MoveWindow(&cRectMessage);



	
	
	if (IsWindowVisible() && !IsZoomed() && !IsIconic()) {
		CRect cRect;
		GetWindowRect(&cRect);
		accountSettings.messagesX = cRect.left;
		accountSettings.messagesY = cRect.top;
		microsipDlg->AccountSettingsPendingSave();
	}
}

void MessagesDlg::OnSize(UINT type, int w, int h)
{
	CBaseDialog::OnSize(type, w, h);
	if (this->IsWindowVisible() && type == SIZE_RESTORED) {
		CRect cRect;
		GetWindowRect(&cRect);
		accountSettings.messagesW = cRect.Width();
		accountSettings.messagesH = cRect.Height();
		microsipDlg->AccountSettingsPendingSave();
	}
}

void MessagesDlg::OnBnClickedCancel()
{
	OnClose();
}

void MessagesDlg::OnBnClickedOk()
{
}

MessagesContact* MessagesDlg::AddTab(CString number, CString name, BOOL activate, pjsua_call_info *call_info, BOOL notShowWindow, BOOL ifExists)
{
	MessagesContact* messagesContact;

	SIPURI sipuri;
	ParseSIPURI(number, &sipuri);
	if (!accountSettings.account.domain.IsEmpty() && RemovePort(accountSettings.account.domain) == RemovePort(sipuri.domain) ) {
		sipuri.domain = accountSettings.account.domain;
	}

	number = (sipuri.user.GetLength() ? sipuri.user + _T("@") : _T("")) + sipuri.domain;
	CString disnumber = (sipuri.user.GetLength() ? sipuri.user : _T("") + sipuri.domain);
	CString disname = name;
//	microsipDlg->ShowMessage(disname);
	LONG exists = -1;
	for (int i=0; i < tab->GetItemCount(); i++)
	{
		messagesContact = GetMessageContact(i);

		CString compareNumber = messagesContact->number;
#ifdef _GLOBAL_NUMBER_PREFIX
		compareNumber = _T(_GLOBAL_NUMBER_PREFIX) + compareNumber;
#endif
		if (messagesContact->number == number || compareNumber == number) {
			exists=i;
			if (call_info)
			{
				if (messagesContact->callId != -1) {
					if (messagesContact->callId != call_info->id) {
						if (call_info->state != PJSIP_INV_STATE_DISCONNECTED) {
							microsipDlg->PostMessage(MYWM_CALL_ANSWER, (WPARAM)call_info->id, -486);
						}
						return NULL;
					}
				} else {
					messagesContact->callId = call_info->id;
				}
			}
			break;
		}
	}
	if (exists==-1)
	{
		if (ifExists)
		{
			return NULL;
		}
#ifndef _GLOBAL_NO_CONTACTS
		if (!name.GetLength()) {
			name = microsipDlg->pageContacts->GetNameByNumber(number);
			disname = name;
				//microsipDlg->ShowMessage(L"0");
			//	microsipDlg->ShowMessage(disname);
#endif
			if (!name.GetLength()) {
				if (!sipuri.name.GetLength())
				{
					name = (sipuri.domain == accountSettings.account.domain ? sipuri.user : number);
					disname = (sipuri.domain == accountSettings.account.domain ? sipuri.user : disnumber);
				//	microsipDlg->ShowMessage(L"1");
				//	microsipDlg->ShowMessage(disname);

				} else 
				{
					name = sipuri.name + _T(" (") + (sipuri.domain == accountSettings.account.domain ? sipuri.user : number) + _T(")");
					disname = sipuri.name + _T(" <") + (sipuri.domain == accountSettings.account.domain ? sipuri.user : disnumber) + _T(">");
					//microsipDlg->ShowMessage(L"2");
					//microsipDlg->ShowMessage(disname);
				}
			}
#ifndef _GLOBAL_NO_CONTACTS
		}


		int p = name.Find(L"@");
		if (p != -1){
			name = name.Mid(0, p);
		}
		name.Replace(L"(", L"");
#endif
		messagesContact = new MessagesContact();
		messagesContact->callId = call_info ? call_info->id : -1;
		messagesContact->number = number;
		messagesContact->name = name;

		//microsipDlg->ShowMessage(L"3");
		//microsipDlg->ShowMessage(name);

		if (name.Find(L"CLICKTO") != -1)
		{
			CString contactName = name;
			contactName.Replace(L"CLICKTO ", L"");
			CString contacttype; 
		//	microsipDlg->ShowMessage(contactName);
		//	microsipDlg->ShowMessage(contactName);
			int p = contactName.Find(L"#");
			if (p != -1){
				
				contacttype = contactName.Mid(p + 1);

				contactName = contactName.Mid(0, p);

				int p2 = contacttype.Find(L" ");
				if (p2 != -1)
					contacttype = contacttype.Mid(0, p2);

			}
		//	microsipDlg->ShowMessage(contactName);
		//	microsipDlg->ShowMessage(contacttype);

			if (contactName.IsEmpty() || contactName == L"")
				contactName = number;
			/*
			contacttype = name.Mid(p + 1);
			int p2 = contacttype.Find(L"@");
			if (p2 != -1)
				contacttype = contacttype.Mid(0, p2);
				*/
			messagesContact->name = contactName;

			if (!contactName.IsEmpty()){
				//microsipDlg->ShowMessage(number + name);
				microsipDlg->pageContacts->ContactAdd(number, contactName, contacttype, 0, -1, TRUE);


			}



		}


	//	microsipDlg->ShowMessage(disname);
		TCITEM item;
		item.mask = TCIF_PARAM | TCIF_TEXT;
		name.Format(_T("   %s  "), name);
		disname.Format(_T("   %s  "), disname);
		/*name.Replace(_T("("), _T(" "));
		name.Replace(_T(")"), _T(""));*/
		
		//CString disname = name.Left(name.Find(_T("@")));
	
	//	microsipDlg->ShowMessage(disname);

		item.pszText = disname.GetBuffer();
		disname.ReleaseBuffer();
		item.cchTextMax=0;
		item.lParam = (LPARAM)messagesContact;
		exists = tab->InsertItem(tab->GetItemCount(), &item); 
		tab->SetCurSel(exists);
		if (tab->GetCurSel() == exists)
		{
			OnChangeTab(call_info);
		}
	} else
	{
		
		if (!call_info || !(call_info->media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD))
		{
			tab->SetCurSel(exists);
			if (tab->GetCurSel() != -1)
				OnChangeTab();
		}
	//	ShowCallInformation(number);
	//	OnChangeTab(call_info);
		if (tab->GetCurSel() == exists && call_info)
		{
			UpdateCallButton(messagesContact->callId != -1, call_info);
		}
	}
	//if (tab->GetCurSel() != exists && (activate || !IsWindowVisible()))
	if (tab->GetCurSel() != exists && activate)
	{
		long result;
		OnTcnSelchangingTab(NULL, &result);
		tab->SetCurSel(exists);
		OnChangeTab(call_info);
	}
	if (!IsWindowVisible() || IsIconic()) {
		if (!notShowWindow) 
		{
			if (!accountSettings.hidden) {
				ShowWindow(SW_SHOWNORMAL);
				CRichEditCtrl* richEdit = (CRichEditCtrl*)GetDlgItem(IDC_MESSAGE);
				GotoDlgCtrl(richEdit);
			}
		}
	}
	/*
	if(IsWindowVisible() || !IsZoomed() || !IsIconic()){
		microsipDlg->pageDialer->m_showCall.ShowWindow(SW_HIDE);
		microsipDlg->pageDialer->m_hideCall.ShowWindow(SW_SHOW);
	}
	else
	{
		microsipDlg->pageDialer->m_showCall.ShowWindow(SW_SHOW);
		microsipDlg->pageDialer->m_hideCall.ShowWindow(SW_HIDE);

	}
	*/
	//microsipDlg->ShowMessage(L"4");
	//microsipDlg->ShowMessage(messagesContact->name);
	return messagesContact;
}

void MessagesDlg::OnChangeTab(pjsua_call_info *p_call_info)
{
	//tab->HighlightItem(tab->GetCurSel(), FALSE);

	MessagesContact* messagesContact = GetMessageContact();
	SetWindowText(messagesContact->name);

	


	if (messagesContact->callId != -1) {
		pjsua_call_info call_info;
		if (!p_call_info) {
			pjsua_call_get_info(messagesContact->callId, &call_info);
			p_call_info = &call_info;
		}

		
	//	CString msg;
	//	msg.Format(L"%d", messagesContact->callId);
	//	microsipDlg->ShowMessage(msg);
	
		UpdateTabItem(TRUE);

		UpdateCallButton(TRUE, p_call_info);
		if (accountSettings.singleMode
			&&(p_call_info->role==PJSIP_ROLE_UAC ||
				(p_call_info->role==PJSIP_ROLE_UAS &&
				(p_call_info->state == PJSIP_INV_STATE_CONFIRMED
				|| p_call_info->state == PJSIP_INV_STATE_CONNECTING)
				))
			) {
			SIPURI sipuri;
			ParseSIPURI(messagesContact->number, &sipuri);
#ifndef _GLOBAL_ACCOUNT_MINI
			microsipDlg->pageDialer->SetNumber(!sipuri.user.IsEmpty() && sipuri.domain == accountSettings.account.domain ? sipuri.user : messagesContact->number);
#else
			microsipDlg->pageDialer->SetNumber(!sipuri.user.IsEmpty() ? sipuri.user : messagesContact->number);
#endif
		}
	} else {
		UpdateTabItem();
		UpdateCallButton();
		if (accountSettings.singleMode) {
			microsipDlg->pageDialer->Clear();
		}
	}

	CRichEditCtrl* richEditList = (CRichEditCtrl*)GetDlgItem(IDC_LIST);
	CRichEditCtrl* richEdit = (CRichEditCtrl*)GetDlgItem(IDC_MESSAGE);

	CString messages = messagesContact->messages;
	EDITSTREAM es;
	es.dwCookie = (DWORD) &messages;
	es.pfnCallback = MEditStreamInCallback; 
	richEditList->StreamIn(SF_RTF, es);

	richEditList->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	richEdit->SetWindowText(messagesContact->message);

	int nEnd = richEdit->GetTextLengthEx(GTL_NUMCHARS);
	richEdit->SetSel(nEnd, nEnd);

	m_bar.SetPaneText(0, messagesContact->lastSystemMessage);

	CString type = L"incoming";
	try{
	pjsua_call_info call_info;
	if (!p_call_info) {
		pjsua_call_get_info(messagesContact->callId, &call_info);
		p_call_info = &call_info;
	}

	if (p_call_info->role == PJSIP_ROLE_UAC)
		type = L"outgoing";


	//CString msg;
	//msg.Format(L"%d", microsipDlg->headinfo.count());

	//microsipDlg->ShowMessage(microsipDlg->headinfo.count);
	
		std::hash_map<pjsua_call_id, CString> ::const_iterator finder;

		finder = microsipDlg->headinfo.find(messagesContact->callId);

		if (finder == microsipDlg->headinfo.end())
		{
			ShowCallInformation(messagesContact->number, type);


		}
		else
		{
			type = L"phonebank";
			ShowCallInformation(messagesContact->number, type, finder->second);

		}
	}
	catch (...)
	{

	}


	//***************************************
	if (messagesContact->number.Find(L"301") != -1 || messagesContact->number.Find(L"1301") != -1){
        
		microsipDlg->pageDialer->setKey6ShowOpenDoor();

	}
	else
	{
		microsipDlg->pageDialer->clearKeyShowOpenDoor();
	}

	if (messagesContact->name == L"324"){
	//	m_webBrowser.ShowWindow(SW_HIDE);
		m_listDaily.ShowWindow(SW_SHOWNORMAL);

		HWND hwnd = m_webBrowser.GetSafeHwnd();
		::ShowWindow(hwnd, SW_HIDE);



	}
	else
	{
	//	m_webBrowser.ShowWindow(SW_SHOWNORMAL);
		m_listDaily.ShowWindow(SW_HIDE);

		HWND hwnd = m_webBrowser.GetSafeHwnd();
		::ShowWindow(hwnd, SW_SHOW);

	}




	//******************************************

		
}

void MessagesDlg::UpdateTabItem(BOOL active)
{
   
	
	int i = tab->GetCurSel();
	if (i != -1)
	{
		TC_ITEM item;
		CString TabTile;
		item.mask = TCIF_PARAM | TCIF_TEXT;
		item.pszText = TabTile.GetBuffer(256);
		item.cchTextMax = 256;
		tab->GetItem(i, &item);
		TabTile.ReleaseBuffer();
#if 1
	  //  TabTile = item.pszText;

		if (TabTile.Find(_T(ACTIVETALK)) != -1)
		{

			if (!active)
				TabTile.Replace(_T(ACTIVETALK), _T(""));


		}
		else
		{

			if (active)
			{
                
				TabTile.Insert(0, _T(ACTIVETALK));

			}


		}
		//item.pszText = TabTile.GetBuffer();
		//	ti.pszText = lpszTitle; // New text 
		tab->SetItem(i, &item);
#endif
		
	}

}


void MessagesDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnChangeTab();
	*pResult = 0;
}


void MessagesDlg::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CRichEditCtrl* richEdit = (CRichEditCtrl*)GetDlgItem(IDC_MESSAGE);
	CString str;
	int len = richEdit->GetWindowTextLength();
	LPTSTR ptr = str.GetBuffer(len);
	richEdit->GetWindowText(ptr,len+1);
	str.ReleaseBuffer();

	MessagesContact* messagesContact = GetMessageContact();
	messagesContact->message = str;
	*pResult = 0;
}

LRESULT  MessagesDlg::OnCloseTab(WPARAM wParam,LPARAM lParam)
{
	int i=wParam;
	CloseTab(i);
	return TRUE;
}

BOOL MessagesDlg::CloseTab(int i, BOOL safe)
{
	int curSel = tab->GetCurSel();

	MessagesContact* messagesContact = GetMessageContact(i);
	if (messagesContact->callId != -1)
	{
		if (safe) {
			return FALSE;
		}
		call_hangup_fast(messagesContact->callId);
	}
	tab->DeleteItem(i);
	delete messagesContact;
	
	int count = tab->GetItemCount();
	if (!count) {
		GetDlgItem(IDC_MESSAGE)->SetWindowText(NULL);
		GetDlgItem(IDC_LIST)->SetWindowText(NULL);
		GetDlgItem(IDC_LISTDAILY)->SetWindowText(NULL);
		
		microsipDlg->pageDialer->m_showCall.ShowWindow(SW_HIDE);
		microsipDlg->pageDialer->m_hideCall.ShowWindow(SW_HIDE);

		microsipDlg->pageDialer->clearKeyShowOpenDoor();
		OnClose();

	} else  {
		tab->SetCurSel( curSel < count ? curSel: count-1 );
		OnChangeTab();
	}
	return TRUE;
}

pjsua_call_id MessagesDlg::CallMake(CString number, bool hasVideo, pj_status_t *pStatus)
{
	if (accountSettings.singleMode) {
		call_hangup_all_noincoming();
	} else {
		call_hold_all_except();
	}
#ifdef _GLOBAL_VIDEO
	if (hasVideo) {
		microsipDlg->createPreviewWin();
	}
#endif

	microsipDlg->SetSoundDevice(microsipDlg->audio_output);

	pjsua_acc_id acc_id;
	pj_str_t pj_uri;
	SelectSIPAccount(number,acc_id,pj_uri);

	pjsua_call_id call_id;
	
	pjsua_call_setting call_setting;
	pjsua_call_setting_default(&call_setting);
	call_setting.flag = 0;
	call_setting.vid_cnt=hasVideo ? 1 : 0;

	pj_status_t status = pjsua_call_make_call(
		acc_id,
		&pj_uri,
		&call_setting,
		NULL,
		NULL,
		&call_id);
	if (pStatus) {
		*pStatus = status;
	}
//	ShowCallInformation(number);
//	microsipDlg->ShowCallInformation(number);

	return status == PJ_SUCCESS ? call_id : PJSUA_INVALID_ID;
}

void MessagesDlg::CallStart(bool hasVideo)
{
	MessagesContact* messagesContact = GetMessageContact();

	CString number = messagesContact->number;

	int pos = number.Find(_T("@"));

	if (pos != -1)
		number = number.Left(pos);

	if (number == accountSettings.account.username)
	{
		::MessageBox(m_hWnd, _T("You can not call yourself"), _T("Warnning"), MB_OK);
		return;
	}


	pj_status_t status;
	pjsua_call_id call_id = CallMake(messagesContact->number,hasVideo, &status);
	if (call_id!=PJSUA_INVALID_ID) {
		messagesContact->callId = call_id;
		UpdateCallButton(TRUE);
	} else {
		if (status != PJ_ERESOLVE) {
			CString message = GetErrorMessage(status);
			AddMessage(messagesContact,message);
			if (accountSettings.singleMode) {
				AfxMessageBox(message);
			}
		}
	}
}

void MessagesDlg::OnBnClickedCallEnd()
{
	MessagesContact* messagesContact = GetMessageContact();
	if (messagesContact->callId == -1)
	{
		CallStart();
	}
}

void MessagesDlg::OnEndCall(pjsua_call_info *call_info)
{
	for (int i = 0; i < tab->GetItemCount(); i++)
	{
		MessagesContact* messagesContact = GetMessageContact(i);
		if (messagesContact->callId == call_info->id)
		{
			lastCall = messagesContact;
			messagesContact->callId = -1;
			if (tab->GetCurSel()==i)
			{
				UpdateTabItem();
				UpdateCallButton(FALSE, call_info);
				//CloseTab(i);
			}

			if (messagesContact->name == L"324"){

				microsipDlg->dlgTop->ShowWindow(SW_HIDE);
				microsipDlg->dlgBot->ShowWindow(SW_HIDE);
				microsipDlg->dlgRigh->ShowWindow(SW_HIDE);
			}

			microsipDlg->dlgBot->ShowWindow(SW_HIDE);

			break;
		}
	}
}

void MessagesDlg::UpdateCallButton(BOOL active, pjsua_call_info *call_info)
{
	GetDlgItem(IDC_CALL_END)->ShowWindow(active? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_END)->ShowWindow(!active? SW_HIDE : SW_SHOW);
#ifdef _GLOBAL_VIDEO
	GetDlgItem(IDC_VIDEO_CALL)->ShowWindow(active? SW_HIDE : SW_SHOW);
#endif
	UpdateHoldButton(call_info);

	if (!active)
	{
		KillTimer(IDT_TIMER_DAILY);
		OnDailyDuration(true);
	}
}

void MessagesDlg::UpdateHoldButton(pjsua_call_info *call_info)
{
#ifndef _GLOBAL_NO_HOLD
	MessagesContact* messagesContact = GetMessageContact();
	if (messagesContact) {
		CButton* button = (CButton*)GetDlgItem(IDC_HOLD);
		CButton* buttonTransfer = (CButton*)GetDlgItem(IDC_TRANSFER);
		CButton* attdbuttonTransfer = (CButton*)GetDlgItem(IDC_ATTDTRANSFER);
		CButton* confbuttonTransfer = (CButton*)GetDlgItem(IDC_CONFERENCE);
		
		CButton* buttonDoorOpen = (CButton*)GetDlgItem(IDC_OPENDOOR);


		CButton* confbuttonTocell = (CButton*)GetDlgItem(IDC_TOCELL);
		CButton* confbuttonMerge = (CButton*)GetDlgItem(IDC_MERGE);
		
#ifndef _GLOBAL_TWO_BUTTONS
		CButton* buttonDialer = (CButton*)microsipDlg->pageDialer->GetDlgItem(IDC_HOLD);
		CButton* buttonTransferDialer = (CButton*)microsipDlg->pageDialer->GetDlgItem(IDC_TRANSFER);
#endif
		if (messagesContact->callId != -1) {
			if (call_info && messagesContact->callId == call_info->id && call_info->media_cnt>0) {
				buttonTransfer->ShowWindow(SW_SHOW);
				attdbuttonTransfer->ShowWindow(SW_SHOW);
				confbuttonTransfer->ShowWindow(SW_SHOW);
			
				if (messagesContact->number.Find(L"301") != -1 || messagesContact->number.Find(L"1301") != -1)
				buttonDoorOpen->ShowWindow(SW_SHOW);

				confbuttonTocell->ShowWindow(SW_SHOW);
				confbuttonMerge->ShowWindow(SW_SHOW);
#ifndef _GLOBAL_TWO_BUTTONS
				buttonTransferDialer->EnableWindow(TRUE);
#endif
				if (call_info->media_status == PJSUA_CALL_MEDIA_ACTIVE
					|| call_info->media_status == PJSUA_CALL_MEDIA_REMOTE_HOLD
					) {
						button->ShowWindow(SW_SHOW);
						button->SetCheck(BST_UNCHECKED);
#ifndef _GLOBAL_TWO_BUTTONS
						buttonDialer->EnableWindow(TRUE);
						buttonDialer->SetCheck(BST_UNCHECKED);
#endif
						return;
				} else if (call_info->media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD
					|| call_info->media_status == PJSUA_CALL_MEDIA_NONE) {
						button->ShowWindow(SW_SHOW);
						button->SetCheck(BST_CHECKED);
#ifndef _GLOBAL_TWO_BUTTONS
						buttonDialer->EnableWindow(TRUE);
						buttonDialer->SetCheck(BST_CHECKED);
#endif
						return;
				}
			}
		} else {
			button->ShowWindow(SW_HIDE);
			button->SetCheck(BST_UNCHECKED);
#ifndef _GLOBAL_TWO_BUTTONS
			buttonDialer->EnableWindow(FALSE);
			buttonDialer->SetCheck(BST_UNCHECKED);
			buttonTransfer->ShowWindow(SW_HIDE);
			attdbuttonTransfer->ShowWindow(SW_HIDE);
			confbuttonTransfer->ShowWindow(SW_HIDE);
			buttonDoorOpen->ShowWindow(SW_HIDE);
			confbuttonTocell->ShowWindow(SW_HIDE);
			confbuttonMerge->ShowWindow(SW_HIDE);
			buttonTransferDialer->EnableWindow(FALSE);
#endif
		}
	}
#endif
}

void MessagesDlg::Call(BOOL hasVideo)
{
	if (!accountSettings.singleMode || !call_get_count_noincoming())
	{
		MessagesContact* messagesContact = GetMessageContact();
		if (messagesContact->callId == -1)
		{
			CallStart(hasVideo);
		}
	} else {
		microsipDlg->GotoTab(0);
	}
}

/*
void MessagesDlg::ShowCallInformation(CString number)
{
	number = number.Left(number.Find(_T("@")));

	CString url = L"https://www.heightsre.com/85256DDC00562372/metaphone?openform&phone=";
	url += number;
	//microsipDlg->DialogWeb->m_web
	

	microsipDlg->DialogWeb->m_web.Navigate(url, NULL, NULL, NULL, NULL);
	microsipDlg->DialogWeb->ShowWindow(SW_NORMAL);



}*/

void MessagesDlg::ShowDailyInformation(CString message)
{

	/*
	if (!checkIfTotalInformtion(message))
	{
		if (checkIfDailyInfo(message))
		{
			icurrDaily++;
			scurrDaily.Format(L"%d", icurrDaily);
		}

	}

	ShowDailyNumInfo();
	*/
	if (checkIfFunction(message)) return;

	//if (checkIfFunction(message)) return;

	m_listDaily.ResetContent();
	

	int p = message.Find(L"&&");
	while (p != -1)
	{
		CString msg = message.Mid(0, p);
		
		if (!chekIfWebLink(msg))// && !checkIfFunction(msg))
		   m_listDaily.AddString(msg);

		

		message = message.Mid(p + 2);
		p = message.Find(L"&&");


	}


	if (!chekIfWebLink(message) )
	    m_listDaily.AddString(message);
	

	


}


bool MessagesDlg::checkIfFunction(CString msg)
{


	int p = msg.Find(L"FUNCTION:");

	if (p != -1)
	{

		CString varValue = msg.Mid(p + 9);

		int pv = varValue.Find(L":");

		if (pv != -1)
		{
			CString var = varValue.Mid(0, pv);
			CString value = varValue.Mid(pv + 1);

			if (var == L"ALLNUM")
				totalDaily = value;

			if (var == L"CURNUM")
			{
				scurrDaily = value;
				ShowDailyNumInfo();

			}

			if (var == L"START")
			{
			//	KillTimer(IDT_TIMER_DAILY);
			//	OnDailyDuration(true);
				dailyTimer = SetTimer(IDT_TIMER_DAILY, 1000, NULL);

			}

			if (var == L"STOP")
			{
				KillTimer(IDT_TIMER_DAILY);

				if (value == L"DONE")
				{
					OnDailyDuration(true);

					microsipDlg->dlgTop->ShowWindow(SW_HIDE);
					microsipDlg->dlgBot->ShowWindow(SW_HIDE);
					//microsipDlg->dlgRigh->ShowWindow(SW_HIDE);
				}

			}

			if (var == L"WEBBOT")
			{
				
				microsipDlg->dlgBot->m_expbot.Navigate(value, NULL, NULL, NULL, NULL);
				microsipDlg->dlgBot->ShowWindow(SW_SHOW);
				

			}
			boolean showRight = false;
		//	microsipDlg->dlgRigh->ShowWindow(SW_HIDE);
		/*	microsipDlg->dlgRigh->m_sendLease.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_complaint.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_moveout.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_rent.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_image.ShowWindow(SW_HIDE);

			microsipDlg->dlgRigh->m_callrenew.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_callrent.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_callrentstate.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_3DN.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->m_coi.ShowWindow(SW_HIDE);

			microsipDlg->dlgRigh->custDocid = L"";

			microsipDlg->dlgRigh->m_callSuper.ShowWindow(SW_HIDE);
			microsipDlg->dlgRigh->phoneNum = L"";

			microsipDlg->dlgRigh->orgDocid = L"";
			microsipDlg->dlgRigh->m_prospect.ShowWindow(SW_HIDE);

			microsipDlg->dlgRigh->m_textsuper.ShowWindow(SW_HIDE);
			//	microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_HIDE);
			microsipDlg->dlgText->superDocid = L"";*/


			if (var == L"SUPEREXT")
			{

				//microsipDlg->dlgBot->m_expbot.Navigate(value, NULL, NULL, NULL, NULL);
				//microsipDlg->dlgBot->ShowWindow(SW_SHOW);
			//	microsipDlg->ShowMessage(value);

				if (value != L"none"){


					microsipDlg->dlgRigh->m_callSuper.ShowWindow(SW_SHOWNORMAL);

					microsipDlg->dlgRigh -> phoneNum = value;
					showRight = true;

				}
				else
				{
					microsipDlg->dlgRigh->m_callSuper.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->phoneNum = L"";
				}


			}


			if (var == L"CUSTDOCID")
			{

				//microsipDlg->dlgBot->m_expbot.Navigate(value, NULL, NULL, NULL, NULL);
				//microsipDlg->dlgBot->ShowWindow(SW_SHOW);
				//	microsipDlg->ShowMessage(value);

				if (value != L"none"){


					microsipDlg->dlgRigh->m_sendLease.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_complaint.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_moveout.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_rent.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_image.ShowWindow(SW_SHOWNORMAL);

					microsipDlg->dlgRigh->m_callrenew.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_callrent.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_callrentstate.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_3DN.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_coi.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_charge.ShowWindow(SW_SHOWNORMAL);
					microsipDlg->dlgRigh->m_prospect.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->custDocid = value;

					showRight = true;
				//	microsipDlg->dlgRigh->ShowWindow(SW_SHOWNORMAL);

				}
				else
				{
					microsipDlg->dlgRigh->m_sendLease.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_complaint.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_moveout.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_rent.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_image.ShowWindow(SW_HIDE);

					microsipDlg->dlgRigh->m_callrenew.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_callrent.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_callrentstate.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_3DN.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_coi.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->m_charge.ShowWindow(SW_HIDE);

					microsipDlg->dlgRigh->custDocid = L"";
					//microsipDlg->dlgRigh->ShowWindow(SW_HIDE);
				}
				

			}


			if (var == L"ORGDOCID")
			{

				//microsipDlg->dlgBot->m_expbot.Navigate(value, NULL, NULL, NULL, NULL);
				//microsipDlg->dlgBot->ShowWindow(SW_SHOW);
				//	microsipDlg->ShowMessage(value);

				if (value != L"none"){


				//	microsipDlg->dlgRigh->m_textsuper.ShowWindow(SW_SHOWNORMAL);
					//	microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_SHOWNORMAL);

					//microsipDlg->dlgRigh->superDocid = value;
					microsipDlg->dlgRigh->orgDocid = value;
					microsipDlg->dlgRigh->m_prospect.ShowWindow(SW_SHOWNORMAL);
					showRight = true;

				}
				else
				{
				//	microsipDlg->dlgRigh->m_textsuper.ShowWindow(SW_HIDE);
					//	microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_HIDE);
					microsipDlg->dlgRigh->orgDocid = L"";
					microsipDlg->dlgRigh->m_prospect.ShowWindow(SW_HIDE);
				}
				

			}

			

			if (var == L"SUPERDOCID")
			{

				//microsipDlg->dlgBot->m_expbot.Navigate(value, NULL, NULL, NULL, NULL);
				//microsipDlg->dlgBot->ShowWindow(SW_SHOW);
				//	microsipDlg->ShowMessage(value);

				if (value != L"none"){


					microsipDlg->dlgRigh->m_textsuper.ShowWindow(SW_SHOWNORMAL);
				//	microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_SHOWNORMAL);

					//microsipDlg->dlgRigh->superDocid = value;
					microsipDlg->dlgText->superDocid = value;
					showRight = true;

				}
				else
				{
					microsipDlg->dlgRigh->m_textsuper.ShowWindow(SW_HIDE);
				//	microsipDlg->dlgRigh->m_ebill.ShowWindow(SW_HIDE);
					microsipDlg->dlgText->superDocid = L"";
				}


			}

			if (showRight) 
				microsipDlg->dlgRigh->ShowWindow(SW_SHOWNORMAL);

		}
		

		



		return true;

	}

	return false;

}


void MessagesDlg::ShowDailyNumInfo()
{
	SetDlgItemText(IDC_STATIC_NUM, scurrDaily + L"/" + totalDaily);



}

bool MessagesDlg::checkIfDailyInfo(CString msg)
{

	int p = msg.Find(L"&&");

	if (p != -1)
	{
		

		return true;

	}

	return false;



}


bool MessagesDlg::checkIfTotalInformtion(CString msg)
{

	int p = msg.Find(L" unheard daily reports");

	if (p != -1)
	{
		int p1 = msg.Find(L"You have all ");
		CString total = msg.Mid(p1 + 9+ 4, p-p1-9-4);
		totalDaily = total;

		return true;

	}

	return false;



}


bool MessagesDlg::chekIfWebLink(CString msg)
{
	int iWeb = 0;

	int p = msg.Find(L"WEB LINK: ");

	if (p != -1)
	{
		iWeb = 1;
	}
	else
	{
		p = msg.Find(L"WEB LINK 2: ");
		if (p != -1)
		{
			iWeb = 2;
		}
	}


	if (p != -1)
	{

		CString link = msg.Mid(p + 10);

		if (iWeb == 2)
			link = msg.Mid(p + 12);
	//	ShellExecute(NULL, _T("open"), link,
	//		NULL, NULL, SW_SHOWNORMAL);
		if (iWeb == 1)
		{
			microsipDlg->dlgTop->ShowWindow(SW_NORMAL);
			microsipDlg->dlgTop->m_ietop.Navigate(link, NULL, NULL, NULL, NULL);
			
		}

		if (iWeb == 2)
		{
			microsipDlg->dlgBot->ShowWindow(SW_SHOW);
			microsipDlg->dlgBot->m_expbot.Navigate(link, NULL, NULL, NULL, NULL);
			
		}

		return true;

	}

	return false;

}

void MessagesDlg::AddMessage(MessagesContact* messagesContact, CString message, int type, BOOL blockForeground)
{
	if (type == MSIP_MESSAGE_TYPE_REMOTE)
	{
		ShowDailyInformation(message);
		return;
		
	}
	
	
	CTime tm = CTime::GetCurrentTime();

	if (type == MSIP_MESSAGE_TYPE_SYSTEM ) {
		if ( messagesContact->lastSystemMessage == message && messagesContact->lastSystemMessageTime > tm.GetTime()-2) {
			messagesContact->lastSystemMessageTime = tm;
			return;
		}
		messagesContact->lastSystemMessage = message;
		messagesContact->lastSystemMessageTime = tm;
	} else if (!messagesContact->lastSystemMessage.IsEmpty()) {
		messagesContact->lastSystemMessage = _T("");
	}

	if (IsWindowVisible() && !blockForeground) {
		//SetForegroundWindow();
	}
	CRichEditCtrl richEdit;
	MessagesContact* messagesContactSelected = GetMessageContact();

	CRichEditCtrl *richEditList = (CRichEditCtrl *)GetDlgItem(IDC_LIST);
	if (messagesContactSelected != messagesContact) {
		CRect rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 300;
		rect.bottom = 300;
		richEdit.Create(ES_MULTILINE | ES_READONLY | ES_NUMBER | WS_VSCROLL, rect, this, NULL);
		richEdit.SetFont(&fontList);
		richEdit.SetParaFormat(para);	

		CString messages = messagesContact->messages;
		EDITSTREAM es;
		es.dwCookie = (DWORD) &messages;
		es.pfnCallback = MEditStreamInCallback; 
		richEdit.StreamIn(SF_RTF, es);

		richEditList = &richEdit;
	}

	COLORREF color;
	CString name;
	if (type==MSIP_MESSAGE_TYPE_LOCAL) {
		color = RGB (0,0,0);
		if (!accountSettings.account.displayName.IsEmpty()) {
			name = accountSettings.account.displayName;
		}
	} else if (type==MSIP_MESSAGE_TYPE_REMOTE) {
		color = RGB (21,101,206);
		name = messagesContact->name;
		int pos = name.Find(_T(" ("));
		if (pos==-1) {
			pos = name.Find(_T("@"));
		}
		if (pos!=-1) {
			name = name.Mid(0,pos);
		}
	}

	int nBegin;
	CHARFORMAT cf;
	CString str;

	CString time = tm.Format(_T("%X"));

	nBegin = richEditList->GetTextLengthEx(GTL_NUMCHARS);
	richEditList->SetSel(nBegin, nBegin);
	str.Format(_T("[%s]  "),time);
	richEditList->ReplaceSel( str );
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.crTextColor = RGB (131,131,131);
	cf.dwEffects = 0;
	cf.yHeight = 160;
	richEditList->SetSel(nBegin,-1);
	richEditList->SetSelectionCharFormat(cf);

	if (type != MSIP_MESSAGE_TYPE_SYSTEM) {
		cf.yHeight = 200;
	}
	if (name.GetLength()) {
		nBegin = richEditList->GetTextLengthEx(GTL_NUMCHARS);
		richEditList->SetSel(nBegin, nBegin);
		richEditList->ReplaceSel( name + _T(": "));
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
		cf.crTextColor = color;
		cf.dwEffects = CFE_BOLD;
		richEditList->SetSel(nBegin,-1);
		richEditList->SetSelectionCharFormat(cf);
	}

	nBegin = richEditList->GetTextLengthEx(GTL_NUMCHARS);
	richEditList->SetSel(nBegin, nBegin);
	richEditList->ReplaceSel(message+_T("\r\n"));
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;

	cf.crTextColor = type == MSIP_MESSAGE_TYPE_SYSTEM ? RGB (131, 131, 131) : color;
	cf.dwEffects = 0;

	richEditList->SetSel(nBegin,-1);
	richEditList->SetSelectionCharFormat(cf);

	if (messagesContactSelected == messagesContact)	{
		richEditList->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	} else {
		for (int i = 0; i < tab->GetItemCount(); i++) {
			if (messagesContact == GetMessageContact(i))
			{
				//tab->HighlightItem(i, TRUE);
				break;
			}
		}
	}

	str=_T("");
	EDITSTREAM es;
	es.dwCookie = (DWORD) &str;
	es.pfnCallback = MEditStreamOutCallback; 
	richEditList->StreamOut(SF_RTF, es);
	messagesContact->messages=str;

	m_bar.SetPaneText(0, messagesContact->lastSystemMessage);
	

}

void MessagesDlg::OnEnMsgfilterMessage(NMHDR *pNMHDR, LRESULT *pResult)
{
	MSGFILTER *pMsgFilter = reinterpret_cast<MSGFILTER *>(pNMHDR);

	if (pMsgFilter->msg == WM_CHAR) {
		if ( pMsgFilter->wParam == VK_RETURN ) {
			CRichEditCtrl* richEdit = (CRichEditCtrl*)GetDlgItem(IDC_MESSAGE);
			CString message;
			int len = richEdit->GetWindowTextLength();
			LPTSTR ptr = message.GetBuffer(len);
			richEdit->GetWindowText(ptr,len+1);
			message.ReleaseBuffer();
			message.Trim();
			if (message.GetLength()) {
				MessagesContact* messagesContact = GetMessageContact();
				if (SendMessage (messagesContact,message) ) {
					richEdit->SetWindowText(NULL);
					GotoDlgCtrl(richEdit);
					AddMessage(messagesContact, message, MSIP_MESSAGE_TYPE_LOCAL);
					if (accountSettings.localDTMF) {
						microsipDlg->onPlayerPlay(MSIP_SOUND_MESSAGE_OUT, 0);  //RingIn(_T("messageout.wav"),TRUE);
					}
				}
			}
			*pResult= 1;
			return;
		}
	}
	*pResult = 0;
}

BOOL MessagesDlg::SendMessage(MessagesContact* messagesContact, CString message, CString number)
{
	message.Trim();
	if (message.GetLength())
	{
		pjsua_acc_id acc_id;
		pj_str_t pj_uri;
		SelectSIPAccount(messagesContact?messagesContact->number:number,acc_id,pj_uri);
		pj_str_t pj_message = StrToPjStr ( message );
		pj_status_t status = pjsua_im_send( acc_id, &pj_uri, NULL, &pj_message, NULL, NULL );
		if ( status != PJ_SUCCESS ) {
			if (messagesContact) {
				CString message = GetErrorMessage(status);
				AddMessage(messagesContact,message);
			}
		} else {
			return TRUE;
		}
	}
	return FALSE;
}

MessagesContact* MessagesDlg::GetMessageContact(int i)
{
	if (i ==-1) {
		i = tab->GetCurSel();
	}
	if (i != -1) {
		TCITEM item;
		item.mask = TCIF_PARAM;
		tab->GetItem(i, &item);
		return (MessagesContact*) item.lParam;
	}
	return NULL;
}
void MessagesDlg::OnBnClickedVideoCall()
{
	CallStart(true);
}

void MessagesDlg::OnBnClickedHold()
{
	MessagesContact* messagesContactSelected = GetMessageContact();
	if (messagesContactSelected->callId!=-1) {
		pjsua_call_info info;
		pjsua_call_get_info(messagesContactSelected->callId,&info);
		if (info.media_cnt>0) {
			if (info.media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD || info.media_status == PJSUA_CALL_MEDIA_NONE) {
				call_hold_all_except(messagesContactSelected->callId);
				pjsua_call_reinvite(messagesContactSelected->callId, PJSUA_CALL_UNHOLD, NULL);
			} else {
				pjsua_call_set_hold(messagesContactSelected->callId, NULL);
			}
		}
	}
}

void MessagesDlg::OnBnClickedTransfer()
{
	if (!microsipDlg->transferDlg)
	{
		microsipDlg->transferDlg = new Transfer(this);
	}
	microsipDlg->transferDlg->attended = 0;
	microsipDlg->transferDlg->SetForegroundWindow();
}

void MessagesDlg::OnBnClickedAttdTransfer()
{
	if (!microsipDlg->transferDlg)
	{
		microsipDlg->transferDlg = new Transfer(this);
	}
	microsipDlg->transferDlg->attended = 1;
	microsipDlg->transferDlg->SetForegroundWindow();
}

void MessagesDlg::OnBnClickedConference()
{
	if (!microsipDlg->transferDlg)
	{
		microsipDlg->transferDlg = new Transfer(this);
	}
	microsipDlg->transferDlg->attended = 2;
	microsipDlg->transferDlg->SetForegroundWindow();
}

void MessagesDlg::OnBnClickedMerge()
{
	int msgboxID = ::MessageBox(
		this->m_hWnd,
		(LPCWSTR)L"Are you sure to merge all calls?",
		(LPCWSTR)L"Merge Calls",
		MB_OKCANCEL
		);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;
	case IDOK:
		// TODO: add code
		char URL[1024];
		char* geturi = "/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=MERGEALLCALLS&exten=%s";
		wsprintfA(URL, geturi, (LPCSTR)(CStringA)(accountSettings.account.username));

		Request(GET, "www.heightsre.com", URL, NULL, NULL);

		CString log = L"merge the all current calls";
		microsipDlg->setOperationLog(log);





		break;

	}







}



void MessagesDlg::OnBnClickedToCell()
{
	/*
	if (!microsipDlg->transferDlg)
	{
		microsipDlg->transferDlg = new Transfer(this);
	}
	microsipDlg->transferDlg->attended = 2;
	microsipDlg->transferDlg->SetForegroundWindow();
	*/
	int msgboxID = ::MessageBox(
		this->m_hWnd,
		(LPCWSTR)L"Are you sure to transfer the call to your cell phone?",
		(LPCWSTR)L"Transfer to Cell",
		 MB_OKCANCEL
		);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;
	case IDOK:
		// TODO: add code
		char URL[1024];
		char* geturi = "/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=TRANSFERINGTOCELL&exten=%s";
		wsprintfA(URL, geturi, (LPCSTR)(CStringA)(accountSettings.account.username));

		Request(GET, "www.heightsre.com", URL, NULL, NULL);







		break;
	
	}




}




void MessagesDlg::OnBnClickedEnd()
{
	MessagesContact* messagesContact = GetMessageContact();
	call_hangup_fast(messagesContact->callId);
}

void MessagesDlg::OnCloseAllTabs()
{
//	microsipDlg->pageDialer->m_showCall.ShowWindow(SW_HIDE);
//	microsipDlg->pageDialer->m_hideCall.ShowWindow(SW_HIDE);
	int i = 0;
	while (i < tab->GetItemCount()) {
		if (CloseTab(i)) {
			i = 0;
		} else {
			i++;
		}
	}
	
}

void MessagesDlg::OnGoToLastTab()
{
	int i = 0;
	BOOL found = FALSE;
	int lastCallIndex = -1;
	while (i < tab->GetItemCount())	{
		MessagesContact* messagesContact = GetMessageContact(i);
		if (messagesContact->callId != -1) {
			found = TRUE;
			if (tab->GetCurSel() != i) {
				long result;
				OnTcnSelchangingTab(NULL, &result);
				tab->SetCurSel(i);
				OnChangeTab();
				break;
			}
		}
		if (messagesContact == lastCall) {
			lastCallIndex = i;
		}
		i++;
	}
	if (!found && lastCallIndex!=-1) {
		if (tab->GetCurSel() != lastCallIndex) {
			long result;
			OnTcnSelchangingTab(NULL, &result);
			tab->SetCurSel(lastCallIndex);
			OnChangeTab();
		}
	}
}

int MessagesDlg::GetCallDuration()
{
	int duration = -1;
	pjsua_call_info call_info;
	int i = 0;
	while (i < tab->GetItemCount()) {
		MessagesContact* messagesContact = GetMessageContact(i);
		if (messagesContact->callId != -1) {
			if (pjsua_call_get_info(messagesContact->callId, &call_info)==PJ_SUCCESS) {
				if (call_info.state == PJSIP_INV_STATE_CONFIRMED) {
					duration = call_info.connect_duration.sec;
				}
			}
		}
		i++;
	}
	return duration;
}

void MessagesDlg::OnCopy()
{
	CRichEditCtrl* richEditList = (CRichEditCtrl*)GetDlgItem(IDC_LIST);
	richEditList->Copy();
}

void MessagesDlg::OnSelectAll()
{
	CRichEditCtrl* richEditList = (CRichEditCtrl*)GetDlgItem(IDC_LIST);
	richEditList->SetSel(0,-1);
}

void MessagesDlg::OnBnClickedCloseAll()
{
	OnCloseAllTabs();
}

void MessagesDlg::OnBnClickedLastCall()
{
	OnGoToLastTab();
}


void MessagesDlg::ShowCallInformation(CString number, CString type, CString phonebankDID)
{

	int pos = number.Find(_T("@"));

	if (pos != -1)
		number = number.Left(pos);

	CString ext = accountSettings.account.username;

	/*

	CString url = L"https://apache.heightsre.com/#/search/";
	url += number;
	url += L"/";
	url += type;
	url += L"/";
	url += ext;
	url += L"/";
	url += phonebankDID;*/


	CString url = L"https://www.heightsre.com/8525760F004BB8DF/GetCallInformation?openAgent"; //“http://apache.heightsre.com/#/search/";

	url += (L"&number=" + number);
	url += (L"&type=" + type);
	url += (L"&ext=" + ext);
	url += (L"&pbDID=" + phonebankDID);
	//microsipDlg->DialogWeb->m_web

	VARIANT v;
	//	v.uintVal = navOpenInNewTab;
	//m_webBrowser.Stop();
	if (URL == url) return;

	URL = url;
	//microsipDlg->ShowMessage(URL);

///	m_webBrowser.Navigate(URL, NULL, NULL, NULL, NULL);
	//DialogWeb->ShowWindow(SW_NORMAL);


	try
	{
		m_webBrowser.Navigate(URL, NULL, NULL, NULL, NULL);
		//DialogWeb->ShowWindow(SW_NORMAL);
	}
	catch (...)
	{
		;
	}
	if (number == L"324") return;
	CString urlHistory = L"https://www.heightsre.com/__8525760f004bb8df.nsf/getCallHistoryByNum?openagent&ext=" + ext + L"&phone=" + number;

	microsipDlg->dlgBot->m_expbot.Navigate(urlHistory, NULL, NULL, NULL, NULL);
	microsipDlg->dlgBot->ShowWindow(SW_SHOW);

}


void MessagesDlg::ShowCallInformation(CString number, CString type)
{

	int pos = number.Find(_T("@"));

	if (pos != -1)
		number = number.Left(pos);

	CString ext = accountSettings.account.username;



	CString url = L"https://www.heightsre.com/8525760F004BB8DF/GetCallInformation?openAgent"; //“http://apache.heightsre.com/#/search/";

	url += (L"&number=" + number);
	url += (L"&type=" + type);
	url += (L"&ext=" + ext);
/*	url += number;
	url += L"/";
	url += type;
	url += L"/";
	url += ext;*/
	//microsipDlg->DialogWeb->m_web

//	VARIANT v;
//	v.uintVal = navOpenInNewTab;
	//m_webBrowser.Stop();
	if (URL == url) return;

	URL = url;
//	m_webBrowser.Stop();
	try
	{
		m_webBrowser.Navigate(URL, NULL, NULL, NULL, NULL);
		//DialogWeb->ShowWindow(SW_NORMAL);
	}
	catch(...)
	{
		;
	}

	if (number == L"324") return;
	CString urlHistory = L"https://www.heightsre.com/__8525760f004bb8df.nsf/getCallHistoryByNum?openagent&ext=" + ext + L"&phone=" + number;

	microsipDlg->dlgBot->m_expbot.Navigate(urlHistory, NULL, NULL, NULL, NULL);
	microsipDlg->dlgBot->ShowWindow(SW_SHOW);


}

void MessagesDlg::OnStnClickedLogo()
{
	// TODO: Add your control notification handler code here
}


void MessagesDlg::OnBnClickedTabclose()
{
	// TODO: Add your control notification handler code here

	
//	int n = ::MessageBox(m_hWnd, _T("Are you sure to close the current tab and end the call?"), _T("Notice"), MB_OKCANCEL);

//	if (n == IDOK)
	
	
	   CloseTab(tab->GetCurSel());

}


void MessagesDlg::OnTabClose()
{
	CloseTab(tab->GetCurSel());// TODO: Add your command handler code here
}

/*
void MessagesDlg::onUpdateNewVersion()
{
	// TODO: Add your command handler code here

	int msgboxID = ::MessageBox(
		this->m_hWnd,
		(LPCWSTR)L"Press OK to start verson updating and microsip will be closed! When finishing updating, it will be restarted again.",
		(LPCWSTR)L"Version Updating",
		MB_OKCANCEL
		);

	if (msgboxID == IDOK)
	{







	}







}*/
BEGIN_EVENTSINK_MAP(MessagesDlg, CBaseDialog)
	ON_EVENT(MessagesDlg, IDC_EXPLORER1, 259, MessagesDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(MessagesDlg, IDC_EXPLORER1, 284, MessagesDlg::NewProcessExplorer1, VTS_I4 VTS_DISPATCH VTS_PBOOL)
	ON_EVENT(MessagesDlg, IDC_EXPLORER1, 251, MessagesDlg::NewWindow2Explorer1, VTS_PDISPATCH VTS_PBOOL)
	ON_EVENT(MessagesDlg, IDC_EXPLORER1, 273, MessagesDlg::NewWindow3Explorer1, VTS_PDISPATCH VTS_PBOOL VTS_UI4 VTS_BSTR VTS_BSTR)
	ON_EVENT(MessagesDlg, IDC_EXPLORER1, 250, MessagesDlg::BeforeNavigate2Explorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()


void MessagesDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: Add your message handler code here
}


//void MessagesDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CBaseDialog::OnActivate(nState, pWndOther, bMinimized);
//
//	
//
//
//
//
//	// TODO: Add your message handler code here
//}


void MessagesDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (tab->GetItemCount() == 0) bShow = false;
	
	CBaseDialog::OnShowWindow(bShow, nStatus);
	//if (!bShow) return;
	CRect cRectMessage;

	GetWindowRect(&cRectMessage);
	int Mwidth = cRectMessage.Width();

	CRect cRect;
	microsipDlg->GetWindowRect(&cRect);

	cRectMessage.top = cRect.top;
	cRectMessage.bottom = cRect.bottom;
	cRectMessage.left = cRect.left - Mwidth;
	cRectMessage.right = cRectMessage.left + Mwidth;

//	if (IsWindowVisible() && !IsZoomed() && !IsIconic())
		MoveWindow(&cRectMessage);

		microsipDlg->pageDialer->m_showCall.ShowWindow(SW_HIDE);
		microsipDlg->pageDialer->m_hideCall.ShowWindow(SW_SHOW);

     
		//microsipDlg->dlgRigh->ShowWindow(SW_SHOW);

		microsipDlg->isMessgeShown = true;
	// TODO: Add your message handler code here
}


void MessagesDlg::NewProcessExplorer1(long lCauseFlag, LPDISPATCH pWB2, BOOL* Cancel)
{
	// TODO: Add your message handler code here
}


void MessagesDlg::NewWindow2Explorer1(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	// TODO: Add your message handler code here
}


void MessagesDlg::NewWindow3Explorer1(LPDISPATCH* ppDisp, BOOL* Cancel, unsigned long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl)
{
	// TODO: Add your message handler code here
}


void MessagesDlg::OnBnClickedOpendoor()
{
	// TODO: Add your control notification handler code here

	microsipDlg->pageDialer->OnBnClickedKey6();

}


void MessagesDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == IDT_TIMER_DAILY) {
		OnDailyDuration();
	}


	CBaseDialog::OnTimer(nIDEvent);
}

void MessagesDlg::OnDailyDuration(bool stop)
{
	CString dur = L"";
	if (!stop)
	{
		SetTimer(IDT_TIMER_DAILY, 1000, NULL);
		dailyDuration++;
		dur = GetDuration(dailyDuration, true);
		

	}
	else
		dailyDuration = 0;




	SetDlgItemText(IDC_TIME_PASS, dur);




}


afx_msg LRESULT MessagesDlg::OnMywmAddMessage(WPARAM wParam, LPARAM lParam)
{
	
	/*
	MessagesContact* messagesContact = (MessagesContact*)wParam;
	CString str = ()
	microsipDlg->messagesDlg->AddMessage(messagesContact, str);*/
	
	return 0;
}


void MessagesDlg::BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	CString url = (wchar_t*)(_bstr_t)*URL;
	//microsipDlg->ShowMessage(url);
	int p = url.Find(L"TEXT%20");

	if (p != -1)
	{
		*Cancel = true;
		// url.Replace(L"TEXT ", L"");
		microsipDlg->dlgText->superDocid = url.Mid(p + 7);
		microsipDlg->dlgText->strType = L"SUPER";
		 microsipDlg->dlgText->ShowWindow(SW_NORMAL);

	}

	int p1 = url.Find(L"CALL%20");
	if (p1 != -1)
	{
		*Cancel = true;
		CString phoneNum = url.Mid(p1 + 7);
		//CComboBox *combobox = (CComboBox*)microsipDlg->pageDialer->GetDlgItem(IDC_NUMBER);
		//combobox->SetWindowText(phoneNum);
		microsipDlg->pageDialer->m_acedit.SetWindowText(phoneNum);
		microsipDlg->pageDialer->OnBnClickedCall();
	}

	int p2 = url.Find(L"EBILL%20");
	if (p2 != -1)
	{
		*Cancel = true;
		microsipDlg->dlgRigh->custDocid = url.Mid(p2 + 8);
		microsipDlg->dlgRigh->OnBnClickedBtnebill();
		//CComboBox *combobox = (CComboBox*)microsipDlg->pageDialer->GetDlgItem(IDC_NUMBER);
		//combobox->SetWindowText(phoneNum);
		//microsipDlg->pageDialer->m_acedit.SetWindowText(phoneNum);
	//	microsipDlg->pageDialer->OnBnClickedCall();
	}

	int p3 = url.Find(L"RENEW%20");
	if (p3 != -1)
	{
		*Cancel = true;
		microsipDlg->dlgRigh->custDocid = url.Mid(p3 + 8);
		microsipDlg->dlgRigh->OnBnClickedBtnsendlease();
		//CComboBox *combobox = (CComboBox*)microsipDlg->pageDialer->GetDlgItem(IDC_NUMBER);
		//combobox->SetWindowText(phoneNum);
		//microsipDlg->pageDialer->m_acedit.SetWindowText(phoneNum);
		//	microsipDlg->pageDialer->OnBnClickedCall();
	}

	int p4 = url.Find(L"REQCOI%20");
	if (p4 != -1)
	{
		*Cancel = true;
		microsipDlg->dlgRigh->custDocid = url.Mid(p4 + 9);
		microsipDlg->dlgRigh->OnBnClickedBtnpreqcoi();
		//CComboBox *combobox = (CComboBox*)microsipDlg->pageDialer->GetDlgItem(IDC_NUMBER);
		//combobox->SetWindowText(phoneNum);
		//microsipDlg->pageDialer->m_acedit.SetWindowText(phoneNum);
		//	microsipDlg->pageDialer->OnBnClickedCall();
	}

	int p5 = url.Find(L"TEXTVDR%20");

	if (p5 != -1)
	{
		*Cancel = true;
		// url.Replace(L"TEXT ", L"");
		microsipDlg->dlgText->vdrDocid = url.Mid(p5 + 10);
		microsipDlg->dlgText->strType = L"VENDOR";

		microsipDlg->dlgText->ShowWindow(SW_NORMAL);

	}
	
	
	// TODO: Add your message handler code here
}


BOOL MessagesDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd* pWnd = GetDlgItem(IDC_LISTDAILY);
	if (WM_RBUTTONDOWN == pMsg->message)
	{
		if (pWnd && pWnd->GetSafeHwnd() == pMsg->hwnd)
		{
			//AfxMessageBox("此处加菜单");   
		//	CMenu   menu;
		//	menu.CreatePopupMenu();     //动态创建弹出式菜单对象   
		//	menu.AppendMenu(MF_STRING, IDC_LISTITEM_COPY, _T("Copy")); // 第一项菜单项   
			CPoint pt;
			GetCursorPos(&pt);
			menu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
			

		}
	}
	




	return CBaseDialog::PreTranslateMessage(pMsg);
}


void MessagesDlg::CopyListItem()
{
	USES_CONVERSION;
	// TODO: Add your control notification handler code here
	CString szResult, szItem;        //最后放上剪贴板的内容，每个列表项内容
	int all[1024];
	int nLen = m_listDaily.GetSelItems(1024, all);
	if (0 == nLen)
	{
		//logStr = _T("Please select the copy content....");
		//mInfoList.InsertString(mInfoList.GetCount(), logStr);
		//mInfoList.SetCurSel(mInfoList.GetCount() - 1);
		return;
	}
	for (int i = 0; i<nLen; i++)
	{
		m_listDaily.GetText(all[i], szItem);
		szResult += szItem + _T("\r\n");
	}
	if (OpenClipboard())
	{
		HGLOBAL clipbuffer;
		EmptyClipboard();
		nLen = szResult.GetLength();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, 2 * (nLen + 1));
		TCHAR* buffer = (TCHAR*)GlobalLock(clipbuffer);
		//strcpy(buffer,LPCSTR(szResult)); 

		lstrcpy((LPWSTR)buffer, (LPCWSTR)szResult);
		//strcpy_s(buffer, szResult.GetLength()+1, W2A( szResult ));
		::GlobalUnlock(clipbuffer);
		SetClipboardData(CF_UNICODETEXT, clipbuffer);
		CloseClipboard();
	}





}
