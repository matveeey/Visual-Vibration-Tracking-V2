#ifndef EULERIAN_ENLARGEMENT_H
#define EULERIAN_ENLARGEMENT_H

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

/*!
* @brief ����� ��� ����������� ���������� �������� �� �����
*/
class EulerEnlarger
{
protected:
	EulerEnlarger();
	~EulerEnlarger();

protected:
	/*!
	* @brief ������������ ���������� �������� �� ���������� ������ ��������
	*/
	void LaplaceEnlarge(Mat& motion_pyramid_level, int current_level);
	/*!
	* @brief "����������" ��������� ����� �� �����
	*/
	void Attenuate(Mat& motion_frame);

	// ���� � ����� �� �������. �������
	/*!
	* @brief ������� �������� �����������
	*/
	void BuildLaplacePyramid(const Mat& input_frame, std::vector<Mat>& input_pyramid, const int levels);
	/*!
	* @brief ������ ��� ��������� ������ � ������� "����������������" ������ ("������ � ����������� ���������� ���������������" - infinite impulse response)
	* 
	* ��� ������ cutoff (��������� ��������), ��� ������� �������� "��������������" ������� ����������� �� "��������������" ��������
	* => ���������� (�������������� �������) �������� ������� ��������
	*/
	void IirFilter(Mat& input_pyramid_level, Mat& motion_pyramid_level, Mat& low_pass_high_level, Mat& low_pass_low_level, double cutoff_low, double cutoff_high);
	/*!
	* @brief ��������� �������� ����������� �� �������� �������
	*/
	void BuildFromLaplacePyramid(std::vector<Mat>& motion_pyramid, Mat& motion_frame, int levels);

protected:
	// lowpass 
	std::vector<Mat> low_pass_high_pyramid_;
	std::vector<Mat> low_pass_low_pyramid_;
	// 
	double cutoff_low_;
	double cutoff_high_;

	/*!
	* @brief ���������� ������� (�����) ��������
	*/
	int levels_amount_;
	/*!
	* @brief ����������� ����������. ��� ���� - ��� ������ ����, ��� ������� ������������� ��������� ��������.
	*/
	double exaggeraiton_coefficient_;
	/*!
	* @brief ������������� ����� �����
	*/
	double lambda_;
	double lambda_cutoff_;
	/*!
	* @brief ����������� ����������� �����
	*/
	double color_attenuation_coefficient_;
	/*!
	* @brief �����������, �������������� ��� ����������
	*/
	double delta_;

	double alpha_;
};

#endif