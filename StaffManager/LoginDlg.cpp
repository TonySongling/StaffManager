// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "mysql.h"
#include "string"

using namespace std;
// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_ManagerName(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (CheckUser())
	{
		CDialogEx::OnOK();
		m_StaffManageDlg.DoModal();
	}else{
		SetDlgItemText(IDC_PASSWORD_EDIT,"");
		SetFocus();
	}

}
BOOL CLoginDlg::CheckUser(void)
{
	CString user_name, pass;
	GetDlgItemText(IDC_USER_EDIT,user_name);
	GetDlgItemText(IDC_PASSWORD_EDIT,pass);

	if (user_name.IsEmpty() || pass.IsEmpty())
	{
		AfxMessageBox("用户名和密码不能为空");
		return FALSE;
	}
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	SQLUtils* sqlutils = new SQLUtils("localhost","root","root","work_database",3306);

	mysql_init(&mysql);

	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		string sql = "select user_name, password from t_user where user_name=";
		sql.append(1,'\'').append(user_name).append(1,'\'').append(" and password=").append(1,'\'').append(pass).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
		
		if (mysql_num_rows(result)>0)
		{
			m_ManagerName = user_name;
			mysql_close(&mysql);
			return TRUE;
		}else{
			AfxMessageBox("用户名或密码有误，请重新输入");
			return FALSE;
		}
		
	}
	else{
		AfxMessageBox("系统出错");
	}
	return FALSE;
}