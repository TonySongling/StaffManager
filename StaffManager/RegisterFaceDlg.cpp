// RegisterFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "RegisterFaceDlg.h"
#include "afxdialogex.h"
#include "string"

using namespace std;
// CRegisterFaceDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterFaceDlg, CDialogEx)

CRegisterFaceDlg::CRegisterFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterFaceDlg::IDD, pParent)
{

}

CRegisterFaceDlg::~CRegisterFaceDlg()
{
}

void CRegisterFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterFaceDlg, CDialogEx)
	ON_BN_CLICKED(ID_ADD_BUTTON, &CRegisterFaceDlg::OnBnClickedAddButton)
END_MESSAGE_MAP()


// CRegisterFaceDlg 消息处理程序



void CRegisterFaceDlg::OnBnClickedAddButton()
{
	POSITION pos = m_list->GetFirstSelectedItemPosition();
	int nSel = m_list->GetNextSelectedItem(pos);
	if (nSel < 0)
	{
		MessageBox("请选中一项再进行操作");
		return;
	}
}


BOOL CRegisterFaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list = (CListCtrl*)GetDlgItem(IDC_INFO_LIST);
	m_list->InsertColumn(0,"工号",0,80);
	m_list->InsertColumn(1,"姓名",0,80);
	m_list->InsertColumn(2,"性别",0,80);
	m_list->InsertColumn(3,"职称",0,100);
	m_list->InsertColumn(4,"电话",0,150);
	readStaff(m_list);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRegisterFaceDlg::readStaff(CListCtrl* pList)
{
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	Staff staff;
	SQLUtils* sqlutils = new SQLUtils("localhost","root","root","work_database",3306);

	mysql_init(&mysql);

	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		string sql = "select staff_no, staff_name, staff_sex, staff_duty, staff_tel from t_staff";
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
						staff.setNo(row[i]);
						break;
					case 1:
						staff.setName(row[i]);
						break;
					case 2:
						staff.setSex(row[i]);
						break;
					case 3:
						staff.setDuty(row[i]);
						break;
					case 4:
						staff.setTel(row[i]);
						break;
				}
			}
			pList->InsertItem(j,staff.getNo());
			pList->SetItemText(j,1,staff.getName());
			pList->SetItemText(j,2,staff.getSex());
			pList->SetItemText(j,3,staff.getDuty());
			pList->SetItemText(j,4,staff.getTel());
			++j;
			row = mysql_fetch_row(result);
		}
		mysql_close(&mysql);

	}
	else{
		MessageBox("系统出错");
	}
}
