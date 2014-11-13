#pragma once
#include "Staff.h"

// CRegisterFaceDlg 对话框

class CRegisterFaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterFaceDlg)

public:
	CListCtrl* m_list;
	CRegisterFaceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterFaceDlg();

// 对话框数据
	enum { IDD = IDD_INPUT_FACE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddButton();
	virtual BOOL OnInitDialog();
	void readStaff(CListCtrl* pList);
};
