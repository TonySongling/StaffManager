#pragma once
#include "Staff.h"
#include "opencv2/opencv.hpp"
#include "preprocessFace.h"
#include "recognition.h"
using namespace cv;
using namespace std;
class Utils
{
public:
	Utils(void);
	~Utils(void);
	// Parameters controlling how often to keep new faces when collecting them. Otherwise, the training set could look to similar to each other!
	double CHANGE_IN_IMAGE_FOR_COLLECTION;      // How much the facial image should change before collecting a new face photo for training.
	double CHANGE_IN_SECONDS_FOR_COLLECTION ;       // How much time must pass before collecting a new face photo for training.

	void DrawPicToHDC(CDC *pDc, Mat &cameraFrame, UINT ID);
	void GetPreprocessFaces(vector<Mat> preprocessedFaces, Mat displayedFrame, int faceWidth, CascadeClassifier faceCascade, CascadeClassifier eyeCascade1, CascadeClassifier eyeCascade2, Rect faceRect, Point leftEye, Point rightEye, Rect searchedLeftEye, Rect searchedRightEye,Mat old_prepreprocessedFace, double old_time);
};

