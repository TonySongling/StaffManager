#pragma once
#include "Log.h"

// CDataStatisticsDlg 对话框

class CDataStatisticsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataStatisticsDlg)

public:
	CListCtrl* m_list;
	CDataStatisticsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataStatisticsDlg();

// 对话框数据
	enum { IDD = IDD_DATAMANAGEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void ReadAllLogs(CListCtrl* pList);
};
