#ifndef POINT_HANDLER_H
#define POINT_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

class PointHandler
{
public:
	PointHandler();

private:
	std::vector<Point2f> positions_;
	std::vector<double> frequencies_;

};

#endif