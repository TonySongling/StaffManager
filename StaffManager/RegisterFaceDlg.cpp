// RegisterFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "RegisterFaceDlg.h"
#include "afxdialogex.h"

// The Face Recognition algorithm can be one of these and perhaps more, depending on your version of OpenCV, which must be atleast v2.4.1:
//    "FaceRecognizer.Eigenfaces":  Eigenfaces, also referred to as PCA (Turk and Pentland, 1991).
//    "FaceRecognizer.Fisherfaces": Fisherfaces, also referred to as LDA (Belhumeur et al, 1997).
//    "FaceRecognizer.LBPH":        Local Binary Pattern Histograms (Ahonen et al, 2006).
const char *facerecAlgorithm = "FaceRecognizer.Fisherfaces";
//const char *facerecAlgorithm = "FaceRecognizer.Eigenfaces";


// Sets how confident the Face Verification algorithm should be to decide if it is an unknown person or a known person.
// A value roughly around 0.5 seems OK for Eigenfaces or 0.7 for Fisherfaces, but you may want to adjust it for your
// conditions, and if you use a different Face Recognition algorithm.
// Note that a higher threshold value means accepting more faces as known people,
// whereas lower values mean more faces will be classified as "unknown".
const float UNKNOWN_PERSON_THRESHOLD = 0.7f;


// Cascade Classifier file, used for Face Detection.
char *faceCascadeFilename = "lbpcascade_frontalface.xml";     // LBP face detector.
//char *faceCascadeFilename = "haarcascade_frontalface_alt_tree.xml";  // Haar face detector.
//char *eyeCascadeFilename1 = "haarcascade_lefteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//char *eyeCascadeFilename2 = "haarcascade_righteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//char *eyeCascadeFilename1 = "haarcascade_mcs_lefteye.xml";       // Good eye detector for open-or-closed eyes.
//char *eyeCascadeFilename2 = "haarcascade_mcs_righteye.xml";       // Good eye detector for open-or-closed eyes.
char *eyeCascadeFilename1 = "haarcascade_eye.xml";               // Basic eye detector for open eyes only.
char *eyeCascadeFilename2 = "haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.
VideoCapture videoCapture;

// Set the desired face dimensions. Note that "getPreprocessedFace()" will return a square face.
const int faceWidth = 70;
const int faceHeight = faceWidth;

const int DESIRED_CAMERA_WIDTH = 640;
const int DESIRED_CAMERA_HEIGHT = 480;

// Parameters controlling how often to keep new faces when collecting them. Otherwise, the training set could look to similar to each other!
const double CHANGE_IN_IMAGE_FOR_COLLECTION = 0.3;      // How much the facial image should change before collecting a new face photo for training.
const double CHANGE_IN_SECONDS_FOR_COLLECTION = 1.0;       // How much time must pass before collecting a new face photo for training.

const char *windowName = "人脸特征提取";   // Name shown in the GUI window.
const int BORDER = 8;  // Border between GUI elements to the edge of the image.

const bool preprocessLeftAndRightSeparately = true;   // Preprocess left & right sides of the face separately, in case there is stronger light on one side.

// Set to true if you want to see many windows created, showing various debug info. Set to 0 otherwise.
bool m_debug = false;

// CRegisterFaceDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterFaceDlg, CDialogEx)

CRegisterFaceDlg::CRegisterFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterFaceDlg::IDD, pParent)
{

}

CRegisterFaceDlg::~CRegisterFaceDlg()
{
}

void CRegisterFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterFaceDlg, CDialogEx)
	ON_BN_CLICKED(ID_ADD_BUTTON, &CRegisterFaceDlg::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_FINISH_BUTTON, &CRegisterFaceDlg::OnBnClickedFinishButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRegisterFaceDlg 消息处理程序



void CRegisterFaceDlg::OnBnClickedAddButton()
{
	POSITION pos = m_list->GetFirstSelectedItemPosition();
	int nSel = m_list->GetNextSelectedItem(pos);
	if (nSel < 0)
	{
		MessageBox("请选中一项再进行操作");
		return;
	}

	CascadeClassifier faceCascade;
	CascadeClassifier eyeCascade1;
	CascadeClassifier eyeCascade2;
	

	InitUtils initUtils;
	initUtils.initDetectors(faceCascade,eyeCascade1,eyeCascade2,faceCascadeFilename,eyeCascadeFilename1,eyeCascadeFilename2);

	int cameraNumber = 0;
	if(!initUtils.initWebcam(videoCapture,cameraNumber))
		return;

	videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
	videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

	//CDC *pDc = GetDlgItem(IDC_IMAGE)->GetDC();
	//HDC hDc = pDc->GetSafeHdc();
	//CRect rect;
	//GetDlgItem(IDC_IMAGE)->GetClientRect(&rect);
	//CvvImage cimg;
	//IplImage *img;
	

}


BOOL CRegisterFaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list = (CListCtrl*)GetDlgItem(IDC_INFO_LIST);
	m_list->InsertColumn(0,"工号",0,80);
	m_list->InsertColumn(1,"姓名",0,80);
	m_list->InsertColumn(2,"性别",0,80);
	m_list->InsertColumn(3,"职称",0,100);
	m_list->InsertColumn(4,"电话",0,150);
	readStaff(m_list);

	pDc = GetDlgItem(IDC_IMAGE)->GetDC();
	hDc = pDc->GetSafeHdc();
	GetDlgItem(IDC_IMAGE)->GetClientRect(&rect);

	Mat cameraFrame;
		videoCapture >> cameraFrame;
		img = &IplImage(cameraFrame);
		cimg.CopyOf(img,3);
		if(true){
		cimg.DrawToHDC(hDc,&rect);
		}
	SetTimer(1,10,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRegisterFaceDlg::readStaff(CListCtrl* pList)
{
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	Staff staff;
	SQLUtils* sqlutils = new SQLUtils("localhost","root","root","work_database",3306);

	mysql_init(&mysql);

	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		string sql = "select staff_no, staff_name, staff_sex, staff_duty, staff_tel from t_staff";
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
		int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		int j = 0;
		while(NULL != row)
		{
			for(int i=0; i<fieldcount; i++)
			{
				switch(i){
					case 0:
						staff.setNo(row[i]);
						break;
					case 1:
						staff.setName(row[i]);
						break;
					case 2:
						staff.setSex(row[i]);
						break;
					case 3:
						staff.setDuty(row[i]);
						break;
					case 4:
						staff.setTel(row[i]);
						break;
				}
			}
			pList->InsertItem(j,staff.getNo());
			pList->SetItemText(j,1,staff.getName());
			pList->SetItemText(j,2,staff.getSex());
			pList->SetItemText(j,3,staff.getDuty());
			pList->SetItemText(j,4,staff.getTel());
			++j;
			row = mysql_fetch_row(result);
		}
		mysql_close(&mysql);

	}
	else{
		MessageBox("系统出错");
	}
}


void CRegisterFaceDlg::OnBnClickedFinishButton()
{
	
}


void CRegisterFaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		Mat cameraFrame;
		videoCapture >> cameraFrame;
		img = &IplImage(cameraFrame);
		cimg.CopyOf(img,3);
		cimg.DrawToHDC(hDc,&rect);
	
	CDialogEx::OnTimer(nIDEvent);
}
