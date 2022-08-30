#ifndef MOVEMENT_ENLARGEMENT_H
#define MOVEMENT_ENLARGEMENT_H

// C++ standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/frame_handler.h"
#include "MovEn/eulerian_enlargement.h"

using namespace cv;

/*!
* @brief �������� (�� EulerEnlarger) ����� ��� ����������� ���������� �������� �� �����
*/
class MovementEnlarger : public EulerEnlarger
{
public:
	/*!
	* @brief ����������� ����� ������
	* @param input_file_name: ��� (����) �������� ����� (�����)
	* @param output_file_name: ��� (����) ��������� ����� (�����)
	*/
	MovementEnlarger(std::string input_file_name, std::string output_file_name);
	~MovementEnlarger();
	/*!
	* @brief ��������� ���������� ���������� ��������
	*/
	void ExecuteLaplaceEnlargement();

private:
	/*!
	* @brief ��� (����) �������� ����� (�����)
	*/
	std::string input_file_name_;
	/*!
	* @brief ��� (����) ��������� ����� (�����)
	*/
	std::string output_file_name_;
	/*!
	* @brief ��� ����, � ������� ����� ���������� ������������ ����
	*/
	std::string win_name_;

	/*!
	* @brief ����������� (����), �������� �� ������� ����� ���������
	*/
	Mat frame_to_be_enlarged_;
	/*!
	* @brief ����������� (����), �������� �� ������� ���� ���������
	*/
	Mat frame_enlarged_;
	/*!
	* @brief Mat-��������, ���������� ���������� �� ����������� ��������
	*/
	Mat frame_with_motion_info_;
	
};

#endif