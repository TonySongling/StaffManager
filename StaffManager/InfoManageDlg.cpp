// InfoManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "InfoManageDlg.h"
#include "afxdialogex.h"


// CInfoManageDlg 对话框

IMPLEMENT_DYNAMIC(CInfoManageDlg, CDialogEx)

CInfoManageDlg::CInfoManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfoManageDlg::IDD, pParent)
{

}

CInfoManageDlg::~CInfoManageDlg()
{
}

void CInfoManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoManageDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CInfoManageDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ADD, &CInfoManageDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CInfoManageDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CInfoManageDlg::OnBnClickedDelete)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CInfoManageDlg 消息处理程序


void CInfoManageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void CInfoManageDlg::OnBnClickedAdd()
{
	CAddDlg dlg;
	dlg.m_pInfoDlg = this;
	dlg.DoModal();
}


void CInfoManageDlg::OnBnClickedModify()
{
	m_list = (CListCtrl*)GetDlgItem(IDC_STAFF_LIST);

	POSITION pos = m_list->GetFirstSelectedItemPosition();
	int nSel = m_list->GetNextSelectedItem(pos);

	if(nSel<0)
	{
		AfxMessageBox("请选中一个员工再进行修改！");
		return;
	}

	CString no, name, sex, duty, tel;
	no = m_list->GetItemText(nSel,0);
	name = m_list->GetItemText(nSel,1);
	sex = m_list->GetItemText(nSel,2);
	duty = m_list->GetItemText(nSel,3);
	tel = m_list->GetItemText(nSel,4);

	CAddDlg dlg;
	dlg.m_pInfoDlg = this;
	dlg.no = no;
	dlg.name = name;
	dlg.sex = sex;
	dlg.duty = duty;
	dlg.tel = tel;
	dlg.modifyFlag = TRUE;
	dlg.nSel = nSel;
	dlg.DoModal();
}


void CInfoManageDlg::OnBnClickedDelete()
{
	m_list = (CListCtrl*)GetDlgItem(IDC_STAFF_LIST);

	POSITION pos = m_list->GetFirstSelectedItemPosition();
	int nSel = m_list->GetNextSelectedItem(pos);

	if(nSel<0)
	{
		AfxMessageBox("请选中一个员工再进行删除！");
		return;
	}

	CString staff_no = m_list->GetItemText(nSel,0);

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
		string sql = "delete from t_staff where staff_no=";
		sql.append(1,'\'').append(staff_no).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		
		//删除目录
		string str = "select face_path from t_face where staff_no = ";
		str.append(1,'\'').append(staff_no).append(1,'\'');
		mysql_query(&mysql,str.c_str());
		result = mysql_store_result(&mysql);
		//  int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		Utils utils;
		if(row==NULL)
		{
			//AfxMessageBox("请先登记");  //如果为空什么也不做
		}else{
			utils.DeletePath(row[0]);
		}
		if(result!=NULL) 
			mysql_free_result(result);//释放结果资源 

		//删除数据表数据
		string m_SQL = "delete from t_face where staff_no=";
		m_SQL.append(1,'\'').append(staff_no).append(1,'\'');
		mysql_query(&mysql,m_SQL.c_str());

		mysql_close(&mysql);
		m_list->DeleteItem(nSel);
		return;
	}
	else{
		MessageBox("系统出错");
	}
}


BOOL CInfoManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//设置窗口背景
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BG_BITMAP);
	m_dc.CreateCompatibleDC(NULL);
	m_dc.SelectObject(&bmp);
	BITMAP bm;
	bmp.GetBitmap(&bm);
	m_size.cx = bm.bmWidth;
	m_size.cy = bm.bmHeight;

	SetListItemName();
	ReadStaff(m_list);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInfoManageDlg::SetListItemName()
{
	m_list = (CListCtrl*)GetDlgItem(IDC_STAFF_LIST);
	m_list->InsertColumn(0,"工号",0,80);
	m_list->InsertColumn(1,"姓名",0,80);
	m_list->InsertColumn(2,"性别",0,80);
	m_list->InsertColumn(3,"职称",0,100);
	m_list->InsertColumn(4,"电话",0,150);
}


void CInfoManageDlg::ReadStaff(CListCtrl* pList)
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
		mysql_set_character_set(&mysql, "gbk");
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


void CInfoManageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.BitBlt(0,0,m_size.cx,m_size.cy,&m_dc,0,0,SRCCOPY);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
