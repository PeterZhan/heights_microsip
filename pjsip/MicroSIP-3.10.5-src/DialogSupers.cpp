// DialogSupers.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DialogSupers.h"
#include "afxdialogex.h"

#include "const.h"
#include "microsipDlg.h"

// CDialogSupers dialog
static CmicrosipDlg *microsipDlg;
//IMPLEMENT_DYNAMIC(CDialogSupers, CBaseDialog)

CDialogSupers::CDialogSupers(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDialogSupers::IDD, pParent)
{
	microsipDlg = (CmicrosipDlg*)AfxGetMainWnd();
	Create(IDD, pParent);
}

CDialogSupers::~CDialogSupers()
{
}

void CDialogSupers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXP, m_exp);
}


BEGIN_MESSAGE_MAP(CDialogSupers, CBaseDialog)
END_MESSAGE_MAP()


// CDialogSupers message handlers


BOOL CDialogSupers::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	

	AutoMove(IDC_EXP, 0, 0, 100, 100);

	//TranslateDialog(this->m_hWnd);

//	m_exp.GetScrollBarCtrl(SB_HORZ)->ShowWindow(SW_HIDE);


	CString supersUrl = _T(SUPERS_URL);
	supersUrl += L"&local=";
	supersUrl += accountSettings.account.username;
	m_exp.Stop();
	m_exp.Navigate(supersUrl, NULL, NULL, NULL, NULL);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
BEGIN_EVENTSINK_MAP(CDialogSupers, CBaseDialog)
	ON_EVENT(CDialogSupers, IDC_EXP, 250, CDialogSupers::BeforeNavigate2Exp, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CDialogSupers, IDC_EXP, 252, CDialogSupers::NavigateComplete2Exp, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


void CDialogSupers::BeforeNavigate2Exp(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	// TODO: Add your message handler code here

	const wchar_t* cpszProtocolSeparator = L"&";

	const wchar_t* cpszParamSeparator = L"=";

	CString str = URL->bstrVal;
	
	int iPos = str.Find(cpszProtocolSeparator);
	if (iPos == -1)    
		return;

	CString name;
	CString number;

	CString strTarget = str.Mid(iPos + wcslen(cpszProtocolSeparator));


   iPos = strTarget.Find(cpszProtocolSeparator);

	while (iPos != -1){
		CString strParam = strTarget.Mid(0, iPos);
		int iEqual = strParam.Find(cpszParamSeparator);

		if (iEqual != -1){
			CString strParamName = strParam.Mid(0, iEqual);
			CString strParamValue = strParam.Mid(iEqual + wcslen(cpszParamSeparator));

			if (strParamName == "remote")
				number = strParamValue;


			if (strParamName == "name")
				name = strParamValue;

		}


		strTarget = strTarget.Mid(iPos + wcslen(cpszProtocolSeparator));
		iPos = strTarget.Find(cpszProtocolSeparator);

	}

	int iEqual = strTarget.Find(cpszParamSeparator);

	if (iEqual != -1){
		CString strParamName = strTarget.Mid(0, iEqual);
		CString strParamValue = strTarget.Mid(iEqual + wcslen(cpszParamSeparator));

		if (strParamName == "remote")
			number = strParamValue;


		if (strParamName == "name")
			name = strParamValue;

	}


	if (!number.IsEmpty()){
		//microsipDlg->ShowMessage(number + name);
	//	microsipDlg->pageContacts->ContactAdd(number, name, 0, -1, TRUE);


	}

	*Cancel = FALSE;

	
}


void CDialogSupers::NavigateComplete2Exp(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: Add your message handler code here


	


}
