#include "StdAfx.h"
#include "Utils.h"


Utils::Utils(void)
{
	CHANGE_IN_IMAGE_FOR_COLLECTION = 0.3;
	CHANGE_IN_SECONDS_FOR_COLLECTION = 1.0;
}


Utils::~Utils(void)
{
}


void Utils::DrawPicToHDC(CDC *pDc, Mat &cameraFrame, UINT ID)
{
	

}


void Utils::GetPreprocessFaces(vector<Mat> preprocessedFaces, Mat displayedFrame, int faceWidth, CascadeClassifier faceCascade, CascadeClassifier eyeCascade1, CascadeClassifier eyeCascade2, Rect faceRect, Point leftEye, Point rightEye, Rect searchedLeftEye, Rect searchedRightEye,Mat old_prepreprocessedFace, double old_time)
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
