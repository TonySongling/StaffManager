// RegisterFaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StaffManager.h"
#include "RegisterFaceDlg.h"
#include "afxdialogex.h"

// CRegisterFaceDlg �Ի���

IMPLEMENT_DYNAMIC(CRegisterFaceDlg, CDialogEx)

CRegisterFaceDlg::CRegisterFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterFaceDlg::IDD, pParent)
{
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

CRegisterFaceDlg::~CRegisterFaceDlg()
{
}

void CRegisterFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterFaceDlg, CDialogEx)
	ON_BN_CLICKED(ID_ADD_BUTTON, &CRegisterFaceDlg::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CRegisterFaceDlg::OnBnClickedDeleteButton)
END_MESSAGE_MAP()


// CRegisterFaceDlg ��Ϣ�������



void CRegisterFaceDlg::OnBnClickedAddButton()
{
	POSITION pos = m_list->GetFirstSelectedItemPosition();
	int nSel = m_list->GetNextSelectedItem(pos);
	if (nSel < 0)
	{
		MessageBox("��ѡ��һ���ٽ��в���");
		return;
	}
	CString staff_no = m_list->GetItemText(nSel,0);

	//����Face��Դ�ļ���
	Utils utils;
	CString path = "Face";
	if (!PathFileExists(path))
	{
		if (utils.CreatePath(path))
		{
			SetCurrentDirectory(path);
		}else{
			AfxMessageBox("������Դ�ļ���ʧ��");
			return;
		}
	}else{
		SetCurrentDirectory(path);
	}

	VideoCapture videoCapture;

	InitUtils initUtils;
	int cameraNumber = 0;
	//��ʼ������ͷ
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
			AfxMessageBox("��ȡͼ��ʧ��");
			return;
		}
		cameraFrame.copyTo(displayedFrame);

		utils.GetPreprocessFaces(preprocessedFaces, displayedFrame, faceWidth, faceCascade, eyeCascade1, eyeCascade2, faceRect, leftEye,  rightEye,  searchedLeftEye, searchedRightEye, old_prepreprocessedFace, old_time);
		if ((preprocessedFaces.size()/2) == 5)
		{
			//����Ա������������Դ�ļ���
			if (!PathFileExists(staff_no))
			{
				if (utils.CreatePath(staff_no))
				{
					SetCurrentDirectory(staff_no);

				}else{
					AfxMessageBox("����Ա������������Դ�ļ���ʧ��");
					return;
				}
			}else{
				SetCurrentDirectory(staff_no);
				utils.DeletePath(staff_no);
			}
			
			//��������ͼƬ
			vector<Mat>::iterator iter;
			vector<int> compression_params;
			compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
			compression_params.push_back(9);    //png��ʽ�£�Ĭ�ϵĲ���Ϊ3.
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
			AfxMessageBox("��ȡ�������");
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
	m_list->InsertColumn(0,"����",0,80);
	m_list->InsertColumn(1,"����",0,80);
	m_list->InsertColumn(2,"�Ա�",0,80);
	m_list->InsertColumn(3,"ְ��",0,100);
	m_list->InsertColumn(4,"�绰",0,150);
	readStaff(m_list);

	pDc = GetDlgItem(IDC_IMAGE)->GetDC();
	hDc = pDc->GetSafeHdc();
	GetDlgItem(IDC_IMAGE)->GetClientRect(&rect);

	InitUtils initUtils;
	initUtils.initDetectors(faceCascade,eyeCascade1,eyeCascade2,faceCascadeFilename,eyeCascadeFilename1,eyeCascadeFilename2);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
		//	pList->SetItemText(j,0,staff.getNo());
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
		MessageBox("ϵͳ����");
	}
}


void CRegisterFaceDlg::OnBnClickedDeleteButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	POSITION pos = m_list->GetFirstSelectedItemPosition();
	int nSel = m_list->GetNextSelectedItem(pos);
	if (nSel < 0)
	{
		MessageBox("��ѡ��һ���ٽ��в���");
		return;
	}
	CString staff_no = m_list->GetItemText(nSel,0);
	//�������ݿ� ��ɾ��staff_no ��Ӧ��face_path�����е��ļ� �ٴ����ݿ���ɾ�� t_staff����staff_no ����һ�У�ɾ��t_face����staff_no ����һ��
	//�������ݿ⣬�ڶԻ�����ȥ����ɾ���ļ�¼
	
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	SQLUtils* sqlutils = new SQLUtils("localhost","root","root","work_database",3306);

	mysql_init(&mysql);
	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();	
    if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		    //ɾ��Ŀ¼
		string str = "select face_path from t_face where staff_no = ";
		ostringstream m_sql;
		m_sql << str <<staff_no;
		string sql;
		sql += m_sql.str();
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
	//  int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		Utils utils;
		if(row==NULL)
		{
			//AfxMessageBox("���ȵǼ�");  //���Ϊ��ʲôҲ����
		}else{
	      utils.DeletePath(row[0]);
		  AfxMessageBox("ɾ��Ŀ¼�ɹ�");
		}
		if(result!=NULL) 
		mysql_free_result(result);//�ͷŽ����Դ 

		//ɾ����¼
		CString strSQL;
		CString str_PreName= staff_no;
		strSQL.Format("delete from t_face where staff_no=\'%s\'",str_PreName); 
		if(mysql_real_query(&mysql,(char*)(LPCTSTR)strSQL,(UINT)strSQL.GetLength())!=0)
		{
			AfxMessageBox("ɾ����¼ʧ��");  
		}
		strSQL.Format("delete from t_staff where staff_no=\'%s\'",str_PreName);
		if(mysql_real_query(&mysql,(char*)(LPCTSTR)strSQL,(UINT)strSQL.GetLength())!=0)
		{
			AfxMessageBox("ɾ����¼ʧ��");  
		}else{
			
			AfxMessageBox("ɾ����¼�ɹ�"); 
		}

	
		UpdateData();
		m_list->DeleteAllItems();
		OnInitDialog();	
		mysql_close(&mysql);
	}
	else{
		AfxMessageBox("ϵͳ����");
		return;
	}




}
