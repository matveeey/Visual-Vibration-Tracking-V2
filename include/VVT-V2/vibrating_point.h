#ifndef VIBRATING_POINT_H
#define VIBRATING_POINT_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// other headers
#include "VVT-V2/PeakFinder.h"

using namespace cv;

// ����������� ����� ��� �������� ����������� �����
class VibratingPoint
{
public:
	void ExecuteFFT();
	// ��������� ����� ���������������� ����������
	void AddNewPointPosition(Point2f position);
	// ��������� ����� ��������� ����������
	void AddNewPointTime(double time);
	// ��a������ ����� �� ������� ��������������
	bool IsInteracted(Point2i coordinates);
	// ������������ ����� �� �����
	virtual void Draw(Mat& frame) = 0;

protected:
	// ������������ ����������� �������� ������ ��������� �����
	virtual void DrawHistogram() = 0;
	// ��������� ������� ���� �����
	virtual void UpdatePointColor() = 0;

public:
	// ���������� ��������� ������� ������� ���������
	Point2f GetLastFoundCoordinates();
	// ���������� ������� ������� �����
	std::vector<double> GetCurrentVibrationFrequency();
	// ���������� ������� ��������� ���������
	Point3f GetCurrentAmplitude();

protected:
	// ������������� ��� �������������� � ������
	Rect interaction_box_;
	// �������� ����� �������������� ��������������
	int interaction_offset_;
	// True - � ������ ��������������, false - � ���������
	bool interacted_;
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