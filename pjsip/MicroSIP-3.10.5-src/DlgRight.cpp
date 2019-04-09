// DlgRight.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgRight.h"
#include "microsipDlg.h"
#include "settings.h"
#include "DlgProspect.h"

// CDlgRight dialog

//IMPLEMENT_DYNAMIC(CDlgRight, CDialog)
static CmicrosipDlg *microsipDlg;

extern AccountSettings accountSettings;

extern void getHttpResult2(CString parturl, CString& result);

CDlgRight::CDlgRight(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgRight::IDD, pParent)
{
	this->m_hWnd = NULL;
	microsipDlg = (CmicrosipDlg*)pParent;
	Create(IDD, pParent);
}

CDlgRight::~CDlgRight()
{
}

void CDlgRight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNCALLSUPER, m_callSuper);
	DDX_Control(pDX, IDC_BTNSENDLEASE, m_sendLease);
	DDX_Control(pDX, IDC_BTNEBILL, m_ebill);
	DDX_Control(pDX, IDC_BTNTEXT, m_textsuper);
	DDX_Control(pDX, IDC_BTNMOVE, m_moveout);
	//	DDX_Control(pDX, IDC_BTNRENT, m_rent);
	DDX_Control(pDX, IDC_BTNIMAGE, m_image);
	DDX_Control(pDX, IDC_BTNCALLRENEWAL, m_callrenew);
	DDX_Control(pDX, IDC_BTNCALLRENT, m_callrent);
	DDX_Control(pDX, IDC_BTNCALLRENTSTATE, m_callrentstate);
	DDX_Control(pDX, IDC_BTNTAGCOMP, m_complaint);
	DDX_Control(pDX, IDC_EDIT1, m_test);
	DDX_Control(pDX, IDC_BTNCALL3DN, m_3DN);
	DDX_Control(pDX, IDC_BTNPROSPECT, m_prospect);
	DDX_Control(pDX, IDC_BTNPREQCOI, m_coi);
	DDX_Control(pDX, IDC_BTNCHARGE, m_charge);
}


BEGIN_MESSAGE_MAP(CDlgRight, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BTNCALLSUPER, &CDlgRight::OnBnClickedBtncallsuper)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTNSENDLEASE, &CDlgRight::OnBnClickedBtnsendlease)
	ON_BN_CLICKED(IDC_BTNEBILL, &CDlgRight::OnBnClickedBtnebill)
	ON_BN_CLICKED(IDC_BTNTEXT, &CDlgRight::OnBnClickedBtntext)
	ON_BN_CLICKED(IDC_BTNTAGCOMP, &CDlgRight::OnBnClickedBtntagcomp)
	ON_BN_CLICKED(IDC_BTNMOVE, &CDlgRight::OnBnClickedBtnmove)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgRight::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BTNRENT, &CDlgRight::OnBnClickedBtnrent)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgRight::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgRight::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BTNCALLRENEWAL, &CDlgRight::OnBnClickedBtncallrenewal)
	ON_BN_CLICKED(IDC_BTNCALLRENT, &CDlgRight::OnBnClickedBtncallrent)
	ON_BN_CLICKED(IDC_BTNCALLRENTSTATE, &CDlgRight::OnBnClickedBtncallrentstate)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTNIMAGE, &CDlgRight::OnBnClickedBtnimage)
//	ON_BN_CLICKED(IDC_BTNCALLRENTSTATE2, &CDlgRight::OnBnClickedBtncallrentstate2)
	ON_BN_CLICKED(IDC_BTNCALL3DN, &CDlgRight::OnBnClickedBtncall3dn)
	ON_BN_CLICKED(IDC_BTNPROSPECT, &CDlgRight::OnBnClickedBtnprospect)
	ON_BN_CLICKED(IDC_BTNPREQCOI, &CDlgRight::OnBnClickedBtnpreqcoi)
	ON_BN_CLICKED(IDC_BTNCHARGE, &CDlgRight::OnBnClickedBtncharge)
END_MESSAGE_MAP()


// CDlgRight message handlers


void CDlgRight::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);

	CRect cRect;
	microsipDlg->GetWindowRect(&cRect);

	CRect cRectRight;

	this->GetWindowRect(&cRectRight);
	cRectRight.top = cRect.top;// +cRectMessage.Height();
	cRectRight.bottom = cRect.bottom;// +cRectMessage.Height();
	cRectRight.left = cRect.left + cRect.Width();
	cRectRight.right = cRect.right + cRect.Width()/2;

	//if (this->IsWindowVisible() && !this->IsZoomed() && !this->IsIconic())
	this->MoveWindow(&cRectRight);


	

	microsipDlg->pageDialer->m_showbuttons.ShowWindow(SW_HIDE);
	microsipDlg->pageDialer->m_hidebuttons.ShowWindow(SW_SHOW);

	// TODO: Add your message handler code here
}


void CDlgRight::OnMove(int x, int y)
{
	CBaseDialog::OnMove(x, y);

	// TODO: Add your message handler code here

	CRect cRectRight;

	GetWindowRect(&cRectRight);
//	int Mwidth = cRectTop.Width();

	CRect cRect;
	microsipDlg->GetWindowRect(&cRect);
	int MainWidth = cRect.Width();
	int MainHeight = cRect.Height();

	cRect.top = cRectRight.top;// +cRectTop.Height();
	cRect.left = cRectRight.left - MainWidth;// +cRect.Width();

	cRect.bottom = cRectRight.bottom;// +MainHeight;
	cRect.right = cRectRight.right - cRectRight.Width();

	//	cRectMessage.top = cRect.top;
	//   cRectMessage.bottom = cRect.bottom;
	//	cRectMessage.left = cRect.left - Mwidth;
	//	cRectMessage.right = cRectMessage.left + Mwidth;

	if (microsipDlg->IsWindowVisible() && !microsipDlg->IsZoomed() && !microsipDlg->IsIconic())
	{
		microsipDlg->MoveWindow(&cRect);
	}



}


void CDlgRight::OnBnClickedBtncallsuper()
{
	// TODO: Add your control notification handler code here
	microsipDlg->pageDialer->DTMF(L"0");
	CComboBox *combobox = (CComboBox*)microsipDlg->pageDialer->GetDlgItem(IDC_NUMBER);
	combobox->SetWindowText(phoneNum);
	microsipDlg->pageDialer->OnBnClickedCall();




}


void CDlgRight::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	m_callSuper.ShowWindow(SW_HIDE);



	CBaseDialog::OnClose();
}


void CDlgRight::OnBnClickedBtnsendlease()
{
	// TODO: Add your control notification handler code here

	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=SENDLEASERENEWAL&CUSTDOCID=";
	parturl.Append(custDocid);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Sending lease renewal......");

//	parturl.Append(L"&log=");
//	parturl.Append(log);

/*	CString result;
	getHttpResult2(parturl, result);
	microsipDlg->ShowMessage(result);



	if (result.Find(L"true") != -1)
		microsipDlg->ShowMessage(L"Sent Successfully");
	else
		microsipDlg->ShowMessage(L"Sending Failed");

*/

}


void CDlgRight::OnBnClickedBtnebill()
{
	// TODO: Add your control notification handler code here
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=SENDEBILL&CUSTDOCID=";
	parturl.Append(custDocid);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Sending Ebill statement......");
	//	parturl.Append(L"&log=");
	//	parturl.Append(log);

/*	CString result;
	getHttpResult2(parturl, result);
//	microsipDlg->ShowMessage(result);


	if (result.Find(L"true") != -1)
		microsipDlg->ShowMessage(L"Sent Successfully");
	else
		microsipDlg->ShowMessage(L"Sending Failed");

		*/

}


int CDlgRight::ShowIE(CString url, CString tempStr)
{
	
	//microsipDlg->dlgIE->m_ie.set
	WriteHTML(tempStr);
	microsipDlg->dlgIE->ShowWindow(SW_NORMAL);
	microsipDlg->dlgIE->SetWindowText(tempStr);
	// m_ie.Navigate2(L"about:blank", NULL, NULL, NULL, NULL);
	//microsipDlg->dlgIE->m_ie.Refresh();
	microsipDlg->dlgIE->m_ie.Navigate(url, NULL, NULL, NULL, NULL);
	
	
	return 0;
}

void CDlgRight::WriteHTML(const wchar_t* html)
{
	IDispatch* pHtmlDoc = microsipDlg->dlgIE->m_ie.get_Document();
	if (!pHtmlDoc)
		return;
	CComPtr<IHTMLDocument2> doc2;
	doc2.Attach((IHTMLDocument2*)pHtmlDoc);
	if (!doc2)
		return;
	// Creates a new one-dimensional array
	SAFEARRAY* psaStrings = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	if (!psaStrings)
		return;
	BSTR bstr = SysAllocString(html);
	if (bstr)
	{
		VARIANT* param;
		HRESULT hr = SafeArrayAccessData(psaStrings, (LPVOID*)&param);
		if (SUCCEEDED(hr))
		{
			param->vt = VT_BSTR;
			param->bstrVal = bstr;
			hr = SafeArrayUnaccessData(psaStrings);
			if (SUCCEEDED(hr))
			{
				doc2->write(psaStrings);
				doc2->close();
			}
		}
	}
	// SafeArrayDestroy calls SysFreeString for each BSTR!
	if (psaStrings)
		SafeArrayDestroy(psaStrings);
}

