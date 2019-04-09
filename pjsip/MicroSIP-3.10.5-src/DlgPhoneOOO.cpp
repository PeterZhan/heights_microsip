// DlgPhoneOOO.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgPhoneOOO.h"
#include "afxdialogex.h"
#include "microsipDlg.h"

// CDlgPhoneOOO dialog


IMPLEMENT_DYNAMIC(CDlgPhoneOOO, CDialog)

static CmicrosipDlg *microsipDlg;

CDlgPhoneOOO::CDlgPhoneOOO(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPhoneOOO::IDD, pParent)
{
	microsipDlg = (CmicrosipDlg*)AfxGetMainWnd();

	


//	m_PhoneList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
}

CDlgPhoneOOO::~CDlgPhoneOOO()
{
}

void CDlgPhoneOOO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTPHONE, m_PhoneList);
}


BEGIN_MESSAGE_MAP(CDlgPhoneOOO, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPhoneOOO::OnBnClickedOk)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDlgPhoneOOO::OnHdnItemclickListphone)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTPHONE, &CDlgPhoneOOO::OnLvnItemchangedListphone)
END_MESSAGE_MAP()


// CDlgPhoneOOO message handlers


int CDlgPhoneOOO::setPhone(CString phoneList, CString phone)
{

	this->phone = phone;
	this->phonelist = phoneList;
#if 0
	microsipDlg->ShowMessage(phoneList);
	CListCtrl *m_PhoneList = (CListCtrl*)GetDlgItem(IDC_LISTPHONE);
	microsipDlg->ShowMessage(phoneList);
	//	m_PhoneList = *list;
	//	microsipDlg->ShowMessage(phoneList);

	//::MessageBox(m_hWnd, phoneList, L"test", MB_OK);

	//	m_PhoneList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	//	m_PhoneList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	/*	LONG lStyle;
		lStyle = GetWindowLong(m_PhoneList->m_hWnd, GWL_STYLE);//获取当前窗口style
		lStyle&= ~LVS_TYPEMASK; //清除显示方式位
		lStyle|= LVS_REPORT; //设置style
		SetWindowLong(m_PhoneList->m_hWnd, GWL_STYLE, lStyle);//设置style*/

	DWORD dwStyle;// = m_PhoneList->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_PhoneList->SetExtendedStyle(dwStyle); //设置扩展风格


	microsipDlg->ShowMessage(phoneList);

	m_PhoneList->InsertColumn(0, TEXT("Type"), LVCFMT_LEFT, 40);
	microsipDlg->ShowMessage(phoneList);

	m_PhoneList->InsertColumn(1, TEXT("Number"), LVCFMT_LEFT, 40);

	microsipDlg->ShowMessage(phoneList);


	int nTokenPos = 0;
	CString strToken = phoneList.Tokenize(_T(","), nTokenPos);

	while (!strToken.IsEmpty())
	{
		//	::MessageBox(m_hWnd, strToken, L"test", MB_OK);
		//microsipDlg->ShowMessage(strToken);
		int nRow = -1;

		int p = 0;
		CString s = strToken.Tokenize(_T(":"), p);
		if (!s.IsEmpty()){
			nRow = m_PhoneList->InsertItem(0, s);
			s = strToken.Tokenize(_T(":"), p);
			if (!s.IsEmpty())
				m_PhoneList->SetItemText(nRow, 1, s);

		}


		/*LVITEM lvItem;

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = 0;
		/*	char* text = "Sandra C. Anschwitz";
		wchar_t wtext[50];
		mbstowcs(wtext, text, strlen(text) + 1);
		LPWSTR ptr = wtext;*/
		/*	lvItem.pszText = strToken.GetBuffer();
			strToken.ReleaseBuffer();*/
		//	int nRow = m_PhoneList.InsertItem(0, strToken);

		if (strToken == phone && nRow != -1)
			m_PhoneList->SetCheck(nRow);

		//	m_PhoneList.SetItemText(nRow, 1, TEXT("Item_2"));




		strToken = phoneList.Tokenize(_T(","), nTokenPos);
	}
	 


#endif // 0

	//m_PhoneList.UpdateWindow();

	return 0;
}


