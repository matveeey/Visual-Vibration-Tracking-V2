#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// standart headers
#include <iostream>
//#include <sstream>

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
	// ���������, ������������� �� ����� ���������� ����
	bool IsInteracted(int x, int interval);
	// ������� �������� ������� ����� � ������
	void PlotMouseValue(Mat& frame, int value_idx);
	// ��� ����������� ���-�� ������
	std::string to_string_with_precision(const float value, const int n = 6);

protected:
	// ���� ��� ��������� �����������
	bool is_histogram_plotted_;

private:
	// �������� ����
	std::string winname_;
	// ������ ����� �����������
	float histogram_offset_;
	// ������ � ������ ���� ����������� (������� ������)
	int histogram_frame_width_;
	int histogram_frame_height_;
	// ���� �����������
	Mat histogram_;
	// �������� �� x, y
	std::vector<double> x_values_;
	std::vector<float> y_values_;
	// ������������ ���������
	std::vector<int> values_accordance_;
	// ������������ (����������) �������� �� ��� X
	int x_limit_;
	// ���������� ����
	Point2i last_mouse_coordinates_;


};

#endif