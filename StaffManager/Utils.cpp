#include "StdAfx.h"
#include "Utils.h"


Utils::Utils(void)
{
	CHANGE_IN_IMAGE_FOR_COLLECTION = 0.3;
	CHANGE_IN_SECONDS_FOR_COLLECTION = 1.0;
	facerecAlgorithm = "FaceRecognizer.Eigenfaces";
}


Utils::~Utils(void)
{
}


void Utils::DrawPicToHDC(CDC *pDc, Mat &cameraFrame, UINT ID)
{
	

}


void Utils::GetPreprocessFaces(vector<Mat> &preprocessedFaces, Mat &displayedFrame, int faceWidth, CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Rect &faceRect, Point &leftEye, Point &rightEye, Rect &searchedLeftEye, Rect &searchedRightEye,Mat &old_prepreprocessedFace, double &old_time)
{
	preprocessFace p_preocess;

	Mat preprocessedFace = p_preocess.getPreprocessedFace(displayedFrame, faceWidth, faceCascade, eyeCascade1, eyeCascade2, true, &faceRect, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);

	bool gotFaceAndEyes = false;
	if (preprocessedFace.data)
		gotFaceAndEyes = true;

	//画出检测到的脸和眼睛
	if (faceRect.width > 0) {
		rectangle(displayedFrame, faceRect, CV_RGB(255, 255, 0), 2, CV_AA);

		// Draw light-blue anti-aliased circles for the 2 eyes.
		Scalar eyeColor = CV_RGB(0,255,255);
		if (leftEye.x >= 0) {   // Check if the eye was detected
			circle(displayedFrame, Point(faceRect.x + leftEye.x, faceRect.y + leftEye.y), 6, eyeColor, 1, CV_AA);
		}
		if (rightEye.x >= 0) {   // Check if the eye was detected
			circle(displayedFrame, Point(faceRect.x + rightEye.x, faceRect.y + rightEye.y), 6, eyeColor, 1, CV_AA);
		}
	}

	if (gotFaceAndEyes) {
		recognition reco;
		// Check if this face looks somewhat different from the previously collected face.
		double imageDiff = 10000000000.0;
		if (old_prepreprocessedFace.data) {
			imageDiff = reco.getSimilarity(preprocessedFace, old_prepreprocessedFace);
		}

		// Also record when it happened.
		double current_time = (double)getTickCount();
		double timeDiff_seconds = (current_time - old_time)/getTickFrequency();

		// Only process the face if it is noticeably different from the previous frame and there has been noticeable time gap.
		if ((imageDiff > CHANGE_IN_IMAGE_FOR_COLLECTION) && (timeDiff_seconds > CHANGE_IN_SECONDS_FOR_COLLECTION)) {
			// Also add the mirror image to the training set, so we have more training data, as well as to deal with faces looking to the left or right.
			Mat mirroredFace;
			flip(preprocessedFace, mirroredFace, 1);//图像反转

			// Add the face images to the list of detected faces.
			preprocessedFaces.push_back(preprocessedFace);
			preprocessedFaces.push_back(mirroredFace);

			// Make a white flash on the face, so the user knows a photo has been taken.
			Mat displayedFaceRegion = displayedFrame(faceRect);
			displayedFaceRegion += CV_RGB(90,90,90);

			// Keep a copy of the processed face, to compare on next iteration.
			old_prepreprocessedFace = preprocessedFace;
			old_time = current_time;
		}
	}

}


bool Utils::CreatePath(CString m_strFolderPath)
{
	if (PathIsDirectory)
	{
		if (CreateDirectory(m_strFolderPath, NULL)) 
		{ 
			return true;
		}
	}
	return false;
}


