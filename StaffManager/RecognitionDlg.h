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

// CRecognitionDlg 对话框

class CRecognitionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecognitionDlg)

public:
	CDC *pDc;
	HDC hDc;
	CRect rect;
	CvvImage cimg;
	IplImage *img;
	CascadeClassifier faceCascade;
	CascadeClassifier eyeCascade1;
	CascadeClassifier eyeCascade2;

	char *facerecAlgorithm;
	float UNKNOWN_PERSON_THRESHOLD;


	// Cascade Classifier file, used for Face Detection.
	char *faceCascadeFilename;     // LBP face detector.
	char *eyeCascadeFilename1;               // Basic eye detector for open eyes only.
	char *eyeCascadeFilename2; // Basic eye detector for open eyes if they might wear glasses.


	// Set the desired face dimensions. Note that "getPreprocessedFace()" will return a square face.
	int faceWidth;
	int faceHeight;

	int DESIRED_CAMERA_WIDTH;
	int DESIRED_CAMERA_HEIGHT;

	vector<Mat> preprocessFaces;
	vector<int> facelabels;

	Ptr<FaceRecognizer> model;

	CRecognitionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecognitionDlg();

// 对话框数据
	enum { IDD = IDD_RECOGNITION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRecognition();
	virtual BOOL OnInitDialog();
};
