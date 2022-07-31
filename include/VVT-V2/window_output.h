#ifndef WINDOW_OUTPUT_H
#define WINDOW_OUTPUT_H

// OpenCV headers
#include <opencv2/opencv.hpp>

// standart headers
#include <iostream>

using namespace cv;

class WindowOutput
{
public:
	WindowOutput(std::string window_name);
	void DisplayFrame(Mat frame);

private:
	std::string window_name_;
};

#endif