void Utils::DeletePath(CString sPath)
{
	CFileFind tempFind;    
	bool IsFinded = tempFind.FindFile(sPath + "\\*.*");    
	while (IsFinded)    
	{    
		IsFinded = tempFind.FindNextFile();    
		//当这个目录中不含有.的时候，就是说这不是一个文件。  
		if (!tempFind.IsDots())    
		{    
			char sFoundFileName[200];     
			strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200));  
			//如果是目录那么删除目录  
			if (tempFind.IsDirectory())    
			{     
				char sTempDir[200];       
				sprintf(sTempDir,"%s\\%s",sPath,sFoundFileName);    
				DeletePath(sTempDir); //其实真正删除文件的也就这两句，别的都是陪衬  
			}    
			//如果是文件那么删除文件  
			else      
			{     
				char sTempFileName[200];      
				sprintf(sTempFileName,"%s\\%s",sPath,sFoundFileName);    
				DeleteFile(sTempFileName);    
			}    
		}    
	}    
	tempFind.Close();
	RemoveDirectory(sPath);
}


void Utils::SaveFacePath(CString staff_no, CString face_path)
{
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
		mysql_set_character_set(&mysql, "gbk");
		string sql = "insert into t_face (staff_no, face_path) values(";
		sql.append(1,'\'').append(staff_no).append(1,'\'').append(",").append(1,'\'').append(face_path).append(1,'\'').append(")");
		mysql_query(&mysql,sql.c_str());
		mysql_close(&mysql);
	}
	else{
		AfxMessageBox("系统出错");
		return;
	}
}


vector<Face> Utils::GetAllFaces()
{
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	SQLUtils* sqlutils = new SQLUtils("localhost","root","root","work_database",3306);
	Face face;
	vector<Face> faces;
	mysql_init(&mysql);

	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		mysql_set_character_set(&mysql, "gbk");
		string sql = "select face_id, staff_no, face_path from t_face";
		mysql_query(&mysql,sql.c_str());

		result = mysql_store_result(&mysql);
		int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while(NULL != row)
		{
			for(int i=0; i<fieldcount; i++)
			{
				switch(i){
				case 0:
					face.SetFaceId(atoi(row[i]));
					break;
				case 1:
					face.SetStaffNo(row[i]);
					break;
				case 2:
					face.SetFacePath(row[i]);
					break;
				}
			}
			faces.push_back(face);
			row = mysql_fetch_row(result);
		}
		mysql_close(&mysql);
		return faces;
	}
	else{
		AfxMessageBox("系统出错");
		return vector<Face>();
	}

}


Ptr<FaceRecognizer> Utils::GetTrainModel(vector<Mat> &preprocessedFaces, vector<int> &facelabels)
{
	bool haveEnoughData = true;
	Ptr<FaceRecognizer> model;
	recognition recog;
	if (preprocessedFaces.size() <= 0 || preprocessedFaces.size() != facelabels.size())
	{
		haveEnoughData = false;
	}
	if (haveEnoughData)
	{
		model = recog.learnCollectedFaces(preprocessedFaces, facelabels, facerecAlgorithm);
		return model;
	}
	return Ptr<FaceRecognizer>();
}


CString Utils::GetStaffNoByFaceId(int face_id)
{
	CString staff_no;
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
		mysql_set_character_set(&mysql, "gbk");
		string str = "select staff_no from t_face where face_id = ";
		ostringstream m_sql;
		m_sql << str <<face_id;
		string sql;
		sql += m_sql.str();

		mysql_query(&mysql,sql.c_str());

		result = mysql_store_result(&mysql);
		int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while(NULL != row)
		{
			for(int i=0; i<fieldcount; i++)
			{
				staff_no = row[i];
			}
			row = mysql_fetch_row(result);
		}
		mysql_close(&mysql);
		return staff_no;
	}
	else{
		AfxMessageBox("系统出错");
	}
	return CString();
}


