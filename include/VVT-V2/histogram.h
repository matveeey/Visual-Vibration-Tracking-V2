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
	/*Histogram();*/
	Histogram(int width, int height, int x_limit, int id);
	~Histogram();

	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// ������������ �����������
	void plot_histogram();
	// ������������� X-�
	void set_x_values(std::vector<double> x_values);
	// ������������� Y-�
	void set_y_values(std::vector<float> y_values);
	// ��������� ������ ����� ��������� �����������
	void SetHistogramFlag(bool flag);

private:
	// ��������� �������� ��� �����������
	Mat calc_histogram();

protected:
	// ���� ��� ��������� �����������
	bool is_histogram_plotted_;

private:
	// �������� ����
	std::string winname_;

	float histogram_offset_;
	int histogram_frame_width_;
	int histogram_frame_height_;
	Mat histogram_;

	std::vector<double> x_values_;
	std::vector<float> y_values_;
	std::vector<int> values_accordance_;

	int x_limit_;


};

#endif