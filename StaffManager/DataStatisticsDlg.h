#pragma once


// CDataStatisticsDlg 对话框

class CDataStatisticsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataStatisticsDlg)

public:
	CDataStatisticsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataStatisticsDlg();

// 对话框数据
	enum { IDD = IDD_DATAMANAGEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
