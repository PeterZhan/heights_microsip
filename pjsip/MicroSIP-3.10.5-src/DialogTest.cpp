// DialogTest.cpp : implementation file
//

#include "stdafx.h"
#include "microsip.h"
#include "DialogTest.h"
#include "afxdialogex.h"


// CDialogTest dialog

IMPLEMENT_DYNAMIC(CDialogTest, CDialogEx)

CDialogTest::CDialogTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogTest::IDD, pParent)
{

}

CDialogTest::~CDialogTest()
{
}

void CDialogTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogTest, CDialogEx)
END_MESSAGE_MAP()


// CDialogTest message handlers
