#include "StdAfx.h"
#include "Calls.h"
#include "microsip.h"
#include "global.h"
#include "settings.h"
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
#include "microsipDlg.h"

#ifndef _GLOBAL_NO_CALLS

static CmicrosipDlg *microsipDlg;

Calls::Calls(CWnd* pParent /*=NULL*/)
: CBaseDialog(Calls::IDD, pParent)
{
	microsipDlg = (CmicrosipDlg* ) AfxGetMainWnd();	
	Create (IDD, pParent);
}

Calls::~Calls(void)
{
}

BOOL Calls::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	AutoMove(IDC_CALLS,0,0,100,100);

	TranslateDialog(this->m_hWnd);

	nextKey = 0;
	
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);

	list->SetExtendedStyle( list->GetExtendedStyle() |  LVS_EX_FULLROWSELECT );

	imageList = new CImageList();
	imageList->Create(16,16,ILC_COLOR32,3,3);
	imageList->SetBkColor(RGB(255, 255, 255));
	imageList->Add(theApp.LoadIcon(IDI_CALL_OUT));
	imageList->Add(theApp.LoadIcon(IDI_CALL_IN));
	imageList->Add(theApp.LoadIcon(IDI_CALL_MISS));
	list->SetImageList(imageList,LVSIL_SMALL);

	list->InsertColumn(0, Translate(_T("Name")), LVCFMT_LEFT, accountSettings.callsWidth0>0 ? accountSettings.callsWidth0 : 81);
	list->InsertColumn(1,Translate(_T("Number")),LVCFMT_LEFT, accountSettings.callsWidth1>0?accountSettings.callsWidth1:81);
	list->InsertColumn(2,Translate(_T("Date")),LVCFMT_LEFT, accountSettings.callsWidth2>0?accountSettings.callsWidth2:71);
	list->InsertColumn(3,Translate(_T("Duration")),LVCFMT_LEFT, accountSettings.callsWidth3>0?accountSettings.callsWidth3:40);
	list->InsertColumn(4,Translate(_T("Info")),LVCFMT_LEFT, accountSettings.callsWidth4>0?accountSettings.callsWidth4:50);
	CallsLoad();





	//SetTimer(IDT_TIMER_GETNAMESCALLS, 1000*60*10, NULL);



	return TRUE;
}

void Calls::PostNcDestroy()
{
	CBaseDialog::PostNcDestroy();
	microsipDlg->pageCalls=NULL;
	delete imageList;
	delete this;
}

BEGIN_MESSAGE_MAP(Calls, CBaseDialog)
	ON_NOTIFY(HDN_ENDTRACK, 0, OnEndtrack)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_COMMAND(ID_CALL,OnMenuCall)
	ON_COMMAND(ID_CHAT,OnMenuChat)
	ON_COMMAND(ID_COPY,OnMenuCopy)
	ON_COMMAND(ID_DELETE,OnMenuDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CALLS, &Calls::OnNMDblclkCalls)
	ON_MESSAGE(WM_CONTEXTMENU,OnContextMenu)
#ifdef _GLOBAL_VIDEO
	ON_COMMAND(ID_VIDEOCALL,OnMenuCallVideo)
#endif
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CALLS, &Calls::OnLvnItemchangedCalls)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void Calls::OnEndtrack(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	int width = list->GetColumnWidth(phdn->iItem);
	switch (phdn->iItem) {
		case 0:
			accountSettings.callsWidth0 = width;
			break;
		case 1:
			accountSettings.callsWidth1 = width;
			break;
		case 2:
			accountSettings.callsWidth2 = width;
			break;
		case 3:
			accountSettings.callsWidth3 = width;
			break;
		case 4:
			accountSettings.callsWidth4 = width;
			break;
	}
	microsipDlg->AccountSettingsPendingSave();
	*pResult = 0;
}


void Calls::OnBnClickedOk()
{
	MessageDlgOpen(accountSettings.singleMode);
}

void Calls::OnBnClickedCancel()
{
	microsipDlg->ShowWindow(SW_HIDE);
}

