#pragma once
#include<opencv2/opencv.hpp>
class SmileFaceMaker
{
public:
	SmileFaceMaker(const cv::Mat& image);
	~SmileFaceMaker();

	void drawFaceOutline();

private:
	cv::Mat mImage;
};

