#pragma once
#include<opencv2/opencv.hpp>
#include <string>


const std::string defaultFaceCascadePath = std::string("data/haarcascades/haarcascade_frontalface_alt.xml");
const std::string defaultSmileCascadePath = std::string("data/haarcascades/haarcascade_smile.xml");
const std::string defaultEyesCascadePath = std::string("data/haarcascades/haarcascade_eye.xml");

class SmileDetector
{
public:
	SmileDetector(const cv::Mat& image, const std::string&faceCascadePath= defaultFaceCascadePath, 
		const std::string&smileCascadePath = defaultSmileCascadePath,
		const std::string&eyesCascadePath = defaultEyesCascadePath);
	~SmileDetector();

	void drawFaceOutline();
	void detectSmile(cv::Mat& img);
	void detectFaceAndEyes(cv::Mat & img);
private:

	cv::Mat mImage;
	cv::CascadeClassifier mFaceCascade, mSmileCascade, mEyesCascade;
};

