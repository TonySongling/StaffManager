// AddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StaffManager.h"
#include "AddDlg.h"
#include "afxdialogex.h"
#include "mysql.h"
#include "string"

using namespace std;
// CAddDlg �Ի���

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


// CAddDlg ��Ϣ�������


void CAddDlg::OnBnClickedAdd()
{
	CString name, no, sex, duty, tel;
	GetDlgItemText(IDC_NAME_EDIT,name);
	GetDlgItemText(IDC_NO_EDIT,no);
	GetDlgItemText(IDC_SEX_COMBO,sex);
	GetDlgItemText(IDC_DUTY_EDIT,duty);
	GetDlgItemText(IDC_TEL_EDIT,tel);

	if (name.IsEmpty() || no.IsEmpty())
	{
		MessageBox("�����ͱ�Ų���Ϊ��");
		return;
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
		if (!checkNo(mysql,sqlutils,no) && !checkTel(mysql,sqlutils,tel))
		{
			string sql = "insert into t_staff (staff_name,staff_no,staff_sex,staff_duty,staff_tel) values(";
			sql.append(1,'\'').append(name).append(1,'\'').append(",").append(1,'\'').append(no).append(1,'\'').append(",")
				.append(1,'\'').append(sex).append(1,'\'').append(",").append(1,'\'').append(duty).append(1,'\'')
				.append(",").append(1,'\'').append(tel).append(1,'\'').append(")");
			mysql_query(&mysql,sql.c_str());
			mysql_close(&mysql);
			MessageBox("�����Ϣ�ɹ�");
			return;
		}else if(checkNo(mysql,sqlutils,no)){
			MessageBox("��Ա������Ѵ���");
		}else if (checkTel(mysql,sqlutils,tel)){
			MessageBox("��Ա���绰�Ѵ���");
		}
		return;
	}
	else{
		MessageBox("ϵͳ����");
	}
}


void CAddDlg::OnBnClickedResetButton()
{
	SetDlgItemText(IDC_NAME_EDIT,"");
	SetDlgItemText(IDC_NO_EDIT,"");
	SetDlgItemText(IDC_DUTY_EDIT,"");
	SetDlgItemText(IDC_TEL_EDIT,"");

	/*CString staff_name;
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "root", "root", "work_database", 3306, NULL, 0);
	string sql = "select staff_name from t_staff;";
	mysql_query(&mysql, sql.c_str());
	result = mysql_store_result(&mysql);
	int fieldcount = mysql_num_fields(result);
	MYSQL_ROW row = NULL;
	row = mysql_fetch_row(result);
	while(NULL != row)
	{
	for(int i=0; i<fieldcount; i++)
	{
	staff_name = row[i];
	SetDlgItemText(IDC_STATICRE,staff_name);
	}
	row = mysql_fetch_row(result);
	}
	mysql_close(&mysql);*/
}


BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_SEX_COMBO);
	pComb->InsertString(0,"��");
	pComb->InsertString(1,"Ů");
	pComb->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CAddDlg::checkNo(MYSQL mysql,SQLUtils* sqlutils,CString staff_no)
{
	MYSQL_RES *result = NULL;
	mysql_init(&mysql);
	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		string no(staff_no.GetBuffer(staff_no.GetLength()));
		string sql = "select staff_no from t_staff where staff_no=";
		sql.append(1,'\'').append(no).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
		int rowcount = mysql_num_rows(result);
		if (rowcount > 0)
		{
			return TRUE;
		}

	}else{
		MessageBox("ϵͳ����");
	}
	return FALSE;
}

//����Ƿ�Ա���绰�Ƿ��Ѵ���
BOOL CAddDlg::checkTel(MYSQL mysql,SQLUtils* sqlutils,CString staff_tel)
{
	MYSQL_RES *result = NULL;
	mysql_init(&mysql);
	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		string tel(staff_tel.GetBuffer(staff_tel.GetLength()));
		string sql = "select staff_tel from t_staff where staff_tel=";
		sql.append(1,'\'').append(tel).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
		int rowcount = mysql_num_rows(result);
		if (rowcount > 0)
		{
			return TRUE;
		}

	}else{
		MessageBox("ϵͳ����");
	}
	return FALSE;
}
