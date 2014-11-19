#include "StdAfx.h"
#include "InitUtils.h"


InitUtils::InitUtils(void)
{
}


InitUtils::~InitUtils(void)
{
}


void InitUtils::initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, char *faceCascadeFilename, char *eyeCascadeFilename1, char *eyeCascadeFilename2)
{
	// Load the Face Detection cascade classifier xml file.
	try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
		faceCascade.load(faceCascadeFilename);
	} catch (cv::Exception &e) {}
	if ( faceCascade.empty() ) {
		AfxMessageBox("ERROR: Could not load Face Detection cascade classifier");
		return;
	}

	// Load the Eye Detection cascade classifier xml file.
	try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
		eyeCascade1.load(eyeCascadeFilename1);
	} catch (cv::Exception &e) {}
	if ( eyeCascade1.empty() ) {
		AfxMessageBox("ERROR: Could not load 1st Eye Detection cascade classifier");
		return;
	}

	// Load the Eye Detection cascade classifier xml file.
	try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
		eyeCascade2.load(eyeCascadeFilename2);
	} catch (cv::Exception &e) {}
	if ( eyeCascade2.empty() ) {
		AfxMessageBox("ERROR: Could not load 2st Eye Detection cascade classifier");
		return;
	}
}


bool InitUtils::initWebcam(VideoCapture &videoCapture, int cameraNumber)
{
	// Get access to the default camera.
	try {
		videoCapture.open(cameraNumber);
	} catch (cv::Exception &e) {}
	if ( !videoCapture.isOpened() ) {
		AfxMessageBox("无法打开摄像头");
		return FALSE;
	}
	return TRUE;
}
