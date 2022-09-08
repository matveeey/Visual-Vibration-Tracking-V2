#ifndef VIBRATING_POINT_H
#define VIBRATING_POINT_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// other headers
#include "VVT-V2/peak_finder.h"
#include "VVT-V2/helper.h"

using namespace cv;

/*!
* @brief ����������� ����� ��� �������� ����������� �����
* 
* ������ ������-���������� ����� ����� ����������� ���������� ����� ���������������� � ��������� �������, ���������� ������� �������� ����� �� ����� �� ������ ���� ������ (ExecuteFFT())
* 
* � ����� ��������� ����������� �������� ������� �������������� � ������ (����� IsInteracted())
*/
class VibratingPoint
{
public:
	void UpdateExtremumAmplitude(double extremum_amplitude);
	/*!
	* @brief ��������� ��� ��� ���������� ������� ��������.
	* 
	����� ���������� ���������� � ��������� frequencies_ ��������� ����, � ����� ��������� � ��������� x_ ��������� ������ ������ �������� (��� ����������� �����������)
	*/
	void ExecuteFFT();
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
	* @brief ������������� ����������������, �������� �� confidence
	*/
	void SetSensivity(double sensivity);
	/*!
	* @brief �������� ����������� ��� ���������� ��������� ����������
	*/
	void CalculateAmplitude();
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


// �������
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
	* @brief ���������� ������� "��������" �������
	*/
	double GetCurrentMainFrequency();
	/*!
	* @brief ���������� ��������� ��������� ������������� ���������
	*/
	Point3f GetRelativeAmplitude();
	/*!
	* @brief ���������� ��������� ��������� ������������� ���������
	*/
	Point3f GetCurrentAmplitude();
	/*!
	* @brief ���������� ��������� ��������� ������� �������������
	*/
	double GetCurrentConfidenceLevel();
	
public:
	/*!
	* @brief �������� � ������� ��������� ����� ���� VibratingPoint ��������-�����������
	* 
	* inline - ��� ������������ ���������� � ������������ ���������� ������ ������
	*/
	static inline std::pair<double, double> extremum_amplitude_;

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
	* @brief ������������� ��������� �������� �� x, y. ���������� ���������� ���������
	*/
	Point3f relative_amplitude_;
	/*!
	* @brief ����������� ��������� �������� �� x, y. ����������� � ��������
	*/
	Point3f current_amplitude_;
	/*!
	* @brief ��������� ��� ��������� �����
	*/
	std::vector<Point2f> point_coordinates_;
	/*!
	* @brief ��������� ��� ��������� ����� �� ��� X
	*/
	std::vector<double> x_coordinates_;
	/*!
	* @brief ��������� ��� ��������� ����� �� ��� Y
	*/
	std::vector<double> y_coordinates_;
	/*!
	* @brief ��������� ��� ��������� ����� ������������ �������� ������
	*/
	std::vector<cv::Point2f> vec_meaned_coordinates_of_point_;
	/*!
	* @brief ��������� ��� ������ �����
	*/
	std::vector<double> frequencies_;
	/*!
	* @brief ������� ������� (��������)
	*/
	double main_frequency_;
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
	int current_point_radius_;
	/*!
	* @brief ��������� ������ �����
	*/
	int default_point_radius_;

	/*!
	* @brief ��������� ��� �������� ��������� ����������� �� ��� X (���������� - ��������� ������ ������ �������� ����� ����� ���������� ���)
	*/
	std::vector<double> x_;
	/*!
	* @brief ��������� ��� �������� ��������� ����������� �� ��� Y (���������� - ��������� ��������� ����� ����� ���������� ���)
	*/
	std::vector<float> y_;
	/*!
	* @brief ���������������� ����������� ��������
	*/
	double sensivity_;
	/*!
	* @brief ������� ������������� ��������
	*/
	double confidence_level_;

private:
	/*!
	* @brief ������������ ������� � ���������� � ��������� ������� ����� ���
	* @see ExecuteFFT() � confidence_level_ - ������������ ��� ����������� "������ �������������" ����������� � ���������� �����
	*/
	std::vector<float> mag_max_differences_;
};

#endif