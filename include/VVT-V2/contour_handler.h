#ifndef CONTOUR_HANDLER_H
#define CONTOUR_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

class ContourHandler
{
public:
	ContourHandler(Mat frame, Rect ROI);
	void DrawContours(Mat& frame);
	std::vector<std::vector<Point2i>> GetContours();
	std::vector<Point2f> GetContinousContours();

private:
	std::vector<std::vector<Point2i>> TranslateCoords(Point2i offset, std::vector<std::vector<Point2i>> contour_shapes);

private:
	std::vector<std::vector<Point>>contour_shapes_;

};

#endif