void CDlgRight::OnBnClickedBtntext()
{
	// TODO: Add your control notification handler code here

	microsipDlg->pageDialer->DTMF(L"0");
	microsipDlg->dlgText->ShowWindow(SW_NORMAL);


}


void CDlgRight::OnBnClickedBtntagcomp()
{
	// TODO: Add your control notification handler code here
/*	MessagesContact* messagesContact = microsipDlg->messagesDlg->GetMessageContact();
	CString phonenumber = messagesContact->number;
	int pos = phonenumber.Find(_T("@"));

	if (pos != -1)
		phonenumber = phonenumber.Left(pos);*/

	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=TAGTENCOMP";// &PHONENUM = ";
	//parturl.Append(phonenumber);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);
	parturl.Append(L"&CUSTDOCID=");
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Tagging As Tenant Compaint......");




}


void CDlgRight::OnBnClickedBtnmove()
{
	// TODO: Add your control notification handler code here
//	microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=MOVEOUT&CUSTDOCID="; 
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	
	
	ShowIE(URL, L"Create Moveout document......");




}


void CDlgRight::OnBnClickedButton2()
{

//	microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=CALLREPORTOFRPAYMENT&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create call report of rent payment......");

	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtnrent()
{
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=LATESTIMAGE&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create latest check image document......");
	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedButton4()
{
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=CALLREPORTOFLRENEWAL&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create call report of lease renewal......");
	
	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedButton5()
{
	
	//microsipDlg->pageDialer->DTMF(L"0");

	CString parturl = L"cmd=CALLREPORTOFRSTATEMENT&CUSTDOCID=";

	parturl.Append(custDocid);

	parturl.Append(L"&ORGDOCID=");

	parturl.Append(orgDocid);

	parturl.Append(L"&EXT=");

	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create call report of rent statement......");
	
	
	
	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtncallrenewal()
{
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=CALLREPORTOFLRENEWAL&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create call report of lease renewal......");
	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtncallrent()
{
	
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=CALLREPORTOFRPAYMENT&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create call report of rent payment......");
}


void CDlgRight::OnBnClickedBtncallrentstate()
{
	//microsipDlg->pageDialer->DTMF(L"0");

	CString parturl = L"cmd=CALLREPORTOFRSTATEMENT&CUSTDOCID=";

	parturl.Append(custDocid);

	parturl.Append(L"&ORGDOCID=");

	parturl.Append(orgDocid);

	parturl.Append(L"&EXT=");

	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create call report of rent statement......");
	
	// TODO: Add your control notification handler code here
}


BOOL CDlgRight::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
		microsipDlg->ShowMessage(L"Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls, either by hard coded string 
		// or using the string table resource
		m_ToolTip.AddTool(&m_callSuper, _T("Call the Super"));
		//m_ToolTip.AddTool(&m_btnVM, _T("New Voicemail:"));
		m_ToolTip.AddTool(&m_sendLease, _T("Send Lease Renewal"));

		m_ToolTip.AddTool(&m_ebill, _T("Send EBill Statement"));
		m_ToolTip.AddTool(&m_textsuper, _T("Text the Super"));
		m_ToolTip.AddTool(&m_moveout, _T("Tag as Moveout document"));
		m_ToolTip.AddTool(&m_image, _T("Create Call report of check image"));

		m_ToolTip.AddTool(&m_callrenew, _T("Create Call report of Lease renewal"));
		m_ToolTip.AddTool(&m_callrent, _T("Create Call report of rent payment"));

		m_ToolTip.AddTool(&m_callrentstate, _T("Create Call report of rent statement"));

		m_ToolTip.AddTool(&m_complaint, _T("tag as tenant complaint"));

		m_ToolTip.AddTool(&m_3DN, _T("tag as 3DN document"));
		
		m_ToolTip.AddTool(&m_coi, _T("send COI request"));
	//	m_ToolTip.AddTool(&m_btnINotes, _T("iNotes"));


	//	m_ToolTip.AddTool(&m_btnDailyListener, _T("Daily Call Listener"));

	//	m_ToolTip.AddTool(&m_unlock, _T("Unlock the Door"));
		//	CButton m_btnINotes;
		//	CButton m_btnDailyListene

		//m_ToolTip.SetTipBkColor(RGB())

		//	m_ToolTip.AddTool(&m_ButtonDialer6, _T("Open Door"));

		m_ToolTip.Activate(TRUE);
	}



	setItemIcon(IDC_BTNCALLSUPER, IDI_ICONSUPERCALLBIG);// IDI_ICONCALLSUPER);

	setItemIcon(IDC_BTNTEXT, IDI_ICONSUPERTEXTBIG);// IDI_BTNTEXT);
	setItemIcon(IDC_BTNSENDLEASE, IDI_ICONSENDLRBIG);// IDI_BTNSENDLEASE);
	setItemIcon(IDC_BTNEBILL, IDI_ICONLBBIG);// IDI_BTNEBILL);
	setItemIcon(IDC_BTNTAGCOMP, IDI_ICONCOMPBIG);// IDI_ICONTENCOMPBIG);// IDI_ICONTENCOMP);
	setItemIcon(IDC_BTNMOVE, IDI_ICONMOVEBIG);// IDI_ICONMOVEOUT);
	
	setItemIcon(IDC_BTNIMAGE, IDI_BTNIMAGE);
	//setItemIcon(IDC_BTNCALLRENEWAL, IDI_BTNCALLRENEWAL);
	setItemIcon(IDC_BTNCALLRENEWAL, IDI_ICONLRBIG);
	
	setItemIcon(IDC_BTNCALLRENT, IDI_ICONPAYMENTBIG);// IDI_ICONRENTPBIG);// IDI_BTNCALLRENT);
	setItemIcon(IDC_BTNCALLRENTSTATE, IDI_ICONRSTATEBIG);// IDI_BTNCALLRENTSTATE);
	//setItemIcon(IDC_BTNCALL3DN, IDI_ICON3DAYS);
	setItemIcon(IDC_BTNCALL3DN, IDI_ICON3DNBIG);
	
	setItemIcon(IDC_BTNPREQCOI, IDI_ICONCOIBIG);// IDI_ICONCOI);
	

	setItemIcon(IDC_BTNPROSPECT, IDI_ICONPROSPECTBIG);

#if 0
	m_test.Init();
	m_test.SetMode();
	m_test.RemoveSearchAll();
	m_test.AddSearchString(L"dfgdfh");
	m_test.AddSearchString(L"jhgf");
	m_test.AddSearchString(L"vcfgdf");
	m_test.AddSearchString(L"jtyjgh");
	m_test.AddSearchString(L"khjg");

#endif






	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgRight::SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pResult)
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


void CDlgRight::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CBaseDialog::OnMouseMove(nFlags, point);
}


BOOL CDlgRight::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CBaseDialog::PreTranslateMessage(pMsg);
}


int CDlgRight::setItemIcon(int item, int icon)
{
	HICON i;
	i = (HICON)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(icon),
		IMAGE_ICON, 0, 0, LR_SHARED);
	((CButton*)GetDlgItem(item))->SetIcon(i);

	return 0;
}


void CDlgRight::OnBnClickedBtnimage()
{
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=LATESTIMAGE&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create latest check image document......");
	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtncallrentstate2()
{
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=3DAYNOTICECALLREPORT&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create latest 3DN document......");
	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtncall3dn()
{
//	microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=3DAYNOTICECALLREPORT&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create latest 3DN document......");// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtnprospect()
{
	microsipDlg->pageDialer->DTMF(L"0");


	CDlgProspect prosDlg;
	prosDlg.m_aptno = "BUILDING";
	prosDlg.m_code = "";

	// Create and show the dialog box
	INT_PTR nRet = -1;
	nRet = prosDlg.DoModal();

	if (nRet != IDOK)
	{
		microsipDlg->pageDialer->DTMF(L"0");
		return;
	}

	if (prosDlg.m_code.Trim() == "")
	{
		AfxMessageBox(_T("Please input building code!"));
		microsipDlg->pageDialer->DTMF(L"0");
		return;
	}

	// Handle the return value from DoModal 
	/*switch (nRet)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case IDABORT:
		// Do something 
		break;
	case IDOK:
		// Do something 
		break;
	case IDCANCEL:
		// Do something 
		break;
	default:
		// Do something 
		break;
	};*/





	CString parturl = L"cmd=CALLPROSPECT";
	//parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);
	parturl.Append(L"&CODE=");
	parturl.Append(prosDlg.m_code.Trim());
	parturl.Append(L"&APTNO=");
	parturl.Append(prosDlg.m_aptno.Trim());

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create call prospect document......");// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtnpreqcoi()
{
	// TODO: Add your control notification handler code here
	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=SENDCOI";
	//parturl.Append(custDocid);
	parturl.Append(L"&CUSTDOCID=");
	parturl.Append(custDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"send coi request......");// TODO: Add your control notification handler code here
}


void CDlgRight::OnBnClickedBtncharge()
{
	// TODO: Add your control notification handler code here


	//microsipDlg->pageDialer->DTMF(L"0");
	CString parturl = L"cmd=TAGWSCCHARGE&CUSTDOCID=";
	parturl.Append(custDocid);
	parturl.Append(L"&ORGDOCID=");
	parturl.Append(orgDocid);
	parturl.Append(L"&EXT=");
	parturl.Append(accountSettings.account.username);

	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	ShowIE(URL, L"Create latest check image document......");
	// TODO: Add your control notification handler code here


}
