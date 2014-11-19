#pragma once
// Include OpenCV's C++ Interface
#include <opencv2/opencv.hpp>
#include "string"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

class recognition
{
public:
	recognition(void);
	~recognition(void);
	Mat reconstructFace(Ptr<FaceRecognizer> &model, Mat &preprocessedFace);
	double getSimilarity(Mat A, Mat B);
	Ptr<FaceRecognizer> learnCollectedFaces(vector<Mat> &preprocessedFaces, vector<int> &faceLabels, string facerecAlgorithm);
	Mat getImageFrom1DFloatMat(const Mat matrixRow, int height);
};