LRESULT Calls::OnContextMenu(WPARAM wParam,LPARAM lParam)
{
	int x = GET_X_LPARAM(lParam); 
	int y = GET_Y_LPARAM(lParam); 
	POINT pt = { x, y };
	RECT rc;
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	POSITION pos = list->GetFirstSelectedItemPosition();
	int selectedItem = -1;
	if (pos) {
		selectedItem = list->GetNextSelectedItem(pos);
	}
	if (x!=-1 || y!=-1) {
		ScreenToClient(&pt);
		GetClientRect(&rc); 
		if (!PtInRect(&rc, pt)) {
			x = y = -1;
		} 
	} else {
		if (selectedItem != -1) {
			list->GetItemPosition(selectedItem,&pt);
			list->ClientToScreen(&pt);
			x = 40+pt.x;
			y = 8+pt.y;
		} else {
			::ClientToScreen((HWND)wParam, &pt);
			x = 10+pt.x;
			y = 26+pt.y;
		}
	}
	if (x!=-1 || y!=-1) {
		CMenu menu;
		menu.LoadMenu(IDR_MENU_CONTACT);
		CMenu* tracker = menu.GetSubMenu(0);
		TranslateMenu(tracker->m_hMenu);
		tracker->RemoveMenu(ID_ADD,0);
		tracker->RemoveMenu(ID_EDIT,0);
		if (selectedItem != -1) {
			tracker->EnableMenuItem(ID_CALL, FALSE);
#ifdef _GLOBAL_VIDEO
			tracker->EnableMenuItem(ID_VIDEOCALL, FALSE);
#endif
			tracker->EnableMenuItem(ID_CHAT, FALSE);
			tracker->EnableMenuItem(ID_COPY, FALSE);
			tracker->EnableMenuItem(ID_DELETE, FALSE);
		} else {
			tracker->EnableMenuItem(ID_CALL, TRUE);
#ifdef _GLOBAL_VIDEO
			tracker->EnableMenuItem(ID_VIDEOCALL, TRUE);
#endif
			tracker->EnableMenuItem(ID_CHAT, TRUE);
			tracker->EnableMenuItem(ID_COPY, TRUE);
			tracker->EnableMenuItem(ID_DELETE, TRUE);
		}
#ifdef _GLOBAL_NO_MESSAGING
		tracker->RemoveMenu(ID_CHAT,MF_BYCOMMAND);
#endif
		tracker->TrackPopupMenu( 0, x, y, this );
		return TRUE;
	}
	return DefWindowProc(WM_CONTEXTMENU,wParam,lParam);
}

void Calls::MessageDlgOpen(BOOL isCall, BOOL hasVideo)
{
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	POSITION pos = list->GetFirstSelectedItemPosition();
	if (pos)
	{
		int i = list->GetNextSelectedItem(pos);
		Call *pCall = (Call *) list->GetItemData(i);
		microsipDlg->messagesDlg->AddTab(GetSIPURI(pCall->number), pCall->name, TRUE, NULL, isCall && accountSettings.singleMode);
	//	microsipDlg->ShowCallInformation(pCall->number);
		if (isCall)
		{
			microsipDlg->messagesDlg->Call(hasVideo);
		}
	}
}

void Calls::OnNMDblclkCalls(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem!=-1) {
		MessageDlgOpen(accountSettings.singleMode);
	}
	*pResult = 0;
}

void Calls::OnMenuCall()
{
	MessageDlgOpen(TRUE);
}

#ifdef _GLOBAL_VIDEO
void Calls::OnMenuCallVideo()
{
	MessageDlgOpen(TRUE, TRUE);
}
#endif

void Calls::OnMenuChat()
{
	MessageDlgOpen();
}

void Calls::OnMenuCopy()
{
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	POSITION pos = list->GetFirstSelectedItemPosition();
	if (pos)
	{
		int i = list->GetNextSelectedItem(pos);
		Call *pCall = (Call *) list->GetItemData(i);
		microsipDlg->CopyStringToClipboard(pCall->number);
	}
}

void Calls::OnMenuDelete()
{
	CListCtrl *pList= (CListCtrl*)GetDlgItem(IDC_CALLS);
	POSITION pos = pList->GetFirstSelectedItemPosition();
	while (pos)	{
		Delete(pList->GetNextSelectedItem(pos));
		pos = pList->GetFirstSelectedItemPosition();
	}
}

