// FaceSmileMaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SmileFaceMaker.h"



#include<iostream>
#include<string>

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	cv::Mat testImage;		// input image
	cv::waitKey(0);
	const std::string testImgName("img.png");
	testImage = cv::imread(testImgName);			// open image

	SmileFaceMaker faceMaker(testImage);

	if (testImage.empty()) {									// if unable to open image
		std::cout << "error: cannot found  file\n\n";		// show error message on command line
		cv::waitKey(0);
		return 0;											// and exit program
	}


	//declare main window
	cv::namedWindow("testImage", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window												
	cv::imshow("testImage", testImage);		// show windows
	faceMaker.drawFaceOutline();
	cv::namedWindow("testImage", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window												
	cv::imshow("testImage", testImage);		// show windows
	cv::waitKey(0);				

	return(0);
}

