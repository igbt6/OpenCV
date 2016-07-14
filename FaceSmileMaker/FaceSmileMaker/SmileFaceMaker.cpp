#include "stdafx.h"
#include "SmileFaceMaker.h"

SmileFaceMaker::SmileFaceMaker(const cv::Mat & image)
{
	mImage = image;
}

SmileFaceMaker::~SmileFaceMaker()
{
}

void SmileFaceMaker::drawFaceOutline()
{
	cv::Size size = mImage.size();
	int imgWidth = size.width;
	int imgHeight = size.height;

	cv::Mat faceOutline = cv::Mat::zeros(size, mImage.type());
	cv::Scalar color = CV_RGB(200, 100,200);   // black
	int thickness = 4;
	int faceHeight = (imgHeight/2) * (70.f / 100.f);
	int faceWidth = (imgWidth/2 ) * (70.f / 100.f);

	cv::ellipse(faceOutline, cv::Point(imgWidth / 2, imgHeight / 2), cv::Size(faceWidth, faceHeight), 0, 0, 360, color, thickness, CV_AA);

	// Draw the eye outlines, as 2 half ellipses.
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
	
	//DEBUG
	//cv::namedWindow("faceOutlineImage", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window												
	//cv::imshow("faceOutlineImage", faceOutline);		// show windows
}
