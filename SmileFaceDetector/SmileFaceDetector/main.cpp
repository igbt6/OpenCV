// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SmileDetector.h"
#include<iostream>
#include<string>

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	cv::Mat testImage;	
	cv::waitKey(0);
	const std::string testImgName("img2.jpg");
	testImage = cv::imread(testImgName);		
	if (testImage.empty()) {
		std::cout << "error: cannot found  file\n\n";
		cv::waitKey(0);
		return -1;
	}

	try {
		SmileDetector smileFaceDetector(testImage);
		//declare main window
		smileFaceDetector.detectSmile(testImage);
		//smileFaceDetector.detectFaceAndEyes(testImage);
		cv::namedWindow("testImage", CV_WINDOW_AUTOSIZE);
		cv::imshow("testImage", testImage);		// show windows
	}
	catch (const std::invalid_argument& ex) {
		std::cerr << "[ERROR]: " << ex.what() << '\n';
		return -1;
	}

	cv::waitKey(0);
	return 0;
}

