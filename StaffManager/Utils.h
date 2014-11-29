#pragma once
#include "afxdialogex.h"
#include "stdio.h"
#include "stdlib.h"
#include <sstream>
#include "Staff.h"
#include "Face.h"
#include "opencv2/opencv.hpp"
#include "preprocessFace.h"
#include "recognition.h"
#include <shlwapi.h>
#include "mysql.h"
#include "SQLUtils.h"
using namespace cv;
using namespace std;
class Utils
{
public:
	Utils(void);
	~Utils(void);
	char *facerecAlgorithm;
	// Parameters controlling how often to keep new faces when collecting them. Otherwise, the training set could look to similar to each other!
	double CHANGE_IN_IMAGE_FOR_COLLECTION;      // How much the facial image should change before collecting a new face photo for training.
	double CHANGE_IN_SECONDS_FOR_COLLECTION ;       // How much time must pass before collecting a new face photo for training.

	void DrawPicToHDC(CDC *pDc, Mat &cameraFrame, UINT ID);
	void GetPreprocessFaces(vector<Mat> &preprocessedFaces, Mat &displayedFrame, int faceWidth, CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Rect &faceRect, Point &leftEye, Point &rightEye, Rect &searchedLeftEye, Rect &searchedRightEye,Mat &old_prepreprocessedFace, double &old_time);
	bool CreatePath(CString m_strFolderPath);
	void DeletePath(CString sPath);
	void SaveFacePath(CString staff_no, CString face_path);
	vector<Face> GetAllFaces();
	Ptr<FaceRecognizer> GetTrainModel(vector<Mat> &preprocessedFaces, vector<int> &facelabels);
	CString GetStaffNoByFaceId(int face_id);
	Staff GetStaffByStaffNo(CString staff_no);
	void SaveFeatureFlag(CString staff_no);
	void SaveLogInfo(CString staff_no,CString staff_name,CString recognizeResult);
	void DeleteStaffByNo(CString staff_no);
	void AddStaff(CString name, CString no, CString sex, CString duty, CString tel, CListCtrl* list,int nCount);
	bool CheckNo(MYSQL mysql,SQLUtils* sqlutils,CString staff_no);
	bool CheckTel(MYSQL mysql,SQLUtils* sqlutils,CString staff_tel);
	void ModifyStaff(CString staff_name, CString staff_sex, CString staff_duty, CString staff_tel, CListCtrl* list, CString no, int nSel);
};