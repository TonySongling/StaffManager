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
	Mat reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace);
	double getSimilarity(const Mat A, const Mat B);
	Ptr<FaceRecognizer> learnCollectedFaces(const vector<Mat> preprocessedFaces, const vector<int> faceLabels, const string facerecAlgorithm = "FaceRecognizer.Eigenfaces");
	Mat getImageFrom1DFloatMat(const Mat matrixRow, int height);
};

