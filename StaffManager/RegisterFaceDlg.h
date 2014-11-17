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
#include "Utils.h"
using namespace cv;
using namespace std;
// CRegisterFaceDlg 对话框

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
	CascadeClassifier faceCascade;
	CascadeClassifier eyeCascade1;
	CascadeClassifier eyeCascade2;
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
	afx_msg void OnBnClickedFinishButton();
};
