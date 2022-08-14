#ifndef AMPLITUDE_HANDLER_H
#define AMPLITUDE_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class AmplitudeHandler
{
public:
	AmplitudeHandler();
	~AmplitudeHandler();
	void CollectTrackedPoints(Point2f coordinates_of_points);
	Point2f CalculateAmplitude();
	Mat GetWarpedFrame(Mat frame, Rect rectangle, float w, float h);

private:
	double DistanceBtwn2Points(Point2f first_point, Point2f second_point);

private:
	std::vector<Point2f> coordinates_of_point_;
	std::vector<Point2f> amplitudes_;
};

#endif