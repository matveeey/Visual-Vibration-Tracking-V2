#ifndef CONTOUR_HANDLER_H
#define CONTOUR_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/helper.h"

using namespace cv;

/*!
* @brief �������� (�� VibratingPoint) �����. ������������ ��� ��������� ��������� ������� ����� �� ���� � ������������ �������� ������ �� ���
*
* ������ ������ - "�������" ����� �� �����, ��������� ������� ������������� � �������� ������� ��� ���������� ������� �������� ���� �����
*
* ����� ����� ���������� ������� Draw() �� �����
*
* ����� ��� �������������� � ������ (VibratingPoint::IsInteracted()) ����� ���������� ����������� ������ � ��������� ���� (DrawHistogram())
*/

class ContourHandler
{
public:
	ContourHandler(Mat frame, Rect ROI);
	void DrawContours(Mat& frame);
	std::vector<std::vector<Point2i>> GetContours();
	std::vector<Point2f> GetContinousContours();

private:
	template<typename T>
	std::vector<T> MakeTwiceThinner(std::vector<T> src);

	template<typename T>
	float CalculateDistance(T point1, T point2);

private:
	std::vector<std::vector<Point>>contour_shapes_;
	float minimal_distance_;

};

#endif
