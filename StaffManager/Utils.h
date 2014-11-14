#pragma once
#include "Staff.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
class Utils
{
public:
	Utils(void);
	~Utils(void);
	void DrawPicToHDC(CDC *pDc, Mat &cameraFrame, UINT ID);
};

