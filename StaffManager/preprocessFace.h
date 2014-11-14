#pragma once
// Include OpenCV's C++ Interface
#include <opencv2/opencv.hpp>
#include "detectObject.h"
#include "string"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

class preprocessFace
{
public:
	preprocessFace(void);
	~preprocessFace(void);
	void detectBothEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Point &leftEye, Point &rightEye, Rect *searchedLeftEye = NULL, Rect *searchedRightEye = NULL);
	void equalizeLeftAndRightHalves(Mat &faceImg);
	Mat getPreprocessedFace(Mat &srcImg, int desiredFaceWidth, CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, bool doLeftAndRightSeparately, Rect *storeFaceRect = NULL, Point *storeLeftEye = NULL, Point *storeRightEye = NULL, Rect *searchedLeftEye = NULL, Rect *searchedRightEye = NULL);
};

