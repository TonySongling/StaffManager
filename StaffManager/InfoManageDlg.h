#pragma once
#include "AddDlg.h"


// CInfoManageDlg 对话框

class CInfoManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoManageDlg)

public:
	CListCtrl* m_list;
	CDC m_dc;
	CSize m_size;
	CInfoManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoManageDlg();

// 对话框数据
	enum { IDD = IDD_INFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAdd();

public:
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	virtual BOOL OnInitDialog();
	void SetListItemName();
	void ReadStaff(CListCtrl* pList);
	afx_msg void OnPaint();
};
