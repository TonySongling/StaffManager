#pragma once
#include "Staff.h"
#include <vector>
#include <string>
#include <iostream>
#include "mysql.h"
#include "CvvImage.h"
// Include OpenCV's C++ Interface
#include "opencv2/opencv.hpp"

// Include the rest of our code!
#include "detectObject.h"       // Easily detect faces or eyes (using LBP or Haar Cascades).
#include "preprocessFace.h"     // Easily preprocess face images, for face recognition.
#include "recognition.h"     // Train the face recognition system and recognize a person from an image.
#include "InitUtils.h"
using namespace cv;
using namespace std;
// CRegisterFaceDlg �Ի���

class CRegisterFaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterFaceDlg)

public:
	CListCtrl* m_list;
	CDC *pDc;
	HDC hDc;
	CRect rect;
	CvvImage cimg;
	IplImage *img;
	bool isFinished;
	CRegisterFaceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterFaceDlg();

// �Ի�������
	enum { IDD = IDD_INPUT_FACE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddButton();
	virtual BOOL OnInitDialog();
	void readStaff(CListCtrl* pList);
	afx_msg void OnBnClickedFinishButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
