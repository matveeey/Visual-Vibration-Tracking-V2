#ifndef LAPLACE_ENLARGEMENT_H
#define LAPLACE_ENLARGEMENT_H

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

class LaplaceEnlarger
{
protected:
	LaplaceEnlarger();
	~LaplaceEnlarger();

protected:
	// �������� ������
	//
	// ������������ ���������� �������� �� ���������� ������ ��������
	void LaplaceEnlarge(Mat& motion_pyramid_level, int current_level);
	// "����������" ����� �� �����
	void Attenuate(Mat& motion_frame);

	// ���� � ����� �� �������. �������
	// 
	// ������� �������� ����������� �������
	void BuildLaplacePyramid(Mat& input_frame, std::vector<Mat>& input_pyramid, int levels);
	// ������ ��� ��������� ������ � ������� "����������������" ������ ("������ � ����������� ���������� ���������������" - infinite impulse response)
	// ��� ������ cutoff (��������� ��������), ��� ������� �������� "��������������" ������� ����������� �� "��������������" ��������
	// => ���������� (�������������� �������) �������� ������� ��������
	void IirFilter(Mat& input_pyramid_level, Mat& motion_pyramid_level, Mat& low_pass_high_level, Mat& low_pass_low_level, double cutoff_low, double cutoff_high);
	// ��������� �������� ����������� �� �������� �������
	void BuildFromLaplacePyramid(std::vector<Mat>& motion_pyramid, Mat& motion_frame, int levels);

protected:
	// lowpass 
	std::vector<Mat> low_pass_high_pyramid_;
	std::vector<Mat> low_pass_low_pyramid_;
	// 
	double cutoff_low_;
	double cutoff_high_;

	// ���������� ������� (�����) ��������
	int levels_amount_;
	// ����������� ����������. ��� ���� - ��� ������ ����, ��� ������� ������������� ��������� ��������.
	double exaggeraiton_coefficient_;
	// ������������� ����� �����
	double lambda_;
	double lambda_cutoff_;
	// ����������� ����������� �����
	double color_attenuation_coefficient_;
	// �����������, �������������� ��� ����������
	double delta_;

	float alpha_;
};

#endif