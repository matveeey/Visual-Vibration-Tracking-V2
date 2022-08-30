#ifndef LONELY_POINT_HANDLER_H
#define LONELY_POINT_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/vibrating_point.h"
#include "VVT-V2/histogram.h"
#include "VVT-V2/output_to_csv.h"

using namespace cv;

/*!
* @brief �������� (�� VibratingPoint) �����. ������������ ��� ��������� ����� �� ���� � ������������ � �������� ������������
* 
* ������ ������ - "��������" ����� �� �����, ��������� ������� ������������� � �������� ������� ��� ���������� ������� �������� ���� �����
* 
* ����� ����� ���������� ������� Draw() �� �����
* 
* ����� ��� �������������� � ������ (VibratingPoint::IsInteracted()) ����� ���������� ����������� ������ � ��������� ���� (DrawHistogram())
*/
class LonelyPointHandler : public Histogram, public VibratingPoint, public OutputToCsv
{
public:
	/*!
	* @brief ����������� ����� ������
	* @param init_coordinates: ����� ������ ���������� ����� (���������� - ���������� ����� ����� �� ����, ���� ���������� �������� �����)
	* @param sampling_rate: ������� ������������� (���������� - FPS ��������� �����)
	* @param point_id: ID �����
	* @param text_resize_factor: ����������� ��������������� ������, ���������� �� ����������� ��� ��������� ������� � ������������� ��������� ��������
	* @param output_csv_filename: ��� (����) csv-����� ��� ������ ����������
	* @note ����������� ���������������: FrameHandler::InitTextResizeFactors()
	*/
	LonelyPointHandler(Point2f init_coordinates, double sampling_rate, int point_id, float text_resize_factor, std::string output_csv_filename);
	/*!
	* @brief ���������� ����� ������
	* 
	* �������� ����� ������������� ������ OutputToCsv Write() ��� ������ ����������
	*/
	~LonelyPointHandler();
	/*!
	* @brief ��������� ������ ��� ������ ����� ���������
	* @param magnitudes: ������, � ������� ����� ��������� ����������
	*/
	void Filter(std::vector<float>& magnitudes);
	/*!
	* @brief ������������ ����� � ������, ��������� � ���
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	void Draw(Mat& frame) override;

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
	* @brief ������������ ����� ����������� (����) �����
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	void DrawPointTrack(Mat& frame);
	/*!
	* @brief ������������ ������������� ��������������
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	void DrawInteractionRectangle(Mat& frame);
	/*!
	* @brief ������������ ��������� ������ ����� � ������
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	void DrawTextData(Mat& frame);


// �������
public:
	/*!
	* @brief ���������� �����, ����������� ��� ����������� ���������
	* @return ���������� ������ ��� �����������
	*/
	Point2f GetTextCoordinates();
	/*!
	* @brief ���������� ���������� �������������� ��������������
	* @return ������������� (�������) ��������������
	*/
	Rect GetInteractionRect();

private:
	/*!
	* @brief ��������� ������� ���� �����
	*/
	void UpdatePointColor() override;
private:
	/*!
	* @brief ����������� �������� ��� ��������� ������
	*/
	float text_resize_factor_;
	/*!
	* @brief ID ����� (��� ���� �����)
	*/
	int point_id_;
	/*!
	* @brief ������� ���������� �������, ����� � ��
	*/
	int update_rate_;
	/*!
	* @brief ���� �����
	*/
	Scalar point_color_;

};

#endif