void CDlgPhoneOOO::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here


	//microsipDlg->ShowMessage(phone);
	for (int i = 0; i<m_PhoneList.GetItemCount(); i++)

	{

		if (m_PhoneList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)

		{
			CString str;
			str.Format(L"%d", i);
			
		//	microsipDlg->ShowMessage(str);
			TCHAR szBuffer[MAX_PATH] = TEXT("");
			LVITEM lvi;

			lvi.iItem = i;

			lvi.mask = LVIF_TEXT;
			lvi.cchTextMax = MAX_PATH;// CountArray(szBuffer);

			lvi.iSubItem = 0;

			lvi.pszText = szBuffer;

			m_PhoneList.GetItem(&lvi);

			phone.Empty();
			phone.Append(szBuffer);

		//	microsipDlg->ShowMessage(phone);

			break;

		}

	}



//	CString log = L"enabled out of office.";
//	microsipDlg->setOperationLog(log);






	CDialog::OnOK();
}


BOOL CDlgPhoneOOO::OnInitDialog()
{
	CDialog::OnInitDialog();




		LONG lStyle;
	lStyle = GetWindowLong(m_PhoneList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle&= ~LVS_TYPEMASK; //清除显示方式位
	lStyle|= LVS_REPORT; //设置style
	SetWindowLong(m_PhoneList.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_PhoneList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
//	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_PhoneList.SetExtendedStyle(dwStyle); //设置扩展风格


	//microsipDlg->ShowMessage(phonelist);

	m_PhoneList.InsertColumn(0, TEXT("Phone Number"), LVCFMT_LEFT, 350);
	//microsipDlg->ShowMessage(phoneList);

//	m_PhoneList.InsertColumn(1, TEXT("Number"), LVCFMT_LEFT, 250);

	//microsipDlg->ShowMessage(phoneList);


	int nTokenPos = 0;
	CString strToken = phonelist.Tokenize(_T(","), nTokenPos);

	while (!strToken.IsEmpty())
	{
		//	::MessageBox(m_hWnd, strToken, L"test", MB_OK);
		//microsipDlg->ShowMessage(strToken);
		CString fullphone = strToken;

		int nRow = m_PhoneList.InsertItem(0, fullphone);
		/*
		int p = 0;
		CString s = fullphone.Tokenize(_T(":"), p);
		if (!s.IsEmpty()){
			nRow = m_PhoneList.InsertItem(0, s);
			s = fullphone;
			if (!s.IsEmpty())
				m_PhoneList.SetItemText(nRow, 1, s);

		}

		*/
		
	//	microsipDlg->ShowMessage(fullphone + " " + phone);
		if (fullphone == phone && nRow != -1){
			//SetItemState(prev_item, ~LVIS_SELECTED, LVIS_SELECTED);
			m_PhoneList.SetItemState(nRow, LVIS_SELECTED, LVIS_SELECTED);
			m_PhoneList.SetSelectionMark(nRow);

		}
			//m_PhoneList.setselectedit

		//	m_PhoneList.SetItemText(nRow, 1, TEXT("Item_2"));




		strToken = phonelist.Tokenize(_T(","), nTokenPos);
	}







	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPhoneOOO::OnHdnItemclickListphone(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	microsipDlg->ShowMessage(phone);
	for (int i = 0; i<m_PhoneList.GetItemCount(); i++)

	{

		if (m_PhoneList.GetCheck(i))

		{
			microsipDlg->ShowMessage(phone);
			TCHAR szBuffer[MAX_PATH] = TEXT("");
			LVITEM lvi;

			lvi.iItem = i;

			lvi.mask = LVIF_TEXT;
			lvi.cchTextMax = MAX_PATH;// CountArray(szBuffer);

			lvi.iSubItem = 0;

			lvi.pszText = szBuffer;

			m_PhoneList.GetItem(&lvi);

			phone.Empty();
			phone.Append(szBuffer);

			microsipDlg->ShowMessage(phone);

			break;

		}

	}
	*/
	
	
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CDlgPhoneOOO::OnLvnItemchangedListphone(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
