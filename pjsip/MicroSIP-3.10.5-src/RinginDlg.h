#pragma once

#include "resource.h"


#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

#include "PictureEx.h"
#include "afxcmn.h"
#include "afxwin.h"

class RinginDlg: public CDialog
{
public:
	RinginDlg(CWnd* pParent = NULL);	// standard constructor
	~RinginDlg();
	enum { IDD = IDD_RINGIN };
	pjsua_call_id call_id;
	CFont m_font;
	CFont m_font_ignore;
private:
	void CallAccept(BOOL hasVideo = FALSE);
	void Close(BOOL accept = FALSE);
protected:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer (UINT TimerVal);
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAudio();
	afx_msg void OnBnClickedVideo();
	afx_msg void OnBnClickedDecline();
	

	CAnimateCtrl m_animation_gif;
	CAnimateCtrl m_Animator;
	CPictureEx m_picture;
};