void Calls::Delete(int i)
{
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	Call *pCall = (Call *) list->GetItemData(i);
	pCall->number = _T("");
	CallSave(pCall);
	delete pCall;
	list->DeleteItem(i);
}

void Calls::CallSave(Call *pCall)
{
	CString key;
	CString data = !pCall->number.IsEmpty() ? CallEncode(pCall) : _T("null");
	key.Format(_T("%d"), pCall->key);
	WritePrivateProfileString(_T("Calls"), key, data, accountSettings.iniFile);
	if (pCall->key == nextKey) {
		nextKey++;
	}
}

void Calls::Add(pj_str_t id, CString number, CString name, int type)
{
	CString callId = PjToStr(&id);
	int i = Get(callId);
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	if (i==-1) {
		SIPURI sipuri;
		ParseSIPURI(number, &sipuri);
		if (!sipuri.user.IsEmpty()) {
			Call *pCall =  new Call();
			pCall->id = callId;
			if (accountSettings.account.domain == sipuri.domain || sipuri.domain.IsEmpty()) {
				pCall->number = sipuri.user;
			} else {
				pCall->number = sipuri.user+_T("@")+sipuri.domain;
			}
			pCall->name = name;
			pCall->type = type;
			pCall->time = CTime::GetCurrentTime().GetTime();
			pCall->duration = 0;
			pCall->key = nextKey;
			Insert(pCall);
			CallSave(pCall);
			int count;
			while ((count = list->GetItemCount())>1000) {
				Delete(count-1);
			}
		}
	} else {
		Call *pCall = (Call *) list->GetItemData(i);
		if (pCall->type == MSIP_CALL_MISS && pCall->type != type) {
			pCall->type = type;
			list->SetItem(i,0,LVIF_IMAGE,NULL,type,0,0,0);
			CallSave(pCall);
		}
	}
}

void Calls::SetDuration(pj_str_t id, int sec) {
	CString callId = PjToStr(&id);
	int i = Get(callId);
	if (i!=-1) {
		CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
		Call *pCall = (Call *) list->GetItemData(i);
		pCall->duration = sec;
		list->SetItemText(i,3,GetDuration(pCall->duration));
		CallSave(pCall);
	}
}

void Calls::SetInfo(pj_str_t id, CString str) {
	CString callId = PjToStr(&id);
	int i = Get(callId);
	if (i!=-1) {
		CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
		Call *pCall = (Call *) list->GetItemData(i);
		pCall->info = str;
		list->SetItemText(i,4,str);
		CallSave(pCall);
	}
}

int Calls::Get(CString id)
{
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	int count = list->GetItemCount();
	for (int i=0;i<count;i++)
	{
		Call *pCall = (Call *) list->GetItemData(i);
		if (pCall->id == id) {
			return i;
		}
	}
	return -1;
}


void Calls::Insert(Call *pCall)
{
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	CString number = pCall->number;
	CString name = pCall->name;
#if 0
	if (pCall->name != pCall->number) {
		number.AppendFormat(_T("%s (%s)"), pCall->name, pCall->number);
	}
	else {
		number = pCall->name;
	}
#endif // 0

	int i = list->InsertItem(LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, 0, name, 0, 0, pCall->type, (LPARAM)pCall);
//	int i = list->InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE,1,number,0,0,pCall->type,(LPARAM)pCall);

	int p = number.Find(L"@");

	if (p != -1){

		number = number.Mid(0, p);

	}


	list->SetItemText(i, 1, number);
	CTime t(pCall->time) ;
	list->SetItemText(i,2,t.Format(_T("%m/%d %H:%M")));
	list->SetItemText(i,3,GetDuration(pCall->duration));
	list->SetItemText(i,4,pCall->info);
}

void Calls::CallsClear()
{
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);
	list->DeleteAllItems();
	nextKey = 0;
}

