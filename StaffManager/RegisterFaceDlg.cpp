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


// Set the desired face dimensions. Note that "getPreprocessedFace()" will return a square face.
const int faceWidth = 70;
const int faceHeight = faceWidth;

const int DESIRED_CAMERA_WIDTH = 640;
const int DESIRED_CAMERA_HEIGHT = 480;


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
	CString staff_no = m_list->GetItemText(nSel,0);

	//创建Face资源文件夹
	Utils utils;
	CString path = "Face";
	if (!PathFileExists(path))
	{
		if (utils.CreatePath(path))
		{
			SetCurrentDirectory(path);
		}else{
			AfxMessageBox("创建资源文件夹失败");
			return;
		}
	}else{
		SetCurrentDirectory(path);
	}

	VideoCapture videoCapture;

	InitUtils initUtils;
	int cameraNumber = 0;
	//初始化摄像头
	if(!initUtils.initWebcam(videoCapture,cameraNumber))
		return;

	videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
	videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

	
	vector<Mat> preprocessedFaces;
	Mat old_prepreprocessedFace;
	Mat cameraFrame;
	Mat displayedFrame;
	// Find a face and preprocess it to have a standard size and contrast & brightness.
	Rect faceRect;  // Position of detected face.
	Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
	Point leftEye, rightEye;    // Position of the detected eyes.

	double old_time = 0;
	while (true)
	{
		videoCapture >> cameraFrame;
		if( cameraFrame.empty() ) {
			AfxMessageBox("获取图像失败");
			return;
		}
		cameraFrame.copyTo(displayedFrame);

		utils.GetPreprocessFaces(preprocessedFaces, displayedFrame, faceWidth, faceCascade, eyeCascade1, eyeCascade2, faceRect, leftEye,  rightEye,  searchedLeftEye, searchedRightEye, old_prepreprocessedFace, old_time);
		if ((preprocessedFaces.size()/2) == 5)
		{
			//创建员工脸部特征资源文件夹
			if (!PathFileExists(staff_no))
			{
				if (utils.CreatePath(staff_no))
				{
					SetCurrentDirectory(staff_no);

				}else{
					AfxMessageBox("创建员工脸部特征资源文件夹失败");
					return;
				}
			}else{
				SetCurrentDirectory(staff_no);
				utils.DeletePath(staff_no);
			}
			
			//迭代保存图片
			vector<Mat>::iterator iter;
			vector<int> compression_params;
			compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
			compression_params.push_back(9);    //png格式下，默认的参数为3.
			int i = 1;
			CString SerialNum;
			CString fileName;
			for (iter = preprocessedFaces.begin();iter!=preprocessedFaces.end();iter++)
			{
				SerialNum.Format("%d",i);
				fileName = staff_no + "_" + SerialNum + ".png";
				try {
					imwrite(fileName.GetBuffer(fileName.GetLength()), *iter, compression_params);
					++i;
				}
				catch (runtime_error& ex) {
					
				}
			}
			CString facePath = path + "\\" + "\\" + staff_no;
			utils.SaveFacePath(staff_no,facePath);
			SetCurrentDirectory("\.\.\\\.\.");
			AfxMessageBox("提取特征完成");
			return;
		}
		img = &IplImage(displayedFrame);
		cimg.CopyOf(img,3);
		cimg.DrawToHDC(hDc,&rect);
		
		if( waitKey(30)>=0 ) 
			break;
	}
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

	InitUtils initUtils;
	initUtils.initDetectors(faceCascade,eyeCascade1,eyeCascade2,faceCascadeFilename,eyeCascadeFilename1,eyeCascadeFilename2);
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
