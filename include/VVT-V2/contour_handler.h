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
* @brief ����� ��� �������� ��������-������������ ��������.
* 
* �������� ����� ������� �� ������� ����������� (����������� � ������������), ���������� ��, � ���������� ������������ �������� ��� ���� (��������, ��������� ������
* ��� ��������� ����� � �������
*/

class ContourHandler
{
public:
	/*!
	* @brief ����������� ����� ������
	* @param frame: Mat-instance �����������, �� ������� ����� �������� ����� ��������. ����������� ����� ���� � RGB/gray-scale (� ������ ������ ����� ��������������� ������)
	* @param ROI: ������ ��������, �� ������� ����� ������� �������. 
	*/
	ContourHandler(Mat frame, Rect* ROI = nullptr);
	/*!
	* @brief ������������ ������� �� �����������
	* @param frame: ����� �����������, �� ������� ����� ���������� ��������� �������
	*/
	void DrawContours(Mat& frame);
	/*!
	* @brief ���������� ��������� ������� � ���� { { contour_1 }, { contour_2 }, ... , { contour_n } }
	* @return �������
	*/
	std::vector<std::vector<Point2i>> GetContours();
	/*!
	* @brief ���������� ��������� ������� � ���� { contour_1, contour_2, ... , contour_n }.
	* 
	* ���������� ��������� "����������" ��������� �������� � ���� ������
	* @return ����������� � ������������� ���������
	*/
	std::vector<Point2f> GetContinousContours();

private:
	/*!
	* @brief ������� ������ ������ ������� ������� (��������� ������ � ��� ����)
	* @param src: ������, ������� ����� "�������"
	*/
	template<typename T>
	std::vector<T> MakeTwiceThinner(std::vector<T> src);
	/*!
	* @brief ��������� ���������� ����� ����� �������
	* @param point1: ������ �����
	* @param point2: ������ �����
	* @return ���������� ����� �������
	*/
	template<typename T>
	float CalculateDistance(T point1, T point2);

private:
	/*!
	* @brief ������� � ���� { { contour_1 }, { contour_2 }, ... , { contour_n } }
	*/
	std::vector<std::vector<Point>>contour_shapes_;
	/*!
	* @brief ����������� ���������� ����� ������� �������, ��� ������� "�����" ("������") ����� ����� ���������
	* @see ������������ � GetContinousContours()
	*/
	float minimal_distance_;

};

#endif
