
// StaffManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "AddDlg.h"
#include "RegisterFaceDlg.h"
#include "RecognitionDlg.h"
// CStaffManagerDlg �Ի���
class CStaffManagerDlg : public CDialogEx
{
// ����
public:
	CStaffManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STAFFMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedInputFaceButton();
	afx_msg void OnBnClickedButton1();
};
