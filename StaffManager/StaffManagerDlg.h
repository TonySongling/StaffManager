
// StaffManagerDlg.h : 头文件
//

#pragma once
#include "AddDlg.h"
#include "RegisterFaceDlg.h"
#include "RecognitionDlg.h"
#include "InfoManageDlg.h"
#include "DataStatisticsDlg.h"
// CStaffManagerDlg 对话框
class CStaffManagerDlg : public CDialogEx
{
private:
	CRegisterFaceDlg* m_RegisterDlg;
	CRecognitionDlg* m_RecognizeDlg;
	CInfoManageDlg* m_InfoDlg;
	CDataStatisticsDlg* m_DataDlg;
	int m_nSel;
	CRect rect,rt;
	CWnd* ps[4];
	bool StartRecognizeFlag;

	int DESIRED_CAMERA_WIDTH;
	int DESIRED_CAMERA_HEIGHT;

// 构造
public:
	CStaffManagerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CStaffManagerDlg();
// 对话框数据
	enum { IDD = IDD_STAFFMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInputFaceButton();
	void SelectPage();
	afx_msg void OnBnClickedRecognizeButton();
	afx_msg void OnBnClickedDataButton();
	afx_msg void OnBnClickedInfoButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void CleanTimerVideo();
};
