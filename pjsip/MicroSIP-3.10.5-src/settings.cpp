#include "stdafx.h"
#include "settings.h"
#include "Crypto.h"
#include "AES.h"

using namespace MFC;

AccountSettings accountSettings;
bool firstRun;
bool pj_ready;



void AccountSettings::Init()
{
	CString str;
	LPTSTR ptr;

	

	accountId = 0;
	//--
	logFile.Format(_T("%s.log"), _T(_GLOBAL_NAME));
	iniFile.Format(_T("%s.ini"), _T(_GLOBAL_NAME));
	pathRoaming=_T("");
#ifdef _GLOBAL_CUSTOM_INI
	CRegKey regKey;
	CString rab;
	ULONG pnChars;
	rab.Format(_T("Software\\%s"), _T(_GLOBAL_NAME));
	if (regKey.Open(HKEY_CURRENT_USER,rab,KEY_READ)==ERROR_SUCCESS) {
		ptr = str.GetBuffer(255);
		pnChars = 256;
		regKey.QueryStringValue(_T("iniPath"),ptr,&pnChars);
		str.ReleaseBuffer();
		regKey.Close();
	}
	if (!str.IsEmpty()) {
		if (str.Mid(str.GetLength()-1)!=_T("\\")) {
			str += _T("\\");
		}
		iniFile = str + iniFile;
		logFile = str + logFile;
		pathRoaming = str;
	}
#endif
	if (pathRoaming.IsEmpty()) {
		CFileStatus rStatus;
		CString appData;
		ptr = appData.GetBuffer(MAX_PATH);
		::GetCurrentDirectory(MAX_PATH, ptr);
		appData.ReleaseBuffer();
		appData += _T("\\");

		CString appDataLocal;
		ptr = appDataLocal.GetBuffer(MAX_PATH);
		SHGetSpecialFolderPath(
			0,
			ptr, 
			CSIDL_LOCAL_APPDATA, 
			FALSE ); 
		appDataLocal.ReleaseBuffer();
		appDataLocal.AppendFormat(_T("\\%s\\"),_T(_GLOBAL_NAME));

		CString appDataRoaming;
		ptr = appDataRoaming.GetBuffer(MAX_PATH);
		SHGetSpecialFolderPath(
			0,
			ptr, 
			CSIDL_APPDATA,
			FALSE ); 
		appDataRoaming.ReleaseBuffer();
		appDataRoaming.AppendFormat(_T("\\%s\\"),_T(_GLOBAL_NAME));

		firstRun = true;
		if (CFile::GetStatus(appData + iniFile, rStatus)) {
			firstRun = false;
		}

		HANDLE h = CreateFile( appData + iniFile, GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ |
			FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		if (h != INVALID_HANDLE_VALUE )
		{
			LARGE_INTEGER filesize;
			// copy ini from appdata to workdir
			if (::GetFileSizeEx(h, &filesize) && filesize.LowPart==0 ) {
				CloseHandle(h);
				CopyFile(appDataRoaming + iniFile, appData + iniFile,  FALSE);
			} else {
				CloseHandle(h);
			}
			iniFile = appData + iniFile;
			logFile = appData + logFile;
			pathRoaming = appData;
		} else {
			CreateDirectory(appDataRoaming, NULL);
			CreateDirectory(appDataLocal, NULL);
			if (CFile::GetStatus(appDataRoaming + iniFile, rStatus)) {
				firstRun = false;
			} else {
				// move ini file from old location
				MoveFile(appDataLocal + iniFile, appDataRoaming + iniFile);
				// copy ini from workdir to appdata
				CopyFile(appData + iniFile, appDataRoaming + iniFile,  TRUE);
			}
			iniFile = appDataRoaming + iniFile;
			logFile = appDataLocal + logFile;
			pathRoaming = appDataRoaming;
		}
	}
	//--
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("accountId"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	if (str.IsEmpty()) {
		if (AccountLoad(-1,account)) {
			accountId = 1;
			WritePrivateProfileString(_T("Settings"),_T("accountId"), _T("1"), iniFile);
		}
	} else {
		accountId=atoi(CStringA(str));
		if (accountId>0) {
			if (!AccountLoad(accountId,account)) {
				accountId = 0;
			}
		}
	}

#if !defined _GLOBAL_CUSTOM || defined _GLOBAL_UPDATES
	ptr = updatesInterval.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("updatesInterval"), NULL, ptr, 256, iniFile);
	updatesInterval.ReleaseBuffer();
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("checkUpdatesTime"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	checkUpdatesTime = atoi(CStringA(str));
#endif

#ifndef _GLOBAL_ACCOUNT_MINI
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("disableLocalAccount"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	disableLocalAccount=str==_T("1")?1:0;
#else
	disableLocalAccount=1;
#endif

#ifndef _GLOBAL_AUTO_ANSWER_VALUE
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("autoAnswer"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
#ifdef _GLOBAL_AUTO_ANSWER_DEFAULT
	autoAnswer=str==_T("1")?1:(str==_T("2")?2:(str==_T("0")?0:_GLOBAL_AUTO_ANSWER_DEFAULT));
#else
	autoAnswer=str==_T("1")?1:(str==_T("2")?2:0);
#endif
#else
	autoAnswer=_GLOBAL_AUTO_ANSWER_VALUE;
#endif

	ptr = userAgent.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("userAgent"), NULL, ptr, 256, iniFile);
	userAgent.ReleaseBuffer();

	ptr = denyIncoming.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("denyIncoming"), NULL, ptr, 256, iniFile);
	denyIncoming.ReleaseBuffer();

#ifndef _GLOBAL_NO_CONTACTS
#ifdef _GLOBAL_USERS_DIRECTORY_VALUE
	usersDirectory=_T(_GLOBAL_USERS_DIRECTORY_VALUE);
#else
#ifndef _GLOBAL_ACCOUNT_MINI
	ptr = usersDirectory.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("usersDirectory"), NULL, ptr, 256, iniFile);
	usersDirectory.ReleaseBuffer();
#endif
#endif
#endif

#ifndef _GLOBAL_SOUND_EVENTS
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("localDTMF"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
#ifdef _GLOBAL_SOUND_EVENTS_VALUE
	localDTMF=str==_T("0")?0:(str==_T("1")?1:_GLOBAL_SOUND_EVENTS_VALUE);
#else
	localDTMF=str==_T("0")?0:1;
#endif
#else
	localDTMF = _GLOBAL_SOUND_EVENTS;
#endif

	ptr = ringingSound.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("ringingSound"), _T("ringin.wav"), ptr, 256, iniFile);
	ringingSound.ReleaseBuffer();

	ptr = audioInputDevice.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("audioInputDevice"), NULL, ptr, 256, iniFile);
	audioInputDevice.ReleaseBuffer();


	ptr = audioOutputDevice.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("audioOutputDevice"), NULL, ptr, 256, iniFile);
	audioOutputDevice.ReleaseBuffer();

	ptr = audioInputDevice_1.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"), _T("audioInputDevice_1"), NULL, ptr, 256, iniFile);
	audioInputDevice_1.ReleaseBuffer();


	ptr = audioOutputDevice_1.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"), _T("audioOutputDevice_1"), NULL, ptr, 256, iniFile);
	audioOutputDevice_1.ReleaseBuffer();

	if (s == 0)
	{
		audioOutputDevice_s = audioOutputDevice;
		audioInputDevice_s = audioInputDevice;
	}

	if (s == 1)
	{
		audioOutputDevice_s = audioOutputDevice_1;
		audioInputDevice_s = audioInputDevice_1;
	}



	ptr = audioRingDevice.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("audioRingDevice"), NULL, ptr, 256, iniFile);
	audioRingDevice.ReleaseBuffer();
	
