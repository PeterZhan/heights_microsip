#include "StdAfx.h"
#include "Transfer.h"
#include "microsipDlg.h"

#include <Windows.h>
#include <WinInet.h>
#pragma comment (lib, "Wininet.lib")
#pragma comment (lib, "urlmon.lib")



#if 0
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <cpprest/json.h>

#include <iostream>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
#endif // 0


static CmicrosipDlg *microsipDlg;

BOOL isDigits(CString number)
{
	BOOL r = TRUE;
	for (int i = 0; i<number.GetLength(); i++)
	{
		if ((number[i]>'9' || number[i]<'0') && number[i] != '*' && number[i] != '#')
		{
			r = FALSE;
			break;
		}
	}

	return r;
}


void Request(int Method, LPCSTR Host, LPCSTR url, LPCSTR header, LPSTR data)
{
	try{
		//Retrieve default http user agent
		char httpUseragent[512];
		DWORD szhttpUserAgent = sizeof(httpUseragent);
		ObtainUserAgentString(0, httpUseragent, &szhttpUserAgent);

		char m[5];

		if (Method == GET)
			strcpy(m, "GET"); 
		else
			strcpy(m, "POST");

		//http://msdn.microsoft.com/en-us/library/windows/desktop/aa385096%28v=vs.85%29.aspx
		HINTERNET internet = InternetOpenA(httpUseragent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (internet != NULL)
		{
			//http://msdn.microsoft.com/en-us/library/windows/desktop/aa384363%28v=vs.85%29.aspx
			HINTERNET connect = InternetConnectA(internet, Host, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
			if (connect != NULL)
			{
				//http://msdn.microsoft.com/en-us/library/windows/desktop/aa384233%28v=vs.85%29.aspx
				HINTERNET request = HttpOpenRequestA(connect, m, url, "HTTP/1.1", NULL, NULL,
					INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
					INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
					INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
					INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
					INTERNET_FLAG_NO_AUTH |
					INTERNET_FLAG_NO_CACHE_WRITE |
					INTERNET_FLAG_NO_UI |
					INTERNET_FLAG_PRAGMA_NOCACHE |
					INTERNET_FLAG_RELOAD, NULL);

				if (request != NULL)
				{
					int datalen = 0;
					if (data != NULL) datalen = strlen(data);
					int headerlen = 0;
					if (header != NULL) headerlen = strlen(header);

					//http://msdn.microsoft.com/en-us/library/windows/desktop/aa384247%28v=vs.85%29.aspx
					HttpSendRequestA(request, header, headerlen, data, datalen);

					//http://msdn.microsoft.com/en-us/library/windows/desktop/aa384350%28v=vs.85%29.aspx
					InternetCloseHandle(request);
				}
			}
			InternetCloseHandle(connect);
		}
		InternetCloseHandle(internet);
	}
	catch (...) {}
}





Transfer::Transfer(CWnd* pParent /*=NULL*/)
: CDialog(Transfer::IDD, pParent)
{
	microsipDlg = (CmicrosipDlg* ) AfxGetMainWnd();	
	Create (IDD, pParent);
}

Transfer::~Transfer(void)
{
	microsipDlg->transferDlg = NULL;
}


BOOL Transfer::OnInitDialog()
{
	CDialog::OnInitDialog();
	TranslateDialog(this->m_hWnd);
	TransLoad();
	return TRUE;
}

void Transfer::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(Transfer, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &Transfer::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &Transfer::OnBnClickedOk)
	ON_EN_CHANGE(IDC_NUMBER1, &Transfer::OnEnChangeNumber1)
END_MESSAGE_MAP()


void Transfer::OnClose() 
{
	DestroyWindow();
}

void Transfer::OnBnClickedCancel()
{
	OnClose();
}


void Transfer::TransLoad()
{
	CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_NUMBER);
	CString key;
	CString val;
	LPTSTR ptr = val.GetBuffer(255);
	int i = 0;
	while (TRUE) {
		key.Format(_T("%d"), i);
		if (GetPrivateProfileString(_T("Transferred"), key, NULL, ptr, 256, accountSettings.iniFile)) {
			combobox->AddString(ptr);
		}
		else {
			break;
		}
		i++;
	}
}

void Transfer::TransSave(CComboBox *combobox)
{
	CString key;
	CString val;
	WritePrivateProfileString(_T("Transferred"), NULL, NULL, accountSettings.iniFile);
	for (int i = 0; i < combobox->GetCount(); i++)
	{
		int n = combobox->GetLBTextLen(i);
		combobox->GetLBText(i, val.GetBuffer(n));
		val.ReleaseBuffer();

		key.Format(_T("%d"), i);
		WritePrivateProfileString(_T("Transferred"), key, val, accountSettings.iniFile);
	}
}

void Transfer::OnBnClickedOk()
{
	MessagesContact* messagesContactSelected = microsipDlg->messagesDlg->GetMessageContact();
	if (messagesContactSelected->callId != -1) {
		CString number;
		CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_NUMBER);
		combobox->GetWindowText(number);
		number.Trim();
		//CString numberFormated = FormatNumber(number);
		//	pj_status_t pj_status = pjsua_verify_sip_url(StrToPj(numberFormated));
		if (isDigits(number)) {

			int pos = combobox->FindStringExact(-1, number);
			if (pos == CB_ERR || pos > 0) {
				if (pos > 0) {
					combobox->DeleteString(pos);
				}
				else if (combobox->GetCount() >= 10)
				{
					combobox->DeleteString(combobox->GetCount() - 1);
				}
				combobox->InsertString(0, number);
				combobox->SetCurSel(0);
			}
			TransSave(combobox);



			if (!number.IsEmpty()) {
				if (attended == 1) {
					CString attdnumber("*2");
					attdnumber += number;
					microsipDlg->pageDialer->DTMF(attdnumber);
					OnClose();

				}
				else if (attended == 2){

					/*	CString url("http://www.heightsre.com/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=CONFERENCETOCELL&exten=");
						url += accountSettings.account.username;
						url += "&cell=";
						url += number;*/


					char URL[1024];
					char* geturi = "/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=CONFERENCETOCELL&exten=%s&cell=%s";
					wsprintfA(URL, geturi, (LPCSTR)(CStringA)(accountSettings.account.username), (LPCSTR)(CStringA)number);

					Request(GET, "www.heightsre.com", URL, NULL, NULL);

					CString log = L"conference with the cell number .";
					log += number;

					microsipDlg->setOperationLog(log);

					OnClose();



#if 0
					CString attdnumber("*6");
					attdnumber += number;
					microsipDlg->pageDialer->DTMF(attdnumber);
					OnClose();

					//	CString url("http://www.heightsre.com/8525760F004BB8DF/generalAgentForAsterisk?openAgent&cmd=CONFERENCETOCELL&exten=");
					//	http_client client(U(url));


					http_client client(U("https://www.heightsre.com/8525760F004BB8DF/generalAgentForAsterisk"));

					// build the query parameters
					auto query = uri_builder()
						.append_query(L"openAgent", L"")
						.append_query(L"cmd", L"CONFERENCETOCELL")
						.append_query(L"exten", (LPCTSTR)(accountSettings.account.username))
						.append_query(L"cell", (LPCTSTR)number)
						.to_string();

					client
						// send the HTTP GET request asynchronous
						.request(methods::GET, query)
#endif // 0
#if 0
						// continue when the response is available
						.then([](http_response response) -> pplx::task<json::value>
					{
						// if the status is OK extract the body of the response into a JSON value
						// works only when the content type is application\json
						if (response.status_code() == status_codes::OK)
						{
							return response.extract_json();
						}

						// return an empty JSON value
						return pplx::task_from_result(json::value());
					})
						// continue when the JSON value is available
						.then([](pplx::task<json::value> previousTask)
					{
						// get the JSON value from the task and display content from it
						try
						{
							json::value const & v = previousTask.get();
							//	print_search_results(v);
						}
						catch (http_exception const & e)
						{
							//	wcout << e.what() << endl;
						}
					}

						)

						.wait();

#endif // 0
					//	OnClose();






				}
				else
				{
					pj_str_t pj_uri = StrToPjStr(GetSIPURI(number, true));
					pjsua_call_xfer(messagesContactSelected->callId, &pj_uri, NULL);
					OnClose();
				}


			}




		}

		else {
			AfxMessageBox(_T("Please input numbers!"));
			//::MessageBox(this->m_hWnd, _T("Please input numbers!"), _T("Input error"), MB_OK);
		}
	}
}



void Transfer::OnEnChangeNumber1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
