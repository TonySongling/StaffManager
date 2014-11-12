// AddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "AddDlg.h"
#include "afxdialogex.h"
#include "mysql.h"
#include "string"

using namespace std;
// CAddDlg 对话框

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDlg::IDD, pParent)
{

}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_BN_CLICKED(ID_ADD, &CAddDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_RESET_BUTTON, &CAddDlg::OnBnClickedResetButton)
END_MESSAGE_MAP()


// CAddDlg 消息处理程序


void CAddDlg::OnBnClickedAdd()
{
	CString name, no, sex, duty, tel;
	GetDlgItemText(IDC_NAME_EDIT,name);
	GetDlgItemText(IDC_NO_EDIT,no);
	GetDlgItemText(IDC_SEX_COMBO,sex);
	GetDlgItemText(IDC_DUTY_EDIT,duty);
	GetDlgItemText(IDC_TEL_EDIT,tel);

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
		/*string sql = "select user_name, password from t_user where user_name=";
		sql.append(1,'\'').append(user_name).append(1,'\'').append(" and password=").append(1,'\'').append(pass).append(1,'\'');*/
		string sql = "insert into t_staff (staff_name,staff_no,staff_sex,staff_duty,staff_tel) values(";
		sql.append(1,'\'').append(name).append(1,'\'').append(",").append(1,'\'').append(no).append(1,'\'').append(",")
			.append(1,'\'').append(sex).append(1,'\'').append(",").append(1,'\'').append(duty).append(1,'\'')
			.append(",").append(1,'\'').append(tel).append(1,'\'').append(")");
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);

	}
	else{
		MessageBox("系统出错");
	}
}


void CAddDlg::OnBnClickedResetButton()
{
	SetDlgItemText(IDC_NAME_EDIT,"");
	SetDlgItemText(IDC_NO_EDIT,"");
	SetDlgItemText(IDC_DUTY_EDIT,"");
	SetDlgItemText(IDC_TEL_EDIT,"");
}


BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_SEX_COMBO);
	pComb->InsertString(0,"男");
	pComb->InsertString(1,"女");
	pComb->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
