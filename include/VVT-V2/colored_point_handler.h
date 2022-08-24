#ifndef COLORED_POINT_HANDLER_H
#define COLORED_POINT_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/vibrating_point.h"
#include "VVT-V2/histogram.h"

using namespace cv;

class ColoredPointHandler : public Histogram, public VibratingPoint
{
	ColoredPointHandler();
};

#endif