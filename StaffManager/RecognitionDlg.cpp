// RecognitionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "RecognitionDlg.h"
#include "afxdialogex.h"

// CRecognitionDlg 对话框

IMPLEMENT_DYNAMIC(CRecognitionDlg, CDialogEx)

CRecognitionDlg::CRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecognitionDlg::IDD, pParent)
{
	facerecAlgorithm = "FaceRecognizer.LBPH";
	UNKNOWN_PERSON_THRESHOLD = 0.7f;


	// Cascade Classifier file, used for Face Detection.
	faceCascadeFilename = "lbpcascade_frontalface.xml";     // LBP face detector.
	eyeCascadeFilename1 = "haarcascade_eye.xml";               // Basic eye detector for open eyes only.
	eyeCascadeFilename2 = "haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.


	// Set the desired face dimensions. Note that "getPreprocessedFace()" will return a square face.
	faceWidth = 70;
	faceHeight = faceWidth;

	DESIRED_CAMERA_WIDTH = 640;
	DESIRED_CAMERA_HEIGHT = 480;
}

CRecognitionDlg::~CRecognitionDlg()
{
}

void CRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecognitionDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CRecognitionDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_RECOGNITION, &CRecognitionDlg::OnBnClickedRecognition)
END_MESSAGE_MAP()


// CRecognitionDlg 消息处理程序


void CRecognitionDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CRecognitionDlg::OnBnClickedRecognition()
{
	vector<Mat> preprocessFaces;
	vector<int> facelabels;
	Utils utils;
	vector<Face> faces = utils.GetAllFaces();

	vector<Face>::iterator iter;
	Face face;
	for (iter = faces.begin();iter!=faces.end();iter++)
	{
		face = *iter;
		CString staff_no = face.GetStaffNo();
		CString face_path = face.GetFacePath();
		CString rootPath = face_path.Left(face_path.Find("\\"));

		Mat faceImg;
		CString fileName;
		CString SerialNum;;
		if (PathFileExists(rootPath))
		{
			SetCurrentDirectory(rootPath);
			if (PathFileExists(staff_no))
			{
				SetCurrentDirectory(staff_no);
				for (int i = 1; i <= 10; i++)
				{
					SerialNum.Format("%d",i);
					fileName = staff_no + "_" + SerialNum + ".png";
					faceImg = imread(fileName.GetBuffer(fileName.GetLength()), CV_LOAD_IMAGE_GRAYSCALE);
					preprocessFaces.push_back(faceImg);
					facelabels.push_back(face.GetFaceId());
				}
			}else{
				AfxMessageBox("请登记特征后再进行验证");
			}
			SetCurrentDirectory("\.\.\\\.\.");
		}else{
			AfxMessageBox("请登记特征后再进行验证");
		}
	}

	Ptr<FaceRecognizer> model = utils.GetTrainModel(preprocessFaces,facelabels);
}


BOOL CRecognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pDc = GetDlgItem(IDC_IMAGE_STATIC)->GetDC();
	hDc = pDc->GetSafeHdc();
	GetDlgItem(IDC_IMAGE_STATIC)->GetClientRect(&rect);

	InitUtils initUtils;
	initUtils.initDetectors(faceCascade,eyeCascade1,eyeCascade2,faceCascadeFilename,eyeCascadeFilename1,eyeCascadeFilename2);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
