// DlgText.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DlgText.h"
#include "afxdialogex.h"
#include "microsipDlg.h"

static CmicrosipDlg *microsipDlg;
// CDlgText dialog

//IMPLEMENT_DYNAMIC(CDlgText, CDialog)



/* {{{ php_htoi
*/
static int php_htoi(char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}
/* }}} */

/* {{{ URL解码，提取自PHP 5.2.17
用法：string urldecode(string str_source)
时间：2012-8-14 By Dewei
*/
std::string urldecode(std:: string &str_source)
{
	char const *in_str = str_source.c_str();
	int in_str_len = strlen(in_str);
	int out_str_len = 0;
	std::string out_str;
	char *str;

	str = _strdup(in_str);
	char *dest = str;
	char *data = str;

	while (in_str_len--) {
		if (*data == '+') {
			*dest = ' ';
		}
		else if (*data == '%' && in_str_len >= 2 && isxdigit((int)*(data + 1))
			&& isxdigit((int)*(data + 2))) {
			*dest = (char)php_htoi(data + 1);
			data += 2;
			in_str_len -= 2;
		}
		else {
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '\0';
	out_str_len = dest - str;
	out_str = str;
	free(str);
	return out_str;
}
/* }}} */




CDlgText::CDlgText(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgText::IDD, pParent)
	, m_text(_T(""))
{
	this->m_hWnd = NULL;
	microsipDlg = (CmicrosipDlg*)pParent;
	Create(IDD, pParent);
}

CDlgText::~CDlgText()
{
}

void CDlgText::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IESEND, m_iesend);
	//DDX_Control(pDX, IDC_EDITTEXT, m_text);
	DDX_Text(pDX, IDC_EDITTEXT, m_text);
	DDX_Control(pDX, IDC_EDITTEXT, m_ctext);
}


BEGIN_MESSAGE_MAP(CDlgText, CBaseDialog)
	ON_BN_CLICKED(IDC_BTNSEND, &CDlgText::OnBnClickedBtnsend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgText message handlers


void CDlgText::OnBnClickedBtnsend()
{
	// TODO: Add your control notification handler code here
	//CString text = m_text.
	CString parturl;//// = L"cmd=SENDTEXTSUPER&SUPERDOCID=";

	if (strType == L"SUPER")
	{
		parturl = L"cmd=SENDTEXTSUPER&SUPERDOCID=";
		parturl.Append(superDocid);
	}

	if (strType == L"VENDOR")
	{
		parturl = L"cmd=SENDTEXTVENDOR&VENDORDOCID=";
		parturl.Append(vdrDocid);
	}

	parturl.Append(L"&TEXT=");

	CString input;
	m_ctext.GetWindowText(input);
	//input = m_ctext.GetWindowText();
	///CURLEncode encode;

	CT2CA pszConvertedAnsiString(input);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);

//	std::string s((LPCTSTR)input);

	CString cs(urldecode(strStd).c_str());

	// CString cs((strStd).c_str());

	parturl.Append(cs);

	//microsipDlg->ShowMessage(parturl);
	CString URL = _T(GENERAL_URL2);
	URL += parturl;
	m_iesend.Navigate(URL, NULL, NULL, NULL, NULL);

}


void CDlgText::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	microsipDlg->pageDialer->DTMF(L"0");
	CBaseDialog::OnClose();
}