Staff Utils::GetStaffByStaffNo(CString staff_no)
{
	Staff staff;
	staff.setNo(staff_no);

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
		mysql_set_character_set(&mysql, "gbk");
		string sql = "select staff_name, staff_sex, staff_duty, staff_tel from t_staff where staff_no = ";
		sql.append(1,'\'').append(staff_no).append(1,'\'');

		mysql_query(&mysql,sql.c_str());

		result = mysql_store_result(&mysql);
		int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		while(NULL != row)
		{
			for(int i=0; i<fieldcount; i++)
			{
				switch(i){
				case 0:
					staff.setName(row[i]);
					break;
				case 1:
					staff.setSex(row[i]);
					break;
				case 2:
					staff.setDuty(row[i]);
					break;
				case 3:
					staff.setTel(row[i]);
				}
			}
			row = mysql_fetch_row(result);
		}
		mysql_close(&mysql);
		return staff;
	}
	else{
		AfxMessageBox("系统出错");
	}
	return Staff();
}


void Utils::SaveFeatureFlag(CString staff_no)
{
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
		mysql_set_character_set(&mysql, "gbk");
		string str = "update t_staff set feature_flag=";
		string sql = "";
		int feature_flag = 1;
		ostringstream oss;
		oss << str << feature_flag;
		sql += oss.str();
		sql.append(" where staff_no = ").append(1,'\'').append(staff_no).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		mysql_close(&mysql);
	}
	else{
		AfxMessageBox("系统出错");
		return;
	}
}


void Utils::SaveLogInfo(CString staff_no,CString staff_name,CString recognizeResult)
{
	//获取当前时间
	CString strTime;
	CTime m_time;
	m_time = CTime::GetCurrentTime();
	strTime = m_time.Format("%Y/%m/%d-%H:%M");

	MYSQL mysql;
	SQLUtils* sqlutils = new SQLUtils("localhost","root","root","work_database",3306);

	mysql_init(&mysql);
	
	MYSQL_RES *result = NULL;
	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		mysql_set_character_set(&mysql, "gbk");
		string sql = "insert into t_log (staff_no, staff_name, result, time) values(";
		sql.append(1,'\'').append(staff_no).append(1,'\'').append(",").append(1,'\'').append(staff_name).append(1,'\'')
			.append(",").append(1,'\'').append(recognizeResult).append(1,'\'').append(",").append(1,'\'').append(strTime).append(1,'\'').append(")");
		if (0 == mysql_query(&mysql,sql.c_str()))
		{
			mysql_close(&mysql);
		}	
	}
	else{
		AfxMessageBox("系统出错");
		return;
	}
}


void Utils::DeleteStaffByNo(CString staff_no)
{
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
		mysql_set_character_set(&mysql, "gbk");
		string sql = "delete from t_staff where staff_no=";
		sql.append(1,'\'').append(staff_no).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		
		//删除目录
		string str = "select face_path from t_face where staff_no = ";
		str.append(1,'\'').append(staff_no).append(1,'\'');
		mysql_query(&mysql,str.c_str());
		result = mysql_store_result(&mysql);
		//  int fieldcount = mysql_num_fields(result);
		MYSQL_ROW row = NULL;
		row = mysql_fetch_row(result);
		Utils utils;
		if(row==NULL)
		{
			//AfxMessageBox("请先登记");  //如果为空什么也不做
		}else{
			utils.DeletePath(row[0]);
		}
		if(result!=NULL) 
			mysql_free_result(result);//释放结果资源 

		//删除数据表数据
		string m_SQL = "delete from t_face where staff_no=";
		m_SQL.append(1,'\'').append(staff_no).append(1,'\'');
		mysql_query(&mysql,m_SQL.c_str());

		mysql_close(&mysql);
		return;
	}else{
		AfxMessageBox("系统出错");
	}
}


