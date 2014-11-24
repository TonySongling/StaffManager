// DataStatisticsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "DataStatisticsDlg.h"
#include "afxdialogex.h"


// CDataStatisticsDlg 对话框

IMPLEMENT_DYNAMIC(CDataStatisticsDlg, CDialogEx)

CDataStatisticsDlg::CDataStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataStatisticsDlg::IDD, pParent)
{

}

CDataStatisticsDlg::~CDataStatisticsDlg()
{
}

void CDataStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataStatisticsDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataStatisticsDlg 消息处理程序
