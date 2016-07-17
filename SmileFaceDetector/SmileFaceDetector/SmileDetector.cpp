#include "stdafx.h"
#include "SmileDetector.h"
#include <stdexcept>      // std::invalid_argument
#include <vector>


SmileDetector::SmileDetector(const cv::Mat & image, const std::string & faceCascadePath, const std::string & smileCascadePath, const std::string & eyesCascadePath):mImage(image)
{
	if (!mFaceCascade.load(faceCascadePath)) {
		throw std::invalid_argument("Could not load face cascade");
	}
	if (!mSmileCascade.load(smileCascadePath)) {
		throw std::invalid_argument("Could not load smile cascade");
	}
	if (!mEyesCascade.load(eyesCascadePath)) {
		throw std::invalid_argument("Could not load eyes cascade");
	}
}

SmileDetector::~SmileDetector()
{
}

void SmileDetector::drawFaceOutline()
{
	cv::Size size = mImage.size();
	int imgWidth = size.width;
	int imgHeight = size.height;

	cv::Mat faceOutline = cv::Mat::zeros(size, mImage.type());
	cv::Scalar color = CV_RGB(200, 100, 200);   // black
	int thickness = 4;
	int faceHeight = (imgHeight / 2) * (70.f / 100.f);
	int faceWidth = (imgWidth / 2) * (70.f / 100.f);

	cv::ellipse(faceOutline, cv::Point(imgWidth / 2, imgHeight / 2), cv::Size(faceWidth, faceHeight), 0, 0, 360, color, thickness, CV_AA);

	// Draw the eye 
	int eyeW = faceWidth * 23 / 100;
	int eyeH = faceHeight * 11 / 100;
	int eyeX = faceWidth * 48 / 100;
	int eyeY = faceHeight * 13 / 100;
	// Set the angle and shift for the eye half ellipses.
	int eyeA = 15; // angle in degrees.
	int eyeYshift = 11;
	// Draw eyes
	ellipse(faceOutline, cv::Point(imgWidth / 2 - eyeX, imgHeight / 2 - eyeY), cv::Size(eyeW, eyeH), 0, 180 + eyeA, 360 - eyeA, color, thickness, CV_AA);
	ellipse(faceOutline, cv::Point(imgWidth / 2 - eyeX, imgHeight / 2 - eyeY - eyeYshift), cv::Size(eyeW, eyeH), 0, 0 + eyeA, 180 - eyeA, color, thickness, CV_AA);
	ellipse(faceOutline, cv::Point(imgWidth / 2 + eyeX, imgHeight / 2 - eyeY), cv::Size(eyeW, eyeH), 0, 180 + eyeA, 360 - eyeA, color, thickness, CV_AA);
	ellipse(faceOutline, cv::Point(imgWidth / 2 + eyeX, imgHeight / 2 - eyeY - eyeYshift), cv::Size(eyeW, eyeH), 0, 0 + eyeA, 180 - eyeA, color, thickness, CV_AA);

	// Draw the bottom lip of the mouth.
	int mouthY = faceHeight * 53 / 100;
	int mouthW = faceWidth * 45 / 100;
	int mouthH = faceHeight * 6 / 100;
	ellipse(faceOutline, cv::Point(imgWidth / 2, imgHeight / 2 + mouthY), cv::Size(mouthW, mouthH), 0, 0, 180, color, thickness, CV_AA);
	cv::addWeighted(mImage, 1.0, faceOutline, 0.7, 0, mImage, mImage.type()/*CV_8UC3*/);
}

void SmileDetector::detectSmile(cv::Mat & img)
{
	std::vector<cv::Rect> faces, faces2;
	const static cv::Scalar colors[] =
	{
		cv::Scalar(255,0,0),
		cv::Scalar(255,128,0),
		cv::Scalar(255,255,0),
		cv::Scalar(0,255,0),
		cv::Scalar(0,128,255),
		cv::Scalar(0,255,255),
		cv::Scalar(0,0,255),
		cv::Scalar(255,0,255)
	};
	cv::Mat gray, smallImg;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	double scale = 1;
	double fx = 1 / scale;
	cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
	cv::equalizeHist(smallImg, smallImg);
	//detect face
	mFaceCascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	for (size_t i = 0; i < faces.size(); i++)
	{
		cv::Rect r = faces[i];
		cv::Mat smallImgROI;
		std::vector<cv::Rect> nestedObjects;
		cv::Point center;
		cv::Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
				cvPoint(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
				color, 3, 8, 0);

		const int half_height = cvRound((float)r.height / 2);
		r.y = r.y + half_height;
		r.height = half_height - 1;
		smallImgROI = smallImg(r);
		mSmileCascade.detectMultiScale(smallImgROI, nestedObjects,1.1, 0, 0| cv::CASCADE_SCALE_IMAGE,cv::Size(30, 30));

		// The number of detected neighbors depends on image size (and also illumination, etc.). The
		// following steps use a floating minimum and maximum of neighbors. Intensity thus estimated will be
		//accurate only after a first smile has been displayed by the user.
		const int smile_neighbors = (int)nestedObjects.size();
		static int max_neighbors = -1;
		static int min_neighbors = -1;
		if (min_neighbors == -1) min_neighbors = smile_neighbors;
		max_neighbors = MAX(max_neighbors, smile_neighbors);

		// Draw rectangle on the left side of the image reflecting smile intensity
		float intensityZeroOne = ((float)smile_neighbors - min_neighbors) / (max_neighbors - min_neighbors + 1);
		int rect_height = cvRound((float)img.rows * intensityZeroOne);
		cv::Scalar col = cv::Scalar((float)255 * intensityZeroOne, 0, 0);
		cv::rectangle(img, cvPoint(0, img.rows), cvPoint(img.cols / 10, img.rows - rect_height), col, -1);
	}
}



void SmileDetector::detectFaceAndEyes(cv::Mat & img) {
	std::vector<cv::Rect> faces;
	cv::Scalar faceEllipseColor = cv::Scalar(90, 30, 200); //red
	cv::Scalar eyeEllipseColor = cv::Scalar(128, 180, 10); //green

	cv::Mat frameGray;
	cv::cvtColor(img, frameGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(frameGray, frameGray);
	//detect faces
	mFaceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		cv::ellipse(img, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, faceEllipseColor, 4, 8, 0);

		cv::Mat faceROI = frameGray(faces[i]);
		std::vector<cv::Rect> eyes;
		//-- In each face, detect eyes
		mEyesCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));	
		for (size_t j = 0; j < eyes.size(); j++)
		{
			cv::Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			cv::circle(img, center, radius, eyeEllipseColor, 4, 8, 0);
		}
		
	}
	
	imshow("result", img);

}