#ifndef POINT_HANDLER_H
#define POINT_HANDLER_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// other headers
#include "VVT-V2/PeakFinder.h"

using namespace cv;

class PointHandler
{
public:
	PointHandler(Point2f init_coordinates, int update_rate, double sampling_rate);
	~PointHandler();
	// ��������� ��� (FFT) ��� ��������� (�������) �����
	void ExecuteFft();
	// ���������� ��������� ������� ������� ���������
	Point2f GetLastFoundCoordinates();
	// ���������� ������� ������� �����
	std::vector<double> GetCurrentVibrationFrequency();
	// ���������� ������� ��������� ���������
	Point3f GetCurrentAmplitude();
	// ���������� �����, ����������� ��� ����������� ���������
	Point2f GetTextCoordinates();
	// ���������� true � ������ ��������� ��������� �� �������� �������� � interaction_box �����
	bool IsInteracted(Point2i coordinates);
	// ��������� ����� ���������� ����� �����
	void AddNewCoordinate(Point2f position);
	// ��������� ����� �������� ������� ����� ��� ��������� ����� �����
	void AddFrameTimePos(double time);

	// TEMPORARY
	std::vector<double> GetX();
	std::vector<float> GetY();

private:

private:
	// ������� ���������� �������, ����� � ��
	int update_rate_;
	// ������������� ��� �������������� � ������
	Rect interaction_box_;
	// �������� ����� �������������� ��������������
	int interaction_offset_;
	// ��������� �������� �� x, y, z
	Point3f amplitude_;
	// ���������� ��� ��������� (�������) �����, � ������ � ��������� ���������
	std::vector<Point2f> point_coordinates_;
	std::vector<double> frequencies_;
	std::vector<double> point_time_coordinates_;
	// ��������� ��� (FFT)
	double sampling_rate_;

	// TEMPORARY
	std::vector<double> x_;
	std::vector<float> y_;

};

#endif