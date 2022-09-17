#ifndef CAMERA_CALIBRATOR_H
#define CAMERA_CALIBRATOR_H

// OpenCV headers
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <thread>
#include <chrono>

// my headers
#include "CamCalib\video_undistorter.h"
#include "VVT-V2/helper.h"

constexpr auto EXTENTION = ".JPG";

using namespace cv;

/*!
* @brief ����� ��� ���������� ������������� ��� ���������� ����� (� ������� ������� ���� ����� �����)
*
* ������ ������ ������������ ������ ������/���������� � ��������� ������ � ��������� ������ �� ����� � ��������� ������������ ������� [] �����
* TODO: �������� ��� ���� ������� (3 � 3)
*/
class CameraCalibrator
{
public:
	/*!
	* @brief ����������� ����� ������
	* @param chessboards_path: ��� (����) � ����� � ������������� ��������� �����
	* @note VideoUndistorter
	*/
	CameraCalibrator(std::string chessboards_path);
	~CameraCalibrator();

	int ExecuteCameraCalibration();

private:
	/*!
	* @brief ��������� ���������� (�����������) ��������� ����� � ������ �� ���� � �����.
	* @note �� ��������� �������� ����������� � ���� ������ ���� �� ����: "<����� �����������>.png"
	*/
	int LoadImages(std::string chessboards_path);
	/*!
	* @brief ��������� ����� ����������� � ����������� ���������� chessboard_amount_ �� ����
	* @param path: ���� � ����� � �������������
	* @return cv::Mat ����������� �����������
	*/
	Mat ReadNextImage(std::string path);
	/*!
	* @brief �� �������� � FrameHandler::AddTips ��������� ��������� ������������. � ������ ������ ��� - ����� �������� ��������������� ����� � ����� ���-�� ������.\n
	* @param frame: ����, �� ������� ����� ��������� ���������
	* @param tip: ������ � ����������
	* @todo ������� � � �����-�� ��������� ������, ����� �� ���� ������������ ����
	*/
	void AddTips(Mat& frame, std::string tip);
	/*!
	* @brief ������� ��������� ������� ��������, �������� � ������������ ������ (CameraCalibrator::CameraCalibrator)
	* @param input_frame: ����, �� ������� ����� ���������� ����� ��������
	* @param pattern_size: ������ �������� (�� ������� � ������ 6�9)
	* @param corners: ����� �� ������, � ������� ����� �������� ��������� ���������� ����� ��������
	* @return true - � ������, ���� ������� �������, false - � ��������� ������
	*/
	bool FindCorners(Mat input_frame, Size pattern_size, std::vector<Point2f>& corners);
	/*!
	* @brief ��������� ��������� ��������� (������������ fx, fy, cx, cy), ����������� ��� �������� ���������
	* @param camera_matrix: �������, ������� ����� �������� � ����
	* @param dist_coeffs_2be_written: ������������ ���������, ������� ����� �������� � ����
	*/
	void SaveFoundParamsToFile(Mat camera_matrix, Mat dist_coeffs_2be_written);
	
private:
	/*!
	* @brief ���� � ����� � ������������ "��������� �����"
	*/
	std::string chessboards_folder_path_;
	/*!
	* @brief ���������� ���������� ��������� ����� (��������� � CameraCalibrator::LoadImages)
	*/
	int chessboards_amount_;
	/*!
	* @brief ������ ����������� ���������� � ���������� �������
	*/
	std::vector<Mat> images_with_chessboard_;
	/*!
	* @brief "�����" �� ������ �������� ����� (������������������)
	*/
	VideoCapture* input_cap_;
	/*!
	* @brief ������ �����
	*/
	double frame_width_;
	/*!
	* @brief ������ �����
	*/
	double frame_height_;
	/*!
	* @brief �������� ����
	*/
	std::string winname_;
	/*!
	* @brief ���� � ���������� �����, � ������� ����� ��������� ��������� ������������
	*/
	std::string txt_file_name_;
	/*!
	* @brief �������, � ������� ��������� ��������� ������������ px, py, cx, cy
	*/
	Mat camera_matrix_;
	/*!
	* @brief ������������ ���������
	*/
	Mat distortion_coefficients_;
	std::vector<Mat> rotation_vector_;
	std::vector<Mat> translation_vector_;
	/*!
	* @brief ��������� ���� ��������� �����, ��������� �� ���������� �����������
	*/
	std::vector<Point2f> corners_;
	/*!
	* @brief ��������� ��� �������� ��������� ����� ��������� �����
	*/
	std::vector<std::vector<Point2f>> vec_of_corners_;
	/*!
	* @brief ������ ��������, �� �������� ����� �������� ��������� �����
	*/
	Size pattern_size_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	*/
	double fx_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	*/
	double fy_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	*/
	double px_;
	/*!
	* @brief ����������� ������� VideoUndistorter::camera_matrix_
	*/
	double py_;

};

#endif