#pragma once

#include "resource.h"
#include "global.h"
#include "ClosableTabCtrl.h"
#include "BaseDialog.h"

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
#include "explorer.h"
#include "afxwin.h"
#define IDC_LISTITEM_COPY 35610

class MessagesDlg :
	public CBaseDialog
{
	CFont fontList;
	CFont fontMessage;
	PARAFORMAT2 para;
	//CClosableTabCtrl tabComponent;
	CTabCtrl tabComponent;
public:

	MessagesDlg(CWnd* pParent = NULL);	// standard constructor
	~MessagesDlg();
	enum { IDD = IDD_MESSAGES };

	MessagesContact* AddTab(CString number, CString name = CString(), BOOL activate = FALSE, pjsua_call_info *call_info = NULL, BOOL notShowWindow = FALSE, BOOL ifExists = FALSE);
	void OnChangeTab(pjsua_call_info *p_call_info = NULL);
	void OnEndCall(pjsua_call_info *call_info);
	void Call(BOOL hasVideo = FALSE);
	pjsua_call_id CallMake(CString number, bool hasVideo = false, pj_status_t *pStatus = NULL);
	void AddMessage(MessagesContact* messagesContact, CString message, int type = MSIP_MESSAGE_TYPE_SYSTEM, BOOL blockForeground = FALSE);
	MessagesContact* GetMessageContact(int i = -1);
	int GetCallDuration();
	BOOL SendMessage(MessagesContact* messagesContact, CString message, CString number = _T(""));
	void UpdateHoldButton(pjsua_call_info *call_info);	

	void ShowCallInformation(CString number, CString type);
	void ShowCallInformation(CString number, CString type, CString phonebankDID);
	void ShowDailyInformation(CString message);


	bool chekIfWebLink(CString msg);

	bool checkIfTotalInformtion(CString msg);

	bool checkIfDailyInfo(CString msg);

	void ShowDailyNumInfo();


	bool checkIfFunction(CString msg);

	void OnDailyDuration(bool stop = false);

	//void ShowCallInformation(CString number);

	//CClosableTabCtrl* tab;
	CTabCtrl* tab;
	MessagesContact* lastCall;

	CString URL = _T("http://apache.heightsre.com/login.html");
	CStatusBar m_bar;

	CString totalDaily = L"";
	CString scurrDaily = L"0";
	int icurrDaily = 0;

	int dailyDuration = 0;

	UINT dailyTimer;

	boolean firstTimeMove = true;
	CMenu   menu;
private:
	void UpdateCallButton(BOOL active = FALSE, pjsua_call_info *call_info = NULL);
	void CallStart(bool hasVideo = false);
	BOOL CloseTab(int i, BOOL safe = FALSE);
	void UpdateTabItem(BOOL active = FALSE);

protected:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT type, int w, int h);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnContextMenu(WPARAM wParam,LPARAM lParam);
	afx_msg void OnEnMsgfilterMessage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnCloseTab(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedCallEnd();
	afx_msg void OnBnClickedVideoCall();
	afx_msg void OnBnClickedHold();
	afx_msg void OnBnClickedTransfer();
	afx_msg void OnBnClickedAttdTransfer();
	afx_msg void OnBnClickedConference();
	afx_msg void OnBnClickedToCell();
	afx_msg void OnBnClickedMerge();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnCloseAllTabs(); 
	afx_msg void OnGoToLastTab();
	afx_msg void OnBnClickedCloseAll();
	afx_msg void OnBnClickedLastCall();
	afx_msg void OnCopy();
	afx_msg void OnSelectAll();
	CExplorer m_webBrowser;
	afx_msg void OnStnClickedLogo();
	afx_msg void OnBnClickedTabclose();
	afx_msg void OnTabClose();
	//afx_msg void onUpdateNewVersion();
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void NewProcessExplorer1(long lCauseFlag, LPDISPATCH pWB2, BOOL* Cancel);
	void NewWindow2Explorer1(LPDISPATCH* ppDisp, BOOL* Cancel);
	void NewWindow3Explorer1(LPDISPATCH* ppDisp, BOOL* Cancel, unsigned long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl);
	afx_msg void OnBnClickedOpendoor();
	CListBox m_listDaily;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnMywmAddMessage(WPARAM wParam, LPARAM lParam);
public:
	void BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CopyListItem();
};