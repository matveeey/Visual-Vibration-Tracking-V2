#ifndef VIBRATING_POINT_H
#define VIBRATING_POINT_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// other headers
#include "VVT-V2/peak_finder.h"

using namespace cv;

/*!
* @brief ����������� ����� ��� �������� ����������� �����
* 
* ������ ������-���������� ����� ����� ����������� ���������� ����� ���������������� � ��������� �������, ���������� ������� �������� ����� �� ����� �� ������ ���� ������ (ExecuteFFT())
* � ����� ��������� ����������� �������� ������� �������������� � ������ (����� IsInteracted())
*/
class VibratingPoint
{
public:
	/*!
	* @brief ��������� ��� ��� ���������� ������� ��������.
	* 
	����� ���������� ���������� � ��������� frequencies_ ��������� ����, � ����� ��������� � ��������� x_ ��������� ������ ������ �������� (��� ����������� �����������)
	*/
	void ExecuteFFT();
	/*!
	* @brief ��������� ������ ��� ������ ����� ���������
	* @param magnitudes: ������, � ������� ����� ��������� ����������
	*/
	template<class T>
	void DeadzoneFilter(std::vector<T>& input_vector);
	/*!
	* @brief ��������� ����� ���������������� ���������� � ����� ���������� VibratingPoint::point_coordinates_;
	* @param position: ���������� �����
	*/
	void AddNewPointPosition(Point2f position);
	/*!
	* @brief ��������� ����� ��������� ���������� (����� �����, �� ������� ����� ������� ����� ���� ��������) � ����� ���������� VibratingPoint::point_time_coordinates_;
	* @param time: ����� � �������������
	*/
	void AddNewPointTime(double time);
	/*!
	* @brief ��������� ����� �� ������� �������������� � ��� (���� ������ ����� � ������� �������������� ����� VibratingPoint::interaction_box_)
	* @param coordinates: ���������� �������
	*/
	bool IsInteracted(Point2i coordinates);
	/*!
	* @brief ������������� ������������ ��������� �������� ��������� ����� � �����
	* @note ���������� ��� ���������� ����� "�������" �����, � ����� ��� ���������� ������������� ��������� �������� "��������" (lonely) �����
	* @param amplitude: ��������� ������������ ���������, ������� ����� �����������
	*/
	void SetMaxAmplitude(double amplitude);
	/*!
	* @brief ������������ ����� �� �����
	* @param frame: �����������, �� ������� ����� ����������� ���������
	*/
	virtual void Draw(Mat& frame) = 0;

protected:
	/*!
	* @brief ������������ ����������� ������� ������ ��������� ����� (���������� ����� ��� � ������ ExecuteFFT())
	*/
	virtual void DrawHistogram() = 0;
	/*!
	* @brief ��������� ������� ���� �����
	*/
	virtual void UpdatePointColor() = 0;
	/*!
	* @brief 
	*/


public:
	/*!
	* @brief ���������� ��������� ������� ������� ���������
	*/
	Point2f GetLastFoundCoordinates();
	/*!
	* @brief ���������� ������ ������� ������ �����
	*/
	std::vector<double> GetCurrentVibrationFrequency();
	/*!
	* @brief ���������� ��������� ��������� ���������
	*/
	Point3f GetCurrentAmplitude();

protected:
	/*!
	* @brief ������������� (�������) �������������� � ������
	*/
	Rect interaction_box_;
	/*!
	* @brief �������� ����� �������������� ��������������
	*/
	int interaction_offset_;
	/*!
	* @brief ���� �������������� � ������
	* 
	* True - � ������ ��������������, false - � ���������
	*/
	bool interacted_;
	/*!
	* @brief ��������� �������� �� x, y (������������� � �� z, �� ���� z �� �������)
	*/
	Point3f amplitude_;
	/*!
	* @brief ��������� ��� ��������� �����
	*/
	std::vector<Point2f> point_coordinates_;
	/*!
	* @brief ���������������� ����������� ��������
	*/
	double sensivity_;

	// ���� �������� double confidence_ � ���� ����� (��������� ����� ����� �����������) (visual studio 2022, ����� ������ ���� ��������), ��������� ����� ������ :/ 
	// ������ ���?

	/*!
	* @brief ��������� ��� ������ �����
	*/
	std::vector<double> frequencies_;
	/*!
	* @brief ��������� ��� ��������� ��������� �����
	*/
	std::vector<double> point_time_coordinates_;
	/*!
	* @brief ������� ������������� (���������� - FPS ��������� �����)
	*/
	double sampling_rate_;
	/*!
	* @brief ������ �����, ������� ����� ��� � ���������
	*/
	int point_radius_;
	/*!
	* @brief ������� ������������ ���������
	*/
	double max_amplitude_;

	/*!
	* @brief ��������� ��� �������� ��������� ����������� �� ��� X (���������� - ��������� ������ ������ �������� ����� ����� ���������� ���)
	*/
	std::vector<double> x_;
	/*!
	* @brief ��������� ��� �������� ��������� ����������� �� ��� Y (���������� - ��������� ��������� ����� ����� ���������� ���)
	*/
	std::vector<float> y_;
	/*!
	* @brief ������� ������������� ��������
	*/
	double confidence_;
};

#endif