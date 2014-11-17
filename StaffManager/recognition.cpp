#include "StdAfx.h"
#include "recognition.h"


recognition::recognition(void)
{
}


recognition::~recognition(void)
{
}


Mat recognition::reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace)
{
	// Since we can only reconstruct the face for some types of FaceRecognizer models (ie: Eigenfaces or Fisherfaces),
	// we should surround the OpenCV calls by a try/catch block so we don't crash for other models.
	try {

		// Get some required data from the FaceRecognizer model.
		Mat eigenvectors = model->get<Mat>("eigenvectors");
		Mat averageFaceRow = model->get<Mat>("mean");

		int faceHeight = preprocessedFace.rows;

		// Project the input image onto the PCA subspace.
		Mat projection = subspaceProject(eigenvectors, averageFaceRow, preprocessedFace.reshape(1,1));
		//printMatInfo(projection, "projection");

		// Generate the reconstructed face back from the PCA subspace.
		Mat reconstructionRow = subspaceReconstruct(eigenvectors, averageFaceRow, projection);
		//printMatInfo(reconstructionRow, "reconstructionRow");

		// Convert the float row matrix to a regular 8-bit image. Note that we
		// shouldn't use "getImageFrom1DFloatMat()" because we don't want to normalize
		// the data since it is already at the perfect scale.

		// Make it a rectangular shaped image instead of a single row.
		Mat reconstructionMat = reconstructionRow.reshape(1, faceHeight);
		// Convert the floating-point pixels to regular 8-bit uchar pixels.
		Mat reconstructedFace = Mat(reconstructionMat.size(), CV_8U);
		reconstructionMat.convertTo(reconstructedFace, CV_8U, 1, 0);
		//printMatInfo(reconstructedFace, "reconstructedFace");

		return reconstructedFace;

	} catch (cv::Exception e) {
		//cout << "WARNING: Missing FaceRecognizer properties." << endl;
		return Mat();
	}
}


double recognition::getSimilarity(const Mat A, const Mat B)
{
	if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols) {
		// Calculate the L2 relative error between the 2 images.
		double errorL2 = norm(A, B, CV_L2);
		// Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
		double similarity = errorL2 / (double)(A.rows * A.cols);
		return similarity;
	}
	else {
		//cout << "WARNING: Images have a different size in 'getSimilarity()'." << endl;
		return 100000000.0;  // Return a bad value
	}
}


Ptr<FaceRecognizer> recognition::learnCollectedFaces(const vector<Mat> preprocessedFaces, const vector<int> faceLabels, const string facerecAlgorithm)
{
	Ptr<FaceRecognizer> model;


	// Make sure the "contrib" module is dynamically loaded at runtime.
	bool haveContribModule = initModule_contrib();
	if (!haveContribModule) {
		AfxMessageBox("ERROR: The 'contrib' module is needed for FaceRecognizer but has not been loaded into OpenCV!");
		return NULL;
	}

	// Use the new FaceRecognizer class in OpenCV's "contrib" module:
	// Requires OpenCV v2.4.1 or later (from June 2012), otherwise the FaceRecognizer will not compile or run!
	model = Algorithm::create<FaceRecognizer>(facerecAlgorithm);
	if (model.empty()) {
		AfxMessageBox( "ERROR: The FaceRecognizer algorithm is not available in your version of OpenCV. Please update to OpenCV v2.4.1 or newer.");
		return NULL;
	}

	// Do the actual training from the collected faces. Might take several seconds or minutes depending on input!
	model->train(preprocessedFaces, faceLabels);

	return model;
}


Mat recognition::getImageFrom1DFloatMat(const Mat matrixRow, int height)
{
	// Make it a rectangular shaped image instead of a single row.
	Mat rectangularMat = matrixRow.reshape(1, height);
	// Scale the values to be between 0 to 255 and store them as a regular 8-bit uchar image.
	Mat dst;
	normalize(rectangularMat, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	return dst;
}
