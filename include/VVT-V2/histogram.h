#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

// ����� ������������ ��� �������� ��������-����������, �� ������� ����� �������� ��������� ����������
class Histogram
{
public:
	Histogram(int width, int height, int x_limit);

	// ������������ �����������
	void plot_histogram();
	// ������������� X-�
	void set_x_values(std::vector<double> x_values);
	// ������������� Y-�
	void set_y_values(std::vector<float> y_values);

private:
	// ��������� �������� ��� �����������
	Mat calc_histogram();

private:
	float histogram_offset_;
	int histogram_frame_width_;
	int histogram_frame_height_;
	Mat histogram_;

	std::vector<double> x_values_;
	std::vector<float> y_values_;

	int x_limit_;


};

#endif