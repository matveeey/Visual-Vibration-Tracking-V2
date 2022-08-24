#ifndef POINT_HANDLER_H
#define POINT_HANDLER_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/histogram.h"

// other headers
#include "VVT-V2/PeakFinder.h"

using namespace cv;

class PointHandler : public Histogram
{
public:
	PointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient);
	~PointHandler();
	// ��������� ��� (FFT) ��� ��������� (�������) �����
	void ExecuteFft();
	// ��������� ������ ��� ������ ����� ���������
	void Filter(std::vector<float>& magnitudes);
	// ���������� true � ������ ��������� ��������� �� �������� �������� � interaction_box �����
	bool IsInteracted(Point2i coordinates);
	// ��������� ����� ���������� ����� �����
	void AddNewCoordinate(Point2f position);
	// ��������� ����� �������� ������� ����� ��� ��������� ����� �����
	void AddFrameTimePos(double time);
	// ������������ ����� � ������, ��������� � ���
	void Draw(Mat& frame);

private:
	// ������������ �����������
	void DrawHistogram();
	// ������������ ����� �� �����
	void DrawPoint(Mat& frame);
	// ������������ ����� ����������� �����
	void DrawPointTrack(Mat& frame);
	// ������������ ������������� ��������������
	void DrawInteractionRectangle(Mat& frame);
	// ������������ ��������� ������ ����� � ������
	void DrawTextData(Mat& frame);


// �������
public:
	// TEMPORARY
	std::vector<double> GetX();
	std::vector<float> GetY();
	// ���������� ��������� ������� ������� ���������
	Point2f GetLastFoundCoordinates();
	// ���������� ������� ������� �����
	std::vector<double> GetCurrentVibrationFrequency();
	// ���������� ������� ��������� ���������
	Point3f GetCurrentAmplitude();
	// ���������� �����, ����������� ��� ����������� ���������
	Point2f GetTextCoordinates();
	// ���������� ���������� �������������� ��������������
	Rect GetInteractionRect();

private:
	// ��������� ������� ���� �����
	void UpdatePointColor();
private:
	// ����������� �������� ��� ��������� ������
	float resizing_coefficient_;
	// ID ����� (��� ���� �����)
	int point_id_;
	// ������� ���������� �������, ����� � ��
	int update_rate_;
	// ������������� ��� �������������� � ������
	Rect interaction_box_;
	// �������� ����� �������������� ��������������
	int interaction_offset_;
	// True - � ������ ��������������, false - � ���������
	bool interacted_;
	// ���� �����
	Scalar point_color_;
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