void Utils::AddStaff(CString name, CString no, CString sex, CString duty, CString tel, CListCtrl* list, int nCount)
{
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
		mysql_set_character_set(&mysql, "gbk");
		if (!CheckNo(mysql,sqlutils,no) && !CheckTel(mysql,sqlutils,tel))
		{
			string sql = "insert into t_staff (staff_name,staff_no,staff_sex,staff_duty,staff_tel) values(";
			sql.append(1,'\'').append(name).append(1,'\'').append(",").append(1,'\'').append(no).append(1,'\'').append(",")
				.append(1,'\'').append(sex).append(1,'\'').append(",").append(1,'\'').append(duty).append(1,'\'')
				.append(",").append(1,'\'').append(tel).append(1,'\'').append(")");
			mysql_query(&mysql,sql.c_str());
			mysql_close(&mysql);
			list->InsertItem(nCount,no);
			list->SetItemText(nCount,1,name);
			list->SetItemText(nCount,2,sex);
			list->SetItemText(nCount,3,duty);
			list->SetItemText(nCount,4,tel);
			AfxMessageBox("添加信息成功");
			return;
		}else if(CheckNo(mysql,sqlutils,no)){
			AfxMessageBox("此员工编号已存在");
		}else if (CheckTel(mysql,sqlutils,tel)){
			AfxMessageBox("此员工电话已存在");
		}
		return;
	}else{
		AfxMessageBox("系统出错");
	}
}

//检查是否员工编号是否已存在
bool Utils::CheckNo(MYSQL mysql,SQLUtils* sqlutils,CString staff_no)
{
	MYSQL_RES *result = NULL;
	mysql_init(&mysql);
	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		string no(staff_no.GetBuffer(staff_no.GetLength()));
		string sql = "select staff_no from t_staff where staff_no=";
		sql.append(1,'\'').append(no).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
		int rowcount = mysql_num_rows(result);
		if (rowcount > 0)
		{
			return true;
		}

	}else{
		AfxMessageBox("系统出错");
	}
	return false;
}

//检查是否员工电话是否已存在
bool Utils::CheckTel(MYSQL mysql,SQLUtils* sqlutils, CString staff_tel)
{
	MYSQL_RES *result = NULL;
	mysql_init(&mysql);
	string serverName = sqlutils->getServerName();
	string userName = sqlutils->getUserName();
	string password = sqlutils->getPassword();
	string databaseName = sqlutils->getDatabaseName();
	int port = sqlutils->getPort();
	if (mysql_real_connect(&mysql,serverName.c_str(),userName.c_str(),password.c_str(),databaseName.c_str(),port,NULL,0))
	{
		string tel(staff_tel.GetBuffer(staff_tel.GetLength()));
		string sql = "select staff_tel from t_staff where staff_tel=";
		sql.append(1,'\'').append(tel).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		result = mysql_store_result(&mysql);
		int rowcount = mysql_num_rows(result);
		if (rowcount > 0)
		{
			return true;
		}

	}else{
		AfxMessageBox("系统出错");
	}
	return false;
}


void Utils::ModifyStaff(CString staff_name, CString staff_sex, CString staff_duty, CString staff_tel, CListCtrl* list, CString no, int nSel)
{
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
		mysql_set_character_set(&mysql, "gbk");
		string sql = "update t_staff set staff_name=";
		sql.append(1,'\'').append(staff_name).append(1,'\'').append(",").append("staff_sex=").append(1,'\'').append(staff_sex).append(1,'\'').append(",")
			.append("staff_duty=").append(1,'\'').append(staff_duty).append(1,'\'').append(",").append("staff_tel=").append(1,'\'').append(staff_tel).append(1,'\'')
			.append(" where staff_no=").append(1,'\'').append(no).append(1,'\'');
		mysql_query(&mysql,sql.c_str());
		mysql_close(&mysql);
		list->SetItemText(nSel,1,staff_name);
		list->SetItemText(nSel,2,staff_sex);
		list->SetItemText(nSel,3,staff_duty);
		list->SetItemText(nSel,4,staff_tel);
		AfxMessageBox("修改信息成功");
		return;
	}
	else{
		AfxMessageBox("系统出错");
	}
}
