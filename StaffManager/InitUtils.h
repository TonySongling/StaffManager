#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
class InitUtils
{
public:
	InitUtils(void);
	~InitUtils(void);
	void initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, char *faceCascadeFilename, char *eyeCascadeFilename1, char *eyeCascadeFilename2);
	bool initWebcam(VideoCapture &videoCapture, int cameraNumber);
};

