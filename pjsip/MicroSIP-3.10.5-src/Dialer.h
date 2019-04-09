#pragma once

#include "resource.h"
#include "const.h"
#include "BaseDialog.h"
#include "ButtonDialer.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "MyButton.h"
#include "ACEdit.h"

enum DialerActions {
	ACTION_CALL, ACTION_VIDEO_CALL, ACTION_MESSAGE
};

class Dialer :
	public CBaseDialog
{
	CFont m_font;
	CFont m_font_balance;

	CButtonDialer m_ButtonDialer1;
	CButtonDialer m_ButtonDialer2;
	CButtonDialer m_ButtonDialer3;
	CButtonDialer m_ButtonDialer4;
	CButtonDialer m_ButtonDialer5;
	CButtonDialer m_ButtonDialer6;
	CButtonDialer m_ButtonDialer7;
	CButtonDialer m_ButtonDialer8;
	CButtonDialer m_ButtonDialer9;
	CButtonDialer m_ButtonDialer0;
	CButtonDialer m_ButtonDialerStar;
    CButtonDialer m_ButtonDialerGrate;
    CButtonDialer m_ButtonDialerDelete;
    CButtonDialer m_ButtonDialerPlus;
    CButtonDialer m_ButtonDialerClear;

	HICON m_hIconMuteOutput;
	HICON m_hIconMutedOutput;
	HICON m_hIconMuteInput;
	HICON m_hIconMutedInput;
	

	BOOL muteOutput;
	BOOL muteInput;
	
public:

	HICON m_hIconHold;
	HICON m_hIconTransfer;

	HICON m_hIconIdle;
	HICON m_hIconDonot;


	HICON m_hIconHeadSet;
	HICON m_hIconSpeaker;
	HICON m_hIconVoicemail;

	HICON m_hIconUnlock;

	//*****************************************************************
	CToolTipCtrl m_ToolTip;

//*********************************************************************
#ifdef _GLOBAL_VIDEO
	HICON m_hIconVideo;
#endif
#ifndef _GLOBAL_NO_MESSAGING
	HICON m_hIconMessage;
#endif
	Dialer(CWnd* pParent = NULL);	// standard constructor
	~Dialer();
	enum { IDD = IDD_DIALER };

	void Input(CString digits, BOOL disableDTMF = FALSE);
	void DTMF(CString digits, BOOL noLocalDTMF = FALSE);
	void DialedClear();
	void DialedLoad();
	void DialedSave(CComboBox *combobox);
	void SetNumber(CString  number);
	void UpdateCallButton(BOOL forse = FALSE, int callsCount = -1);
	void Action(DialerActions action);
	void Clear(bool update=true);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

#ifdef _GLOBAL_LOGO_DIALER
	HICON m_hIcon;
	BITMAP bm;
	CRect logoRect;
#endif
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCall();
	afx_msg void OnBnClickedVoicemail();
#ifdef _GLOBAL_VIDEO
	afx_msg void OnBnClickedVideoCall();
#endif
#ifndef _GLOBAL_NO_MESSAGING
	afx_msg void OnBnClickedMessage();
#endif
#ifdef _GLOBAL_CALL_TO_SKYPE
	afx_msg void OnBnClickedCallSkype();
	void CallToSkype();
#endif
	afx_msg void OnBnClickedHold();
	afx_msg void OnBnClickedTransfer();
	//afx_msg void OnBnClickedAttdTransfer();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnBnClickedMuteOutput();
	afx_msg void OnBnClickedMuteInput();
	afx_msg void OnBnClickedModeSel();

//	afx_msg void OnCbnEditchangeComboAddr();
	afx_msg void OnCbnSelchangeComboAddr();
	afx_msg void OnBnClickedKey1();
	afx_msg void OnBnClickedKey2();
	afx_msg void OnBnClickedKey3();
	afx_msg void OnBnClickedKey4();
	afx_msg void OnBnClickedKey5();
	afx_msg void OnBnClickedKey6();
	afx_msg void OnBnClickedKey7();
	afx_msg void OnBnClickedKey8();
	afx_msg void OnBnClickedKey9();
	afx_msg void OnBnClickedKeyStar();
	afx_msg void OnBnClickedKey0();
	afx_msg void OnBnClickedKeyGrate();
	afx_msg void OnBnClickedKeyPlus();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnRButtonUp( UINT nFlags, CPoint pt );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint pt );
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt );
	afx_msg void OnVScroll( UINT, UINT, CScrollBar* );
#ifdef _GLOBAL_LOGO_DIALER
	afx_msg void OnPaint();
#endif
	virtual BOOL PreTranslateMessage(MSG* pMsg);

//	CAnimateCtrl m_a1;
	afx_msg void OnBnClickedDontdist();
	afx_msg void OnNMCustomdrawVolumeOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFop2();
	CButton m_btnMode;
	CMyButton m_btnVM;
	CButton m_btnFOP2;
	CButton m_btnDND;
	CButton m_btnMuteMic;
	CButton m_btnMuteSpe;
	CButton m_showCall;
	CButton m_hideCall;
	afx_msg void OnBnClickedShowcall();
	afx_msg void OnBnClickedHidecall();
	afx_msg void OnBnClickedButton2();
	BOOL SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pResult);
	afx_msg void OnBnClickedButton3();
	CButton m_btnOOO;
	afx_msg void OnBnClickedButtondaily();
	afx_msg void OnBnClickedButtoninotes();
	CButton m_btnINotes;
	CButton m_btnDailyListener;
	afx_msg void OnBnClickedButtonooo();
	afx_msg void OnBnClickedButton4();
	CButton m_btnDoor;
	int setKey6ShowOpenDoor();
	int clearKeyShowOpenDoor();
	afx_msg void OnBnClickedBtnunlock();
	CButton m_unlock;
	CButton m_hidebuttons;
	CButton m_showbuttons;
	afx_msg void OnBnClickedHidebuttons();
	afx_msg void OnBnClickedShowbuttons();
	CACEdit m_acedit;
	void trimNoneNums(CString& s, CString & s2);
	afx_msg void OnCbnEditchangeNumber();
	afx_msg void OnEnChangeAcedit();
	afx_msg void OnEnUpdateAcedit();
	afx_msg void OnNMCustomdrawVolumeInput(NMHDR *pNMHDR, LRESULT *pResult);
	int getCurrentDateTime();
};
