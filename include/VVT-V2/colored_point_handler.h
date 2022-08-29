#ifndef COLORED_POINT_HANDLER_H
#define COLORED_POINT_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/vibrating_point.h"
#include "VVT-V2/histogram.h"
#include "VVT-V2/output_to_csv.h"
#include "VVT-V2/helper.h"

using namespace cv;

enum mode
{
	DEFAULT,
	COLORING_BASED_ON_AMPLITUDE,
	COLORING_BASED_ON_FREQUENCY
};

// ����� ��������� ������-��������� ����� � ������������ ��������� ������ � ����������� �� ��������� ��� ������� ��������
class ColoredPointHandler : public Histogram, public VibratingPoint, public OutputToCsv
{
public:
	ColoredPointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient, std::string output_csv_filename);
	~ColoredPointHandler();
	// ������������ ����� � ������, ��������� � ���
	void Draw(Mat& frame) override;
	// ������������� ������ ����� ��������� ������
	void SetMode(int mode);

private:
	// ������������ �����������
	void DrawHistogram() override;
	// ������������ ����� �� �����
	void DrawPoint(Mat& frame);
	// ������������ ����� ����������� �����
	void DrawInteractionRectangle(Mat& frame);

private:
	// ��������� ������� ���� �����
	void UpdatePointColor() override;

private:
	// ������� ����� ������ (����� �� ������ ��������/������)
	int mode_;
	// ����������� �������� ��� ��������� ������
	float resizing_coefficient_;
	// ID ����� (��� ���� �����)
	int point_id_;
	// ������� ���������� �������, ����� � ��
	int update_rate_;
	// ���� �����
	Scalar point_color_;
	// ����, ���������� �� ��� ������� �����

};

#endif