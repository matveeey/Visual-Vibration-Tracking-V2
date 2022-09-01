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
* 
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
	void UpdateMaxAmplitudeOverall(double amplitude);
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

private:
	/*!
	* @brief ��������� ������� �������� �������� ��������� �������
	* @param src - ������� ������ ��� �������
	*/
	template<typename T>
	T CalculateMeanDifferenceInVector(std::vector<T> src);
	/*!
	* @brief ��������� ������������ �������� �������� ��������� �������
	* @param src - ������� ������ ��� �������
	*/
	template<typename T>
	T CalculateMaxDifferenceInVector(std::vector<T> src);
	/*!
	* @brief ��������� �������� �������
	* @param src - ������� ������ ��� �������
	*/
	template<typename T>
	int FindGlobalMaxIdx(std::vector<T> src);

private:
	std::vector<float> max_differences_;

// �������
public:
	void SetSensivity(double sensivity);

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
	* @brief ���������� ��������� ��������� ���������
	*/
	Point3f GetCurrentAmplitude();
	/*!
	* @brief ���������� ��������� ��������� ������� �������������
	*/
	double GetCurrentConfidenceLevel();

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

	// ���� �������� double confidence_level_ � ���� ����� (��������� ����� ����� �����������) (visual studio 2022, ����� ������ ���� ��������), ��������� ����� ������ :/ 
	// ������ ���?

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
	* @brief ���������������� ����������� ��������
	*/
	double sensivity_;
	/*!
	* @brief ������� ������������� ��������
	*/
	double confidence_level_;
};

#endif