#ifndef _GLOBAL_CODECS_HARDCODED
	ptr = audioCodecs.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("audioCodecs"), NULL, ptr, 256, iniFile);
	audioCodecs.ReleaseBuffer();
#else
	audioCodecs = _T(_GLOBAL_CODECS_HARDCODED);
#endif

#ifndef _GLOBAL_NO_VAD
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("VAD"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	vad = str == "1" ? 1 : 0;

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("EC"), _T("1"), ptr, 256, iniFile);
	str.ReleaseBuffer();
//	ec = 1;
//	if (str == "0") ec = 0;

	ec = str == "1" ? 1 : 0;
	ec = 1;

/*	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"), _T("STUNCHECK"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	stuncheck = str == "1" ? 1 : 0;


	if (stuncheck)
		account.stun =  _T(STUN_SERVER);
	else
		account.stun =  _T("");
	*/

	account.stun =  _T("");
#ifdef _GLOBAL_EC_VALUE
	ec = str==_T("1")?1:(str==_T("0")?0:_GLOBAL_EC_VALUE);
#else

//	ec = 1;
//	if (str == "0") ec = 0;

//	ec = str==_T("1")?1:0;

	//ec = 1;
#endif

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("forceCodec"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
#else
	vad = 0;
	ec = 1;
#endif

#ifdef _GLOBAL_FORCE_CODEC_CHECKED
	forceCodec = str == "0" ? 0 : 1;
#else
	forceCodec = str == "1" ? 1 : 0;
#endif

#ifdef _GLOBAL_VIDEO
	ptr = videoCaptureDevice.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("videoCaptureDevice"), NULL, ptr, 256, iniFile);
	videoCaptureDevice.ReleaseBuffer();
	ptr = videoCodec.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("videoCodec"), NULL, ptr, 256, iniFile);
	videoCodec.ReleaseBuffer();
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("disableH264"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	disableH264 = str == "1" ? 1 : 0;
	ptr = bitrateH264.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("bitrateH264"), NULL, ptr, 256, iniFile);
	bitrateH264.ReleaseBuffer();
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("disableH263"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	disableH263 = str == "1" ? 1 : 0;
	ptr = bitrateH263.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("bitrateH263"), NULL, ptr, 256, iniFile);
	bitrateH263.ReleaseBuffer();
#endif

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("mainX"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainX = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("mainY"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainY = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("mainW"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainW = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("mainH"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainH = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("messagesX"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesX = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("messagesY"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesY = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("messagesW"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesW = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("messagesH"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesH = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("callsWidth0"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth0 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("callsWidth1"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth1 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("callsWidth2"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth2 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("callsWidth3"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth3 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("callsWidth4"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth4 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("volumeOutput"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	volumeOutput = str.IsEmpty()?100:atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("volumeInput"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
#ifndef _GLOBAL_VOLUME_MIC
	volumeInput = str.IsEmpty()?100:atoi(CStringA(str));
#else
	volumeInput = str.IsEmpty()?_GLOBAL_VOLUME_MIC:atoi(CStringA(str));
#endif

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("activeTab"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	activeTab = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("alwaysOnTop"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	alwaysOnTop = atoi(CStringA(str));

	ptr = cmdIncomingCall.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("cmdIncomingCall"), NULL, ptr, 256, iniFile);
	cmdIncomingCall.ReleaseBuffer();

	ptr = cmdCallAnswer.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("cmdCallAnswer"), NULL, ptr, 256, iniFile);
	cmdCallAnswer.ReleaseBuffer();

#ifndef _GLOBAL_NO_LOG
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(_T("Settings"),_T("enableLog"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	enableLog=str==_T("1")?1:0;
#else
	enableLog=0;
#endif

#ifndef _GLOBAL_SINGLE_MODE
	ptr = str.GetBuffer(255);
#ifdef _GLOBAL_EXTENDED_MODE
	GetPrivateProfileString(_T("Settings"),_T("singleMode"), _T("0"), ptr, 256, iniFile);
#else
	GetPrivateProfileString(_T("Settings"),_T("singleMode"), NULL, ptr, 256, iniFile);
#endif
	str.ReleaseBuffer();
  #ifndef _GLOBAL_MULTI_MODE
	singleMode=str==_T("0")?0:1;
  #else
	singleMode = !_GLOBAL_MULTI_MODE;
	
  #endif
#else
	singleMode = 1;
#endif

	hidden = 0;


	

}

void AccountSettings::ModeChange()
{
	if (s == 0)
	{
		audioOutputDevice_s = audioOutputDevice;
		audioInputDevice_s = audioInputDevice;
	}

	if (s == 1)
	{
		audioOutputDevice_s = audioOutputDevice_1;
		audioInputDevice_s = audioInputDevice_1;
	}
}
AccountSettings::AccountSettings()
{
	Init();
}

void AccountSettings::AccountDelete(int id)
{
	CString section;
	section.Format(_T("Account%d"),id);
	WritePrivateProfileStruct(section, NULL, NULL, 0, iniFile);
}

bool AccountSettings::AccountLoad(int id, Account &account)
{
	CString str;
	CString rab;
	LPTSTR ptr;

	CString section;
	if (id==-1) {
		section = _T("Settings");
	} else {
		section.Format(_T("Account%d"),id);
	}

#ifdef _GLOBAL_ACCOUNT_SIP_SERVER
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = account.server.GetBuffer(255);
		GetPrivateProfileString(section,_T("server"), _T(_GLOBAL_ACCOUNT_SIP_SERVER), ptr, 256, iniFile);
		account.server.ReleaseBuffer();		
	#else
		account.server = _T(_GLOBAL_ACCOUNT_SIP_SERVER);
	#endif
#else
	ptr = account.server.GetBuffer(255);
	GetPrivateProfileString(section,_T("server"), NULL, ptr, 256, iniFile);
	account.server.ReleaseBuffer();
#endif

#ifdef _GLOBAL_ACCOUNT_SIP_PROXY
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = account.proxy.GetBuffer(255);
		GetPrivateProfileString(section,_T("proxy"), _T(_GLOBAL_ACCOUNT_SIP_PROXY), ptr, 256, iniFile);
		account.proxy.ReleaseBuffer();		
	#else
		account.proxy = _T(_GLOBAL_ACCOUNT_SIP_PROXY);
	#endif
#else
	ptr = account.proxy.GetBuffer(255);
	GetPrivateProfileString(section,_T("proxy"), NULL, ptr, 256, iniFile);
	account.proxy.ReleaseBuffer();
#endif

#if defined _GLOBAL_ACCOUNT_DOMAIN && !defined _GLOBAL_PROFILE
#ifdef _GLOBAL_ACCOUNT_DOMAIN_EQ_SERVER
	account.domain = account.server;
#else
#ifdef _GLOBAL_ACCOUNT_OVERRIDE
	ptr = account.domain.GetBuffer(255);
	GetPrivateProfileString(section,_T("domain"), _T(_GLOBAL_ACCOUNT_DOMAIN), ptr, 256, iniFile);
	account.domain.ReleaseBuffer();		
#else
	account.domain = _T(_GLOBAL_ACCOUNT_DOMAIN);
#endif
#endif
#else
	ptr = account.domain.GetBuffer(255);
	GetPrivateProfileString(section,_T("domain"), NULL, ptr, 256, iniFile);
	account.domain.ReleaseBuffer();
#ifdef _GLOBAL_PROFILE
	if (account.domain==_T("B")) {
		account.domain = _T(_GLOBAL_PROFILE_B);
#ifdef _GLOBAL_PROFILE_C
	} else if (account.domain==_T("C")) {
		account.domain = _T(_GLOBAL_PROFILE_C);
#endif
	} else {
		account.domain = _T(_GLOBAL_PROFILE_A);
	}
	account.server=account.domain;
	account.proxy=account.domain;
#endif
#endif

#ifdef _GLOBAL_ACCOUNT_LOGIN
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = account.authID.GetBuffer(255);
		GetPrivateProfileString(section,_T("authID"), _T(_GLOBAL_ACCOUNT_LOGIN), ptr, 256, iniFile);
		account.authID.ReleaseBuffer();		
	#else
		account.authID = _T(_GLOBAL_ACCOUNT_LOGIN);
	#endif
#else
	ptr = account.authID.GetBuffer(255);
	GetPrivateProfileString(section,_T("authID"), NULL, ptr, 256, iniFile);
	account.authID.ReleaseBuffer();
#endif

	CString usernameLocal;
	CString passwordLocal;

#ifdef _GLOBAL_ACCOUNT_USERNAME
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = usernameLocal.GetBuffer(255);
		GetPrivateProfileString(section,_T("username"), _T(_GLOBAL_ACCOUNT_USERNAME), ptr, 256, iniFile);
		usernameLocal.ReleaseBuffer();		
	#else
		usernameLocal = _T(_GLOBAL_ACCOUNT_USERNAME);
	#endif
#else
	ptr = usernameLocal.GetBuffer(255);
	GetPrivateProfileString(section,_T("username"), NULL, ptr, 256, iniFile);
	usernameLocal.ReleaseBuffer();
#endif

#ifdef _GLOBAL_ACCOUNT_PASSWORD
	passwordLocal = _T(_GLOBAL_ACCOUNT_PASSWORD);
#else
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("passwordSize"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	CByteArray arPassword;
//	CCrypto crypto;

	unsigned char key[] = "sdf%^^7dk66JGdfj%4*JGHhn";
	AES aes(key);

	if (!str.IsEmpty()) {
		int size = atoi(CStringA(str));
		arPassword.SetSize(size>0?size:16);
		GetPrivateProfileStruct(section,_T("password"), arPassword.GetData(),arPassword.GetSize(), iniFile);
	//	if (crypto.DeriveKey((LPCTSTR)_GLOBAL_KEY)) {
		if (true){
		//	if (!crypto.Decrypt(arPassword,passwordLocal)) {
		if (!aes.InvCipher(arPassword, passwordLocal)) {
				str = _T("");
			}
		}
	}
	if (str.IsEmpty()){
		ptr = passwordLocal.GetBuffer(255);
		GetPrivateProfileString(section,_T("password"), NULL, ptr, 256, iniFile);
		passwordLocal.ReleaseBuffer();
	//	if (!passwordLocal.IsEmpty() && crypto.DeriveKey((LPCTSTR)_GLOBAL_KEY)) {
		//crypto.Encrypt(passwordLocal, arPassword);
		if (!passwordLocal.IsEmpty()) {
			aes.Cipher(passwordLocal, arPassword);
			if (arPassword.GetSize()) {
				WritePrivateProfileStruct(section,_T("password"),arPassword.GetData(),arPassword.GetSize(),iniFile);
				CString str;
				str.Format(_T("%d"),arPassword.GetSize());
				WritePrivateProfileString(section,_T("passwordSize"),str,iniFile);
			}
		}
	}
#endif

#ifndef _GLOBAL_ACCOUNT_PASSWORD
#ifndef _GLOBAL_ACCOUNT_REMEMBER_PASSWORD_CHECKED
	account.rememberPassword = passwordLocal.GetLength()?1:0;
#else
	account.rememberPassword = !usernameLocal.GetLength() || passwordLocal.GetLength()?1:0;
#endif
#else
	account.rememberPassword = usernameLocal.GetLength()?1:0;
#endif


#ifndef _GLOBAL_API
	account.username = usernameLocal;
	account.password = passwordLocal;
	
	
#else
	account.username = _T("");
	account.password = _T("");
	account.apiLogin = usernameLocal;
	account.apiPassword = passwordLocal;
#endif


#ifdef _GLOBAL_HOST_BASED
	DWORD size;
	size = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(account.username.GetBuffer(size),&size);
	account.username.ReleaseBuffer();
	account.password = "12345";
#endif

#ifdef _GLOBAL_API_ID
	ptr = account.apiId.GetBuffer(255);
	GetPrivateProfileString(section,_T("id"), NULL, ptr, 256, iniFile);
	account.apiId.ReleaseBuffer();
#endif

#ifdef _GLOBAL_ACCOUNT_NAME
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = account.displayName.GetBuffer(255);
		GetPrivateProfileString(section,_T("displayName"), _T(_GLOBAL_ACCOUNT_NAME), ptr, 256, iniFile);
		account.displayName.ReleaseBuffer();		
	#else
		account.displayName = _T(_GLOBAL_ACCOUNT_NAME);
	#endif
#else
	ptr = account.displayName.GetBuffer(255);
	GetPrivateProfileString(section,_T("displayName"), NULL, ptr, 256, iniFile);
	account.displayName.ReleaseBuffer();
#endif

#ifdef _GLOBAL_ACCOUNT_SRTP
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = account.srtp.GetBuffer(255);
		GetPrivateProfileString(section,_T("SRTP"), _T(_GLOBAL_ACCOUNT_SRTP), ptr, 256, iniFile);
		account.srtp.ReleaseBuffer();		
	#else
		account.srtp = _T(_GLOBAL_ACCOUNT_SRTP);
	#endif
#else
	ptr = account.srtp.GetBuffer(255);
	GetPrivateProfileString(section,_T("SRTP"), NULL, ptr, 256, iniFile);
	account.srtp.ReleaseBuffer();
#endif

#ifdef _GLOBAL_ACCOUNT_TRANSPORT
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = account.transport.GetBuffer(255);
		GetPrivateProfileString(section,_T("transport"), _T(_GLOBAL_ACCOUNT_TRANSPORT), ptr, 256, iniFile);
		account.transport.ReleaseBuffer();		
	#else
		account.transport = _T(_GLOBAL_ACCOUNT_TRANSPORT);
	#endif
#else
	ptr = account.transport.GetBuffer(255);
	GetPrivateProfileString(section,_T("transport"), NULL, ptr, 256, iniFile);
	account.transport.ReleaseBuffer();
#endif

#ifndef _GLOBAL_ACCOUNT_MINI
	ptr = account.publicAddr.GetBuffer(255);
	GetPrivateProfileString(section,_T("publicAddr"), NULL, ptr, 256, iniFile);
	account.publicAddr.ReleaseBuffer();
#endif

#ifndef _GLOBAL_ACCOUNT_MINI
	ptr = account.listenPort.GetBuffer(255);
	GetPrivateProfileString(section,_T("listenPort"), NULL, ptr, 256, iniFile);
	account.listenPort.ReleaseBuffer();
#endif

#ifdef _GLOBAL_ACCOUNT_PUBLISH
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = str.GetBuffer(255);
		rab.Format(_T("%d"),_GLOBAL_ACCOUNT_PUBLISH);
		GetPrivateProfileString(section,_T("publish"), rab, ptr, 256, iniFile);
		str.ReleaseBuffer();		
		account.publish=str==_T("1")?1:0;
	#else
		account.publish=_GLOBAL_ACCOUNT_PUBLISH;
	#endif
#else
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("publish"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	account.publish=str==_T("1")?1:0;
#endif


#ifdef _GLOBAL_ACCOUNT_STUN
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = account.stun.GetBuffer(255);
		GetPrivateProfileString(section,_T("STUN"), _T(_GLOBAL_ACCOUNT_STUN), ptr, 256, iniFile);
		account.stun.ReleaseBuffer();		
	#else
		account.stun = _T(_GLOBAL_ACCOUNT_STUN);
	#endif
#else
	ptr = account.stun.GetBuffer(255);
	GetPrivateProfileString(section,_T("STUN"), NULL, ptr, 256, iniFile);
	account.stun.ReleaseBuffer();
#endif

#ifdef _GLOBAL_ACCOUNT_ICE
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = str.GetBuffer(255);
		rab.Format(_T("%d"),_GLOBAL_ACCOUNT_ICE);
		GetPrivateProfileString(section,_T("ICE"), rab, ptr, 256, iniFile);
		str.ReleaseBuffer();		
		account.ice=str==_T("1")?1:0;
	#else
		account.ice=_GLOBAL_ACCOUNT_ICE;
	#endif
#else
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("ICE"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	account.ice=str==_T("1")?1:0;
#endif

#ifdef _GLOBAL_ACCOUNT_ALLOW_REWRITE
	#ifdef _GLOBAL_ACCOUNT_OVERRIDE
		ptr = str.GetBuffer(255);
		rab.Format(_T("%d"),_GLOBAL_ACCOUNT_ALLOW_REWRITE);
		GetPrivateProfileString(section,_T("allowRewrite"), rab, ptr, 256, iniFile);
		str.ReleaseBuffer();		
		account.allowRewrite=str==_T("1")?1:0;
	#else
		account.allowRewrite=_GLOBAL_ACCOUNT_ALLOW_REWRITE;
	#endif
#else
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("allowRewrite"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	account.allowRewrite=str==_T("1")?1:0;
#endif

	account.stun = _T("");// _T(STUN_SERVER);

	account.ice = 0;

//	account.allowRewrite = 1;



	if (id==-1) {
		// delete old
		WritePrivateProfileString(section,_T("server"), NULL, iniFile);
		WritePrivateProfileString(section,_T("proxy"), NULL, iniFile);
		WritePrivateProfileString(section,_T("SRTP"), NULL, iniFile);
		WritePrivateProfileString(section,_T("transport"), NULL, iniFile);
		WritePrivateProfileString(section,_T("publicAddr"), NULL, iniFile);
		WritePrivateProfileString(section,_T("listenPort"), NULL, iniFile);
		WritePrivateProfileString(section,_T("publish"), NULL, iniFile);
		WritePrivateProfileString(section,_T("STUN"), NULL, iniFile);
		WritePrivateProfileString(section,_T("ICE"), NULL, iniFile);
		WritePrivateProfileString(section,_T("allowRewrite"), NULL, iniFile);
		WritePrivateProfileString(section,_T("domain"), NULL, iniFile);
		WritePrivateProfileString(section,_T("authID"), NULL, iniFile);
		WritePrivateProfileString(section,_T("username"), NULL, iniFile);
		WritePrivateProfileString(section,_T("passwordSize"), NULL, iniFile);
		WritePrivateProfileString(section,_T("password"), NULL, iniFile);
		WritePrivateProfileString(section,_T("id"), NULL, iniFile);
		WritePrivateProfileString(section,_T("displayName"), NULL, iniFile);
		// save new
		AccountSave(1, &account);
	}

	return !account.domain.IsEmpty() && !account.username.IsEmpty();
}

void AccountSettings::AccountSave(int id, Account *account)
{
	CString section;
	section.Format(_T("Account%d"),id);

#if !defined _GLOBAL_ACCOUNT_SIP_SERVER || defined _GLOBAL_ACCOUNT_OVERRIDE
	WritePrivateProfileString(section,_T("server"),account->server,iniFile);
#endif

#if !defined _GLOBAL_ACCOUNT_SIP_PROXY || defined _GLOBAL_ACCOUNT_OVERRIDE
	WritePrivateProfileString(section,_T("proxy"),account->proxy,iniFile);
#endif

#ifdef _GLOBAL_PROFILE
	if (account->domain == _T(_GLOBAL_PROFILE_B)) {
		WritePrivateProfileString(section,_T("domain"),_T("B"),iniFile);
#ifdef _GLOBAL_PROFILE_C
	} else if (account->domain == _T(_GLOBAL_PROFILE_C)) {
		WritePrivateProfileString(section,_T("domain"),_T("C"),iniFile);
#endif
	} else {
		WritePrivateProfileString(section,_T("domain"),_T("A"),iniFile);
	}
#else
#if !defined _GLOBAL_ACCOUNT_DOMAIN || defined _GLOBAL_ACCOUNT_OVERRIDE
	WritePrivateProfileString(section,_T("domain"),account->domain,iniFile);
#endif
#endif

#if !defined _GLOBAL_ACCOUNT_LOGIN || defined _GLOBAL_ACCOUNT_OVERRIDE
	WritePrivateProfileString(section,_T("authID"),account->authID,iniFile);
#endif

	CString usernameLocal;
	CString passwordLocal;

#ifndef _GLOBAL_API
	usernameLocal = account->username;
	passwordLocal = account->password;
#else
	usernameLocal = account->apiLogin;
	passwordLocal = account->apiPassword;
#endif
	unsigned char key[] = "sdf%^^7dk66JGdfj%4*JGHhn";
	AES aes(key);
	//CCrypto crypto;
	CByteArray arPassword;
	//if (crypto.DeriveKey((LPCTSTR)_GLOBAL_KEY)) {
	if (true){
		//crypto.Encrypt(passwordLocal,arPassword);
		aes.Cipher(passwordLocal, arPassword);
	}

	if (!account->rememberPassword) {
#if !defined _GLOBAL_ACCOUNT_USERNAME || defined _GLOBAL_ACCOUNT_OVERRIDE
		WritePrivateProfileString(section,_T("username"),_T(""),iniFile);
#endif
#if !defined _GLOBAL_ACCOUNT_PASSWORD || defined _GLOBAL_ACCOUNT_OVERRIDE
		WritePrivateProfileString(section,_T("password"),_T(""),iniFile);
		WritePrivateProfileString(section,_T("passwordSize"),NULL,iniFile);
#endif
	}

	if (account->rememberPassword) {
#if !defined _GLOBAL_ACCOUNT_USERNAME || defined _GLOBAL_ACCOUNT_OVERRIDE
		WritePrivateProfileString(section,_T("username"),usernameLocal,iniFile);
#endif
#if !defined _GLOBAL_ACCOUNT_PASSWORD || defined _GLOBAL_ACCOUNT_OVERRIDE
		if (arPassword.GetSize()) {
			WritePrivateProfileStruct(section,_T("password"),arPassword.GetData(),arPassword.GetSize(),iniFile);
			CString str;
			str.Format(_T("%d"),arPassword.GetSize());
			WritePrivateProfileString(section,_T("passwordSize"),str,iniFile);
		} else {
			WritePrivateProfileString(section,_T("password"),passwordLocal,iniFile);
			WritePrivateProfileString(section,_T("passwordSize"),NULL,iniFile);
		}
#endif
	}

#ifdef _GLOBAL_API_ID
	WritePrivateProfileString(section,_T("id"),account->apiId,iniFile);
#endif

#if !defined _GLOBAL_ACCOUNT_NAME || defined _GLOBAL_ACCOUNT_OVERRIDE
	WritePrivateProfileString(section,_T("displayName"),account->displayName,iniFile);
#endif

#if !defined _GLOBAL_ACCOUNT_MINI
	WritePrivateProfileString(section,_T("publicAddr"),account->publicAddr,iniFile);
	WritePrivateProfileString(section,_T("listenPort"),account->listenPort,iniFile);
#endif
#if !defined _GLOBAL_ACCOUNT_MINI || defined _GLOBAL_ACCOUNT_OVERRIDE
	WritePrivateProfileString(section,_T("SRTP"),account->srtp,iniFile);
	WritePrivateProfileString(section,_T("transport"),account->transport,iniFile);
	WritePrivateProfileString(section,_T("publish"),account->publish?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("STUN"),account->stun,iniFile);
	WritePrivateProfileString(section,_T("ICE"),account->ice?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("allowRewrite"),account->allowRewrite?_T("1"):_T("0"),iniFile);
#endif
}

void AccountSettings::SettingsSave()
{
	CString str;

	str.Format(_T("%d"),accountId);
	WritePrivateProfileString(_T("Settings"),_T("accountId"),str,iniFile);

#ifndef _GLOBAL_ACCOUNT_MINI
	WritePrivateProfileString(_T("Settings"),_T("disableLocalAccount"),disableLocalAccount?_T("1"):_T("0"),iniFile);
#endif

#ifndef _GLOBAL_NO_LOG
	WritePrivateProfileString(_T("Settings"),_T("enableLog"),enableLog?_T("1"):_T("0"),iniFile);
#endif

#if !defined _GLOBAL_CUSTOM || defined _GLOBAL_UPDATES
	WritePrivateProfileString(_T("Settings"),_T("updatesInterval"),updatesInterval,iniFile);
	str.Format(_T("%d"),checkUpdatesTime);
	WritePrivateProfileString(_T("Settings"),_T("checkUpdatesTime"),str,iniFile);
#endif

#ifndef _GLOBAL_AUTO_ANSWER_VALUE
	WritePrivateProfileString(_T("Settings"),_T("autoAnswer"),autoAnswer==1?_T("1"):(autoAnswer==2?_T("2"):_T("0")),iniFile);
#endif
	WritePrivateProfileString(_T("Settings"),_T("denyIncoming"),denyIncoming,iniFile);

#ifndef _GLOBAL_NO_CONTACTS
#if !defined _GLOBAL_ACCOUNT_MINI || (defined _GLOBAL_ACCOUNT_OVERRIDE && defined _GLOBAL_USERS_DIRECTORY_VALUE)
	WritePrivateProfileString(_T("Settings"),_T("usersDirectory"),usersDirectory,iniFile);
#endif
#endif

#ifndef _GLOBAL_SINGLE_MODE
	WritePrivateProfileString(_T("Settings"),_T("singleMode"),singleMode?_T("1"):_T("0"),iniFile);
#endif
#ifndef _GLOBAL_SOUND_EVENTS
	WritePrivateProfileString(_T("Settings"),_T("localDTMF"),localDTMF?_T("1"):_T("0"),iniFile);
#endif
	WritePrivateProfileString(_T("Settings"),_T("ringingSound"),ringingSound,iniFile);
	WritePrivateProfileString(_T("Settings"),_T("audioInputDevice"),_T("\"")+audioInputDevice+_T("\""),iniFile);
	WritePrivateProfileString(_T("Settings"),_T("audioOutputDevice"),_T("\"")+audioOutputDevice+_T("\""),iniFile);

	WritePrivateProfileString(_T("Settings"), _T("audioInputDevice_1"), _T("\"") + audioInputDevice_1 + _T("\""), iniFile);
	WritePrivateProfileString(_T("Settings"), _T("audioOutputDevice_1"), _T("\"") + audioOutputDevice_1 + _T("\""), iniFile);

	WritePrivateProfileString(_T("Settings"),_T("audioRingDevice"),_T("\"")+audioRingDevice+_T("\""),iniFile);
#ifndef _GLOBAL_CODECS_HARDCODED
	WritePrivateProfileString(_T("Settings"),_T("audioCodecs"),audioCodecs,iniFile);
#endif
#ifndef _GLOBAL_NO_VAD
	WritePrivateProfileString(_T("Settings"),_T("VAD"),vad?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(_T("Settings"),_T("EC"),ec?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(_T("Settings"), _T("STUNCHECK"), stuncheck ? _T("1") : _T("0"), iniFile);
	WritePrivateProfileString(_T("Settings"),_T("forceCodec"),forceCodec?_T("1"):_T("0"),iniFile);
#endif
#ifdef _GLOBAL_VIDEO
	WritePrivateProfileString(_T("Settings"),_T("videoCaptureDevice"),_T("\"")+videoCaptureDevice+_T("\""),iniFile);
	WritePrivateProfileString(_T("Settings"),_T("videoCodec"),videoCodec,iniFile);
	WritePrivateProfileString(_T("Settings"),_T("disableH264"),disableH264?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(_T("Settings"),_T("bitrateH264"),bitrateH264,iniFile);
	WritePrivateProfileString(_T("Settings"),_T("disableH263"),disableH263?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(_T("Settings"),_T("bitrateH263"),bitrateH263,iniFile);
#endif

	str.Format(_T("%d"),mainX);
	WritePrivateProfileString(_T("Settings"),_T("mainX"),str,iniFile);

	str.Format(_T("%d"),mainY);
	WritePrivateProfileString(_T("Settings"),_T("mainY"),str,iniFile);

	str.Format(_T("%d"),mainW);
	WritePrivateProfileString(_T("Settings"),_T("mainW"),str,iniFile);

	str.Format(_T("%d"),mainH);
	WritePrivateProfileString(_T("Settings"),_T("mainH"),str,iniFile);

	str.Format(_T("%d"),messagesX);
	WritePrivateProfileString(_T("Settings"),_T("messagesX"),str,iniFile);

	str.Format(_T("%d"),messagesY);
	WritePrivateProfileString(_T("Settings"),_T("messagesY"),str,iniFile);

	str.Format(_T("%d"),messagesW);
	WritePrivateProfileString(_T("Settings"),_T("messagesW"),str,iniFile);

	str.Format(_T("%d"),messagesH);
	WritePrivateProfileString(_T("Settings"),_T("messagesH"),str,iniFile);

	str.Format(_T("%d"),callsWidth0);
	WritePrivateProfileString(_T("Settings"),_T("callsWidth0"),str,iniFile);
		
	str.Format(_T("%d"),callsWidth1);
	WritePrivateProfileString(_T("Settings"),_T("callsWidth1"),str,iniFile);

	str.Format(_T("%d"),callsWidth2);
	WritePrivateProfileString(_T("Settings"),_T("callsWidth2"),str,iniFile);

	str.Format(_T("%d"),callsWidth3);
	WritePrivateProfileString(_T("Settings"),_T("callsWidth3"),str,iniFile);

	str.Format(_T("%d"),callsWidth4);
	WritePrivateProfileString(_T("Settings"),_T("callsWidth4"),str,iniFile);

	str.Format(_T("%d"),volumeOutput);
	WritePrivateProfileString(_T("Settings"),_T("volumeOutput"),str,iniFile);

	str.Format(_T("%d"),volumeInput);
	WritePrivateProfileString(_T("Settings"),_T("volumeInput"),str,iniFile);

	str.Format(_T("%d"),activeTab);
	WritePrivateProfileString(_T("Settings"),_T("activeTab"),str,iniFile);

	str.Format(_T("%d"),alwaysOnTop);
	WritePrivateProfileString(_T("Settings"),_T("alwaysOnTop"),str,iniFile);
	
	WritePrivateProfileString(_T("Settings"),_T("cmdIncomingCall"),cmdIncomingCall,iniFile);
	WritePrivateProfileString(_T("Settings"),_T("cmdCallAnswer"),cmdCallAnswer,iniFile);
}
