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
	PointHandler(int frequency_update_rate);
	// ��������� ����� ���������� ����� �����
	void AddPosition(Point2f position);
	// ��������� ����� ����������� �������� ������� ����� �����
	void AddFrequency(double frequency);
	// ��������� ����� �������� ������� ����� ��� ��������� ����� �����
	void AddFrameTimePos(double time);

private:
	int frequency_update_rate_;

	std::vector<Point2f> positions_;
	std::vector<double> frequencies_;
	std::vector<double> frame_time_positions_;
};

#endif