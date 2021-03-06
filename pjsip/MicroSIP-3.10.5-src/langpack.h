#pragma once

#if defined( UNICODE ) && !defined( _UNICODE )
	#define _UNICODE
#endif

#define SIZEOF(X) (sizeof(X)/sizeof(X[0]))

void LoadLangPackModule(void);
void UnloadLangPackModule(void);
int TranslateDialog(HWND hwndDlg);
void TranslateMenu(HMENU hmenu);

char* LangPackTranslateString(const char *szEnglish, const int W);
__inline LPTSTR Translate(LPTSTR source)
{
#ifdef _UNICODE
	return ( LPTSTR )LangPackTranslateString( (char*)source, 1 );
#else
	return ( LPTSTR )LangPackTranslateString( source, 0 );
#endif

	
}