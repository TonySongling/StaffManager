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
	ON_BN_CLICKED(IDCANCEL, &CDataStatisticsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDataStatisticsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDataStatisticsDlg 消息处理程序


void CDataStatisticsDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void CDataStatisticsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


BOOL CDataStatisticsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list = (CListCtrl*)GetDlgItem(IDC_LOG_LIST);
	m_list->InsertColumn(0,"工号",0,100);
	m_list->InsertColumn(1,"姓名",0,80);
	m_list->InsertColumn(2,"操作状态",0,130);
	m_list->InsertColumn(3,"发生时间",0,195);

	ReadAllLogs(m_list);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDataStatisticsDlg::ReadAllLogs(CListCtrl* pList)
{
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	Log m_log;
	SQLUtils* sqlutils = new SQLUtils("localhost","root","root","work_database",3306);

	mysql_init(&mysql);

	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		mysql_set_character_set(&mysql, "gbk");
		string sql = "select staff_no, staff_name, result, time from t_log";
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
		int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		int j = 0;
		while(NULL != row)
		{
			for(int i=0; i<fieldcount; i++)
			{
				switch(i){
				case 0:
					m_log.SetStaffNo(row[i]);
					break;
				case 1:
					m_log.SetStaffName(row[i]);
					break;
				case 2:
					m_log.SetResult(row[i]);
					break;
				case 3:
					m_log.SetTime(row[i]);
					break;
				}
			}
			pList->InsertItem(j,m_log.GetStaffNo());
			pList->SetItemText(j,1,m_log.GetStaffName());
			pList->SetItemText(j,2,m_log.GetResult());
			pList->SetItemText(j,3,m_log.GetTime());
			++j;
			row = mysql_fetch_row(result);
		}
		mysql_close(&mysql);

	}
	else{
		MessageBox("系统出错");
	}
}
