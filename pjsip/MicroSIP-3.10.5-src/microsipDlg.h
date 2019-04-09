#pragma once

#include "const.h"
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>



#ifdef _GLOBAL_LIB_FILENAME
#pragma comment(lib, _GLOBAL_LIB_FILENAME)
#else

#ifdef NDEBUG
#ifdef _GLOBAL_VIDEO
#pragma comment(lib, "libpjproject-i386-Win32-vc8-Release-Static-Video.lib")
#else
#pragma comment(lib, "libpjproject-i386-Win32-vc8-Release-Static.lib")
#endif
#else
#ifdef _GLOBAL_VIDEO
#pragma comment(lib, "libpjproject-i386-Win32-vc8-Debug-Static-Video.lib")
#else
#pragma comment(lib, "libpjproject-i386-Win32-vc8-Debug-Static.lib")
#endif
#endif

#endif

#include "langpack.h"

#include "BaseDialog.h"
#include "RinginDlg.h"
#ifndef _GLOBAL_NO_ACCOUNT
#include "AccountDlg.h"
#endif
#ifndef _GLOBAL_NO_SETTINGS
#include "SettingsDlg.h"
#endif
#include "MessagesDlg.h"

#include "Dialer.h"
#ifndef _GLOBAL_NO_CONTACTS
#include "Contacts.h"
#endif
#include "Calls.h"
#include "Preview.h"
#include "Transfer.h"
#include "addons.h"
#include "dialogweb.h"
#include "DlgFop.h"

#include "DialogSupers.h"
#include "DlgOOO.h"

#include "DlgTop.h"
#include "DlgBot.h"
#include "DlgRight.h"
#include "DlgIE.h"
#include "DlgText.h"

#include <hash_map>
#include <iostream>
// CmicrosipDlg dialog

//using namespace std;
//using namespace stdext;

typedef std::pair<pjsua_call_id, CString> Callid_headervalue_Pair;


class CmicrosipDlg : public CBaseDialog
{
	// Construction
public:
	CmicrosipDlg(CWnd* pParent = NULL);	// standard constructor
	~CmicrosipDlg();

	// Dialog Data
	enum { IDD = IDD_MICROSIP_DIALOG };

	bool m_startMinimized;

	//CDialogWeb * DialogWeb;

	boolean isNotifyNew;
//	CString * vmcount;// = new CString("New Voicemail: ");


	boolean isMessgeShown;

	CString  vmcount;

	CString PhoneTrans;
	CString PhoneList;

	CString CURLOG;

	//CString NameList;

#ifndef _GLOBAL_NO_SETTINGS
	SettingsDlg* settingsDlg;
#endif
MessagesDlg* messagesDlg;
	Transfer* transferDlg;
	//CDlgFop* dlgFop;
	CDlgOOO* dlgOOO;

	CDlgTop* dlgTop;

	CDlgBot* dlgBot;


	CDlgRight * dlgRigh;

	CDlgIE * dlgIE;

	CDlgText * dlgText;

#ifdef _GLOBAL_ACCOUNT_REG
	Reg1* reg1Dlg;
	Reg2* reg2Dlg;
	Reg3* reg3Dlg;
#endif
#ifdef _GLOBAL_BALANCE_BEE
	int regUserId;
#endif
#ifdef _GLOBAL_CONFERENCE_DIALOG
	ConferenceDlg* conferenceDlg;
	CString lastCallNumber;
#endif

	Dialer* pageDialer;
#ifndef _GLOBAL_NO_CONTACTS
	Contacts* pageContacts;
#endif
#ifndef _GLOBAL_NO_CALLS
	Calls* pageCalls;
#endif
#ifdef _GLOBAL_PAGE_BUTTONS
	Buttons* pageButtons;
#endif

	CDialogSupers *pageSuper;

	BOOL notStopRinging;
	CArray <RinginDlg*> ringinDlgs;
	CString dialNumberDelayed;
	CString balance;
	UINT callTimer;
	pjsua_acc_config acc_cfg;

	pjsua_transport_id transport_udp_local;
	pjsua_transport_id transport_udp;
	pjsua_transport_id transport_tcp;
	pjsua_transport_id transport_tls;
	pjsua_player_id player_id;
	int audio_input;
	int audio_output;
	int audio_ring;

	BOOL disableAutoRegister;
	CString callIdImcomingIgnore;

	CList<int, int> toneCalls;
	CList<int, int> attendedCalls;
	CList<CString> audioCodecList;
	
	void PJCreate();
	void PJDestroy();
	void PJAccountAdd();
	void PJAccountAddLocal();
	void PJAccountDelete();
	void PJAccountDeleteLocal();
	void PJAccountConfig(pjsua_acc_config *acc_cfg);

	void UpdateWindowText(CString = CString(), int icon = IDI_DEFAULT, bool afterRegister = false);
	void PublishStatus(bool online = true, bool init=false);
	void BaloonPopup(CString title, CString message, DWORD flags = NIIF_WARNING);
	void GotoTab(int i, CTabCtrl* tab = NULL);
	void DialNumberFromCommandLine(CString number);
	void DialNumber(CString params);
	void RingIn(CString filename=CString(), BOOL noLoop = FALSE, BOOL inCall = FALSE);
//	void SetSoundDevice(int outDev);

