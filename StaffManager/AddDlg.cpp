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

		Utils utils;
		utils.ModifyStaff(staff_name,staff_sex,staff_duty,staff_tel,m_pInfoDlg->m_list,no,nSel);
		
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
			AfxMessageBox("姓名和编号不能为空");
			return;
		}
		
		Utils utils;
		utils.AddStaff(name,no,sex,duty,tel,list,nCount);
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