void Calls::CallsLoad()
{
	CString key;
	CString rab;
	CString val;
	LPTSTR ptr = val.GetBuffer(255);
	int i=0;
	int emptyKey=-1;
	while (TRUE) {
		key.Format(_T("%d"),i);
		if (GetPrivateProfileString(_T("Calls"), key, NULL, ptr, 256, accountSettings.iniFile)) {
			if (val == _T("null")) {
				if (emptyKey==-1) {
					emptyKey = i;
				}
			} else {
				Call *pCall =  new Call();
				CallDecode(ptr, pCall);
				Insert(pCall);
				if (emptyKey!=-1) {
					rab.Format(_T("%d"),emptyKey);
					WritePrivateProfileString(_T("Calls"), rab, ptr, accountSettings.iniFile);
					pCall->key = emptyKey;	
					WritePrivateProfileString(_T("Calls"), key, _T("null"), accountSettings.iniFile);
					emptyKey++;
				} else {
					pCall->key = i;
				}
				nextKey = pCall->key;
			}
		} else {
			i--;
			break;
		}
		i++;
	}
	if (i!=-1) {
		nextKey++;
	}
	val.ReleaseBuffer();
	while (i>=nextKey) {
		key.Format(_T("%d"),i);
		WritePrivateProfileString(_T("Calls"), key, NULL, accountSettings.iniFile);
		i--;
	}
}

CString Calls::CallEncode(Call *pCall)
{
	CString data;
	data.Format(_T("%s;%s;%d;%d;%d;%s"), pCall->number, pCall->name, pCall->type, pCall->time, pCall->duration, pCall->info);
	return data;
}

void Calls::CallDecode(CString str, Call *pCall)
{
	pCall->number=str;
	pCall->name = pCall->number;
	pCall->type = 0;
	pCall->time = 0;
	pCall->duration = 0;

	CString rab;
	int begin;
	int end;
	begin = 0;
	end = str.Find(';', begin);

	if (end != -1)
	{
		pCall->number=str.Mid(begin, end-begin);
		begin = end + 1;
		end = str.Find(';', begin);
		if (end != -1)
		{
			pCall->name=str.Mid(begin, end-begin);
			begin = end + 1;
			end = str.Find(';', begin);
			if (end != -1)
			{
				pCall->type=atoi(CStringA(str.Mid(begin, end-begin)));
				if (pCall->type>2 || pCall->type<0) {
					pCall->type = 0;
				}
				begin = end + 1;
				end = str.Find(';', begin);
				if (end != -1)
				{
					pCall->time=atoi(CStringA(str.Mid(begin, end-begin)));
					begin = end + 1;
					end = str.Find(';', begin);
					if (end != -1)
					{
						pCall->duration=atoi(CStringA(str.Mid(begin, end-begin)));
						begin = end + 1;
						end = str.Find(';', begin);
						if (end != -1)
						{
							pCall->info=str.Mid(begin, end-begin);
							begin = end + 1;
							end = str.Find(';', begin);
						} else {
							pCall->info=str.Mid(begin);
						}
					}
				}
			}
		}
	}
}

CString Calls::GetNameByNumber(CString number)
{
	CString name;
	CListCtrl *list= (CListCtrl*)GetDlgItem(IDC_CALLS);

	CString sipURI = GetSIPURI(number);
	int n = list->GetItemCount();
	for (int i=0; i<n; i++) {
		Call* pCall = (Call *) list->GetItemData(i);
		if (GetSIPURI(pCall->number) == sipURI)
		{
			name = pCall->name;
			break;
		}
	}
	return name;
}

#endif

void Calls::OnLvnItemchangedCalls(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void Calls::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default


	if (IDT_TIMER_GETNAMESCALLS == nIDEvent)
	{

		CListCtrl *list = (CListCtrl*)GetDlgItem(IDC_CALLS);
		int count = list->GetItemCount();

		int proCount = 0;
		for (int i = 0; i<count; i++)
		{
			Call *pCall = (Call *)list->GetItemData(i);

			if (pCall->info.Find(L"Processed") == -1) continue;
			
			proCount++;
			CString number = pCall->number;

			int p = number.Find(L"@");

			if (p != -1){

				number = number.Mid(0, p);
				pCall->number = number;
			}

			pCall->name = getNameByNumberFromNotes(number);


			list->SetItemText(i, 0, pCall->name);

			pCall->info += " Processed";

			CallSave(pCall);
            

			if (proCount >= MAXPROCOUNT) break;
		}




	}



	CBaseDialog::OnTimer(nIDEvent);
}


CString Calls::getNameByNumberFromNotes(CString number)
{

    





	return CString();
}