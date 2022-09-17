#ifndef VIDEO_UNDISTORTER_H
#define VIDEO_UNDISTORTER_H

// OpenCV headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/videoio/videoio.hpp>

#include <iostream>
#include <fstream>
#include <iomanip>

// my headers
#include "VVT-V2/helper.h"

using namespace cv;

/*!
* @brief ����� ��� �������� ��������� �� �����
*
* ������ ������ ������������ ������ � ���������� ������ � ��������� ���������� � �������� ������������������ (�����)
* 
* @todo ������ � ������� (������, ���������) ������� ��������� � ������� FrameHandler. �� ������ ������ ��� ���������� �� ���. ������ ����� ������ ���� ���������� ������ �����.\n
* cv::VideoCapture �������� ��������� ����� ������������������ �����������, �� ����������� �����, ����� � ����� ����������� �� ������-�� ���� (�� �����, ��������).\n
* ��� �� ����� �������
*/
class VideoUndistorter
{
public:
	/*!
	* @brief ����������� ����� ������
	* @param input_file_name: ��� (����) �������� ����� ��� ���������
	* @param output_file_name: ��� (����) ���������� ����� ����� ���������
	* @param txt_file_name: ��� (����) ����� � "�������������� ���������" �����
	* @note CameraCalibrator
	*/
	VideoUndistorter(std::string input_file_name_, std::string output_file_name, std::string txt_file_name);
	~VideoUndistorter();
	/*!
	* @brief �������� �����. ��������� �������� ��������� �� �����
	* @return 0 - � ������ ������ �� ��������� (������ ���� �������). 1 - � ���������
	* @todo ����� ���������� ����� ������)
	*/
	int ExecuteVideoUndistortion();

private:
	/*!
	* @brief �� �������� � FrameHandler::AddTips ��������� ��������� ������������. � ������ ������ ��� - ����� �������� ��������������� ����� � ����� ���-�� ������.\n
	* @param frame: ����, �� ������� ����� ��������� ���������
	* @param tip: ������ � ����������
	* @todo ������� � � �����-�� ��������� ������, ����� �� ���� ������������ ����
	*/
	void AddTips(Mat& frame, std::string tip);
	/*!
	* @brief ��������� ���� � �������� ������������������ (�����)
	* @param frame: ����, ������� ����� �������
	* @todo ����������� �� ��������� ������� �������� ������ (VideoUndistorter)
	*/
	void SaveFrame(Mat frame);
	/*!
	* @brief ��������� ��������� (������������ fx, fy, cx, cy), ����������� ��� �������� ���������. ���� ������� � CameraCalibrator
	* @param txt_file_name: ���� � ����� � ��������������
	*/
	void LoadFoundParamsFromFile(std::string txt_file_name);
	/*!
	* @brief ��������� ������ �� ��������� ���������� �������� �� ������������ ������� - �����������
	* @param input_string: �������� ������
	* @param delimiter: �����������, �� �������� �������� ������ ����� ���������
	* @return ������, ��������� �� ����������� ������ �������� ������ (������ �� ��������)
	*/
	std::vector<std::string> SplitString(std::string line, std::string delimiter);

private:
	/*!
	* @brief �������� �������� ����
	*/
	std::string winname_;
	/*!
	* @brief "�����" �� ������ �������� ����� (������������������)
	*/
	VideoCapture* input_cap_;
	/*!
	* @brief "�����" �� ������ ��������� ����� (������������������)
	*/
	VideoWriter* output_cap_;
	/*!
	* @brief ������ ����� (�������� � ���������)
	*/
	double frame_width_;
	/*!
	* @brief ������ ����� (�������� � ���������)
	*/
	double frame_height_;
	/*!
	* @brief ���������� ������ �������� �����
	*/
	double frame_count_;
	/*!
	* @brief ��� �������� �����
	*/
	double fps_;
	/*!
	* @brief ���� � �������� �����
	*/
	std::string input_file_name_;
	/*!
	* @brief ���� � ��������� �����
	*/
	std::string output_file_name_;
	/*!
	* @brief �� ����� �������, �������� ���������� ���� - ����� + ������.
	* @see CameraCalibrator
	*/
	Mat camera_matrix_;
	/*! @brief ������������ ���������
	* @note CameraCalibrator
	*/
	Mat distortion_coefficients_;
	/*!
	* @brief ������� ���� � ����������
	*/
	Mat current_frame_;
	/*!
	* @brief ������� ���� � �������� ����������
	*/
	Mat undistorted_frame_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double fx_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double fy_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double px_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double py_;
	/*!
	* @brief ���� � ����� � ��������������
	* @see CameraCalibrator
	*/
	std::string txt_file_name_;
};

#endif