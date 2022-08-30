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

/*!
* @brief �������� (�� VibratingPoint) �����. ������������ ��� ��������� ��������� ������� ����� �� ���� � ������������ �������� ������ �� ���
*
* ������ ������ - "�������" ����� �� �����, ��������� ������� ������������� � �������� ������� ��� ���������� ������� �������� ���� �����
*
* ����� ����� ���������� ������� Draw() �� �����
*
* ����� ��� �������������� � ������ (VibratingPoint::IsInteracted()) ����� ���������� ����������� ������ � ��������� ���� (DrawHistogram())
*/
class ColoredPointHandler : public Histogram, public VibratingPoint, public OutputToCsv
{
	/*!
	* @brief ������������, ������������ ��� ����� �������� ����������� �������� ������ ��������� �����
	*/
	enum coloring_mode
	{
		DEFAULT,
		COLORING_BASED_ON_AMPLITUDE,
		COLORING_BASED_ON_FREQUENCY
	};

public:
	/*!
	* @brief ����������� ����� ������
	* @param init_coordinates: ����� ������ ���������� ����� (���������� - ���������� ����� ����� �� ����, ���� ���������� �������� �����)
	* @param sampling_rate: ������� ������������� (���������� - FPS ��������� �����)
	* @param point_id: ID �����
	* @param radius_resize_factor: ����������� ��������������� ������� �������������� �����
	* @param output_csv_filename: ��� (����) csv-����� ��� ������ ����������
	* @note ����������� ���������������: FrameHandler::InitTextResizeFactors()
	*/
	ColoredPointHandler(Point2f init_coordinates, double sampling_rate, int point_id, float radius_resize_factor, std::string output_csv_filename);
	/*!
	* @brief ���������� ����� ������
	*
	* �������� ����� ������������� ������ OutputToCsv Write() ��� ������ ����������
	*/
	~ColoredPointHandler();
	/*!
	* @brief ������������ ����� � ������, ��������� � ���
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	void Draw(Mat& frame) override;
	/*!
	* @brief ������������� ������ ����� ��������� ������
	* @param coloring_mode: �����, ������� ����� ���������� 
	* @note ColoredPointHandler::coloring_mode
	*/
	void SetColoringMode(int coloring_mode);

private:
	/*!
	* @brief ������������ �����������
	*/
	void DrawHistogram() override;
	/*!
	* @brief ������������ ����� �� �����
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	void DrawPoint(Mat& frame);
	/*!
	* @brief ������������ ������������� ��������������
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	void DrawInteractionRectangle(Mat& frame);

private:
	/*!
	* @brief ��������� ������� ���� �����
	*/
	void UpdatePointColor() override;

private:
	// ������� ����� ������ (����� �� ������ ��������/������)
	int coloring_mode_;
	/*!
	* @brief ����������� �������� ��� ������� �����
	*/
	float radius_resize_factor_;
	/*!
	* @brief ID ����� (��� ���� �����)
	*/
	int point_id_;
	/*!
	* @brief ���� �����
	*/
	Scalar point_color_;

};

#endif