	void PlayerPlay(CString filename, BOOL noLoop = FALSE, BOOL inCall = FALSE);
	void SetSoundDevice(int outDev, bool forse = 0);
	void PlayerStop();

	BOOL CopyStringToClipboard( IN const CString & str );
	void OnTimerCall (bool manual = false);
	void UsersDirectoryLoad();
	void UpdateVMCount(char * strCount);
	void ShowCallInformation(CString number);

#ifndef _GLOBAL_NO_CONTACTS
	void OnTimerContactBlink();
#endif
	void SetupJumpList();
	void RemoveJumpList();
	void MainPopupMenu();
	void AccountSettingsPendingSave();
	void UpdateSoundDevicesIds();

#ifdef _GLOBAL_VIDEO
	Preview* previewWin;
	int VideoCaptureDeviceId(CString name=_T(""));
#endif

#ifdef _GLOBAL_BALANCE_GTN
	void BalanceGTN();
#endif

#ifdef _GLOBAL_BALANCE_PLAIN
	void BalancePlain();
#endif

#ifdef _GLOBAL_BALANCE_OPTIONS
	void BalanceOption();
#endif

#ifdef _GLOBAL_BALANCE_BEE
	void BalanceBEE();
	void LoginBEE();
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Implementation
protected:
	HICON m_hIcon;
	HICON iconSmall;
	HICON iconInactive;
	NOTIFYICONDATA tnd;
	

	unsigned char m_tabPrev;

	POINT m_mousePos;
	int m_idleCounter;
	BOOL m_isAway;

	HANDLE hThread;
	DWORD ThreadID;

	DWORD ThreadID2;
		
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	// Generated message map functions
	afx_msg LRESULT onTrayNotify(WPARAM, LPARAM);
	afx_msg LRESULT onCreateRingingDlg(WPARAM, LPARAM);
	afx_msg LRESULT onDestroyRingingDlg(WPARAM, LPARAM);
	afx_msg LRESULT OnAccount(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onCallMediaState(WPARAM, LPARAM);

	afx_msg LRESULT onCallState(WPARAM wParam,LPARAM lParam);
#ifndef _GLOBAL_NO_MESSAGING
	afx_msg LRESULT onPager(WPARAM, LPARAM);
	afx_msg LRESULT onPagerStatus(WPARAM, LPARAM);
#endif
#ifndef _GLOBAL_NO_CONTACTS
	afx_msg LRESULT onBuddyState(WPARAM, LPARAM);
#endif
	afx_msg LRESULT onShellExecute(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onDialNumber(WPARAM, LPARAM);
	afx_msg LRESULT CreationComplete(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	boolean isOOO;

	CString iNotesUrl;

	
	std::hash_map<pjsua_call_id, CString> headinfo;

	int INTERNUM = 14;
	CStringArray intOffice; 

	int adminNum = 4;
	CStringArray intAdmin;
	
	CStatusBar m_bar;
	afx_msg LRESULT onPowerBroadcast(WPARAM, LPARAM);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point );
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);

	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT type, int w, int h);
	afx_msg LRESULT onCallAnswer(WPARAM,LPARAM);
	afx_msg LRESULT onCallHangup(WPARAM,LPARAM);
	afx_msg LRESULT onSetPaneText(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onSetVMCountText(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT onPlayerPlay(WPARAM wParam, LPARAM lParam);

#ifndef _GLOBAL_NO_ACCOUNT
	afx_msg void OnMenuAccountAdd();
	afx_msg void OnMenuAccountChange(UINT nID);
	afx_msg void OnMenuAccountEdit(UINT nID);
#endif
	afx_msg void OnMenuSettings();
	afx_msg void OnMenuAlwaysOnTop();
#ifndef _GLOBAL_NO_LOG	
	afx_msg void OnMenuLog();
#endif	
	afx_msg void OnMenuExit();
	afx_msg void OnTimer (UINT TimerVal);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuWebsite();
#if !defined _GLOBAL_CUSTOM || defined _GLOBAL_UPDATES
	afx_msg void CheckUpdates();
#endif
#ifdef _GLOBAL_VIDEO
	afx_msg void createPreviewWin();
#endif
	afx_msg void onVersionUpdate();
	afx_msg void OnHelp();
	boolean isNewAvailable(CString& feature);
	int notifyUpdate();
	//void getHttpResult(CString parturl, CString& result);
	boolean getooo();
	boolean setooo(CString ooo);
	void setooophone(CString phone);
	void getooophone(CString & phone);
	void getooophonelist(CString & phonelist);
	void ShowMessage(CString msg);
	void getiNotesUrl();
	void processurls();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	int isInternalNumber(CString ext);
	int isAdminNumber(CString ext);
	void setOperationLog(CString log);
	void setOperationLogInThread();
protected:
	afx_msg LRESULT OnMywmSetWindowText(WPARAM wParam, LPARAM lParam);
public:
	int getNameListPhones();
	int getAllContactsInThread();
protected:
	afx_msg LRESULT OnUmAllcontactsDone(WPARAM wParam, LPARAM lParam);
public:
	int loadAllContacts();
	int addSearchStringsTo(WPARAM wParam, LPARAM q);
	int getHttpResultInThread(CString url);
};