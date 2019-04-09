// DialogTest2.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DialogTest2.h"
#include "afxdialogex.h"


// CDialogTest2 dialog

IMPLEMENT_DYNAMIC(CDialogTest2, CDialogEx)

CDialogTest2::CDialogTest2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogTest2::IDD, pParent)
{

}

CDialogTest2::~CDialogTest2()
{
}

void CDialogTest2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogTest2, CDialogEx)
END_MESSAGE_MAP()


// CDialogTest2 message handlers
