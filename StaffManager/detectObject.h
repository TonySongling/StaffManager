#pragma once
// Include OpenCV's C++ Interface
#include <opencv2/opencv.hpp>
#include "string"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

class detectObject
{
public:
	detectObject(void);
	~detectObject(void);


	void detectLargestObject(Mat &img, CascadeClassifier &cascade, Rect &largestObject, int scaledWidth = 320);
	void detectObjectsCustom(Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors);
};

