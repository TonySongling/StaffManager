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
	facerecAlgorithm = "FaceRecognizer.Eigenfaces";
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
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CRecognitionDlg 消息处理程序


void CRecognitionDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CRecognitionDlg::OnBnClickedRecognition()
{
	VideoCapture videoCapture;

	InitUtils initUtils;
	int cameraNumber = 0;
	//初始化摄像头
	if(!initUtils.initWebcam(videoCapture,cameraNumber))
		return;

	videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
	videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

	Mat old_prepreprocessedFace;
	Mat cameraFrame;
	Mat displayedFrame;
	Mat preprocessedFace;
	//Mat reconstructedFace;
	// Find a face and preprocess it to have a standard size and contrast & brightness.
	Rect faceRect;  // Position of detected face.
	Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
	Point leftEye, rightEye;    // Position of the detected eyes.
	double old_time = 0;
	double similarity;
	int face_id = -1;
	//开始检测时间
	double start_detect = (double)getTickCount();

	preprocessFace p_preocess;
	Utils utils;

	while (true)
	{
		videoCapture >> cameraFrame;
		if( cameraFrame.empty() ) {
			AfxMessageBox("获取图像失败");
			return;
		}
		cameraFrame.copyTo(displayedFrame);

		preprocessedFace = p_preocess.getPreprocessedFace(displayedFrame, faceWidth, faceCascade, eyeCascade1, eyeCascade2, true, &faceRect, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);

		// 画出检测到的脸和眼睛
		if (faceRect.width > 0) {
			rectangle(displayedFrame, faceRect, CV_RGB(255, 255, 0), 2, CV_AA);

			Scalar eyeColor = CV_RGB(0,255,255);
			if (leftEye.x >= 0) {
				circle(displayedFrame, Point(faceRect.x + leftEye.x, faceRect.y + leftEye.y), 6, eyeColor, 1, CV_AA);
			}
			if (rightEye.x >= 0) {
				circle(displayedFrame, Point(faceRect.x + rightEye.x, faceRect.y + rightEye.y), 6, eyeColor, 1, CV_AA);
			}
		}

		img = &IplImage(displayedFrame);
		cimg.CopyOf(img,3);
		cimg.DrawToHDC(hDc,&rect);
		double start_recognize = (double)getTickCount();
		if ((start_recognize - start_detect) / getTickFrequency() > 2.0)
		{
			// Generate a face approximation by back-projecting the eigenvectors & eigenvalues.
			recognition recog;
			Mat reconstructedFace = recog.reconstructFace(model, preprocessedFace);
			similarity = recog.getSimilarity(preprocessedFace, reconstructedFace);

			if (similarity < UNKNOWN_PERSON_THRESHOLD) {
				// Identify who the person is in the preprocessed face image.
				face_id = model->predict(preprocessedFace);
				CString staff_no = utils.GetStaffNoByFaceId(face_id);
				Staff staff = utils.GetStaffByStaffNo(staff_no);
				SetDlgItemText(IDC_NAME_EDIT,staff.getName());
				SetDlgItemText(IDC_NO_EDIT,staff.getNo());
				SetDlgItemText(IDC_SEX_EDIT,staff.getSex());
				SetDlgItemText(IDC_DUTY_EDIT,staff.getDuty());
				SetDlgItemText(IDC_TEL_EDIT,staff.getTel());
				AfxMessageBox("验证通过",MB_OK);
				return;
			}
			else {
				AfxMessageBox("信息不存在");
				return;
			}
		}
		if( waitKey(30)>=0 ) 
			break;
	}
}


BOOL CRecognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pDc = GetDlgItem(IDC_IMAGE_STATIC)->GetDC();
	hDc = pDc->GetSafeHdc();
	GetDlgItem(IDC_IMAGE_STATIC)->GetClientRect(&rect);

	InitUtils initUtils;
	initUtils.initDetectors(faceCascade,eyeCascade1,eyeCascade2,faceCascadeFilename,eyeCascadeFilename1,eyeCascadeFilename2);

	Utils utils;
	vector<Face> faces = utils.GetAllFaces();

	vector<Face>::iterator iter;
	Face face;
	for (iter = faces.begin();iter!=faces.end();iter++)
	{
		face = *iter;
		int face_id = face.GetFaceId();
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
					faceImg = imread(fileName.GetBuffer(fileName.GetLength()),CV_LOAD_IMAGE_GRAYSCALE);
					preprocessFaces.push_back(faceImg);
					facelabels.push_back(face.GetFaceId());
				}
				SetCurrentDirectory("\.\.\\\.\.");
			}else{
				AfxMessageBox("请登记特征后再进行验证");
			}

		}else{
			AfxMessageBox("请登记特征后再进行验证");
		}
	}

	//获取后续匹配所需的model
	model = utils.GetTrainModel(preprocessFaces,facelabels);

	//设置窗口背景
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BG_BITMAP);
	m_dc.CreateCompatibleDC(NULL);
	m_dc.SelectObject(&bmp);
	BITMAP bm;
	bmp.GetBitmap(&bm);
	m_size.cx = bm.bmWidth;
	m_size.cy = bm.bmHeight;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRecognitionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//画背景
	dc.BitBlt(0,0,m_size.cx,m_size.cy,&m_dc,0,0,SRCCOPY);

	//画标题栏
	CRect rect;
	GetClientRect(rect);
	rect.bottom = 50;
	dc.FillSolidRect(rect,RGB(80,70,66));
	CString str = "员工身份验证";
	dc.SetTextColor(RGB(227,223,221));
	dc.DrawText(str,rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}


LRESULT CRecognitionDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nHitTest = CDialogEx::OnNcHitTest(point);
	CRect rect;
	GetClientRect(&rect);
	rect.bottom=50;
	ScreenToClient(&point);
	if(rect.PtInRect(point))
	{
		if(nHitTest == HTCLIENT)
			nHitTest = HTCAPTION;
	}
	return nHitTest;
}
