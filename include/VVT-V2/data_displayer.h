#ifndef DATA_DISPLAYER_H_
#define DATA_DISPLAYER_H_

// OpenCV headers
#include <opencv2/opencv.hpp>

// standart headers
#include <iostream>

// my headers
#include "VVT-V2/point_handler.h"

using namespace cv;

class DataDisplayer
{
public:
	DataDisplayer(std::vector<PointHandler>& vec_point_handlers, std::vector<Point2f> previous_points_coordinates, Point2i last_mouse_position);

	// Отрисуем данные
	void DrawData(Mat& frame);

	//void ContourHandler(std::vector<std::vector<Point>> contour_shapes);
	//void GetContourHeadPoints(std::vector<std::vector<Point>> contour_shapes, std::vector<Point>& contour_head_points);
	void ProcessFrame(Mat tmp_frame);
	Mat GetFrame();
	void SetRoi(Rect roi);
	void UpdateFrequencies(std::vector<float> frequencies, double range);
	void UpdateDisplayingPoints(std::vector<Point2f> points);
	void SetMode(int mode);

private:
	Mat AddTipText(Mat frame);
	void UpdateDisplayingRectangle();
	void UpdateColors();
	void AddGradient();

public:
	// Getting color value from gradient
	std::vector<int> Rgb(double ratio);
private:
	// Для вывода данных
	std::vector<PointHandler>& vec_point_handlers_;
	std::vector<Point2f> previous_points_coordinates_;
	Point2i last_mouse_position_;

	Mat tmp_frame_;
	int mode_;

	std::string window_name_;
	int frame_width_;
	int frame_height_;

	std::vector<Point2f> points_;
	std::vector<float> frequencies_;
	std::vector<Scalar> colors_;
	double range_;

	Mat frame_;
	Rect roi_;
};

#endif