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
	modifyFlag = FALSE;
	nSel = 0;
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
	if (modifyFlag)
	{
		CString staff_name, staff_sex, staff_duty, staff_tel;
		GetDlgItemText(IDC_NAME_EDIT,staff_name);
		GetDlgItemText(IDC_SEX_COMBO,staff_sex);
		GetDlgItemText(IDC_DUTY_EDIT,staff_duty);
		GetDlgItemText(IDC_TEL_EDIT,staff_tel);

		CListCtrl* list = m_pInfoDlg->m_list;

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
			mysql_set_character_set(&mysql, "gbk");
			string sql = "update t_staff set staff_name=";
				sql.append(1,'\'').append(staff_name).append(1,'\'').append(",").append("staff_sex=").append(1,'\'').append(staff_sex).append(1,'\'').append(",")
					.append("staff_duty=").append(1,'\'').append(staff_duty).append(1,'\'').append(",").append("staff_tel=").append(1,'\'').append(staff_tel).append(1,'\'')
					.append(" where staff_no=").append(1,'\'').append(no).append(1,'\'');
			mysql_query(&mysql,sql.c_str());
			mysql_close(&mysql);
			list->SetItemText(nSel,1,staff_name);
			list->SetItemText(nSel,2,staff_sex);
			list->SetItemText(nSel,3,staff_duty);
			list->SetItemText(nSel,4,staff_tel);
			MessageBox("修改信息成功");
			return;
		}
		else{
			MessageBox("系统出错");
		}
	}else{
		CString name, no, sex, duty, tel;
		GetDlgItemText(IDC_NAME_EDIT,name);
		GetDlgItemText(IDC_NO_EDIT,no);
		GetDlgItemText(IDC_SEX_COMBO,sex);
		GetDlgItemText(IDC_DUTY_EDIT,duty);
		GetDlgItemText(IDC_TEL_EDIT,tel);

		CListCtrl* list = m_pInfoDlg->m_list;
		int nCount = list->GetItemCount();

		if (name.IsEmpty() || no.IsEmpty())
		{
			MessageBox("姓名和编号不能为空");
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
			mysql_set_character_set(&mysql, "gbk");
			if (!checkNo(mysql,sqlutils,no) && !checkTel(mysql,sqlutils,tel))
			{
				string sql = "insert into t_staff (staff_name,staff_no,staff_sex,staff_duty,staff_tel) values(";
				sql.append(1,'\'').append(name).append(1,'\'').append(",").append(1,'\'').append(no).append(1,'\'').append(",")
					.append(1,'\'').append(sex).append(1,'\'').append(",").append(1,'\'').append(duty).append(1,'\'')
					.append(",").append(1,'\'').append(tel).append(1,'\'').append(")");
				mysql_query(&mysql,sql.c_str());
				mysql_close(&mysql);
				list->InsertItem(nCount,no);
				list->SetItemText(nCount,1,name);
				list->SetItemText(nCount,2,sex);
				list->SetItemText(nCount,3,duty);
				list->SetItemText(nCount,4,tel);
				MessageBox("添加信息成功");
				return;
			}else if(checkNo(mysql,sqlutils,no)){
				MessageBox("此员工编号已存在");
			}else if (checkTel(mysql,sqlutils,tel)){
				MessageBox("此员工电话已存在");
			}
			return;
		}
		else{
			MessageBox("系统出错");
		}
	}
}


void CAddDlg::OnBnClickedResetButton()
{
	SetDlgItemText(IDC_NAME_EDIT,"");
	if (modifyFlag == FALSE)
	{
		SetDlgItemText(IDC_NO_EDIT,"");
	}
	SetDlgItemText(IDC_DUTY_EDIT,"");
	SetDlgItemText(IDC_TEL_EDIT,"");
}


BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_SEX_COMBO);
	pComb->InsertString(0,"男");
	pComb->InsertString(1,"女");

	if (modifyFlag)
	{
		SetWindowText("修改员工信息");
		GetDlgItem(ID_ADD)->SetWindowText("修改");
		SetDlgItemText(IDC_NO_EDIT,no);
		GetDlgItem(IDC_NO_EDIT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_NAME_EDIT,name);
		if (sex.Compare("男") == 0)
		{
			pComb->SetCurSel(0);
		}else{
			pComb->SetCurSel(1);
		}
		SetDlgItemText(IDC_SEX_COMBO,sex);
		SetDlgItemText(IDC_DUTY_EDIT,duty);
		SetDlgItemText(IDC_TEL_EDIT,tel);
	}else{
		
		pComb->SetCurSel(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
		MessageBox("系统出错");
	}
	return FALSE;
}

//检查是否员工电话是否已存在
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
		MessageBox("系统出错");
	}
	return FALSE;
}