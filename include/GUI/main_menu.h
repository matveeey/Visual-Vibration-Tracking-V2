#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#define MAIN_MENU_WINDOW_NAME "MainMenu"

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// my headers
#include "VVT-V2\vibration_detector.h"
#include "CamCalib\video_undistorter.h"
#include "CamCalib\camera_calibrator.h"
#include "MovEn/movement_enlargement.h"

using namespace cv;

/*!
* @brief ����� ��� �������� ���������� ���� ������ ������ ������ ���������
*/
class MainMenu
{
	/*!
	* @brief ������������ �� ������� �������� ������� ������
	*/
	enum MODE
	{
		CALIBRATION,
		UNDISTORTION,
		VIBRATION_MONITORING,
		MOVEMENT_ENLARGEMENT
	};

public:
	/*!
	* @brief ����������� ����� ������
	* @param txt_file_name: ��� (����) ���������� ���������, ���������� ������ ��� ������ ���� � ������� � �������� �������������
	*/
	MainMenu(std::string txt_file_name);

private:
	/*!
	* @brief ��������� ������ ����
	*/
	void LoadPaths();
	/*!
	* @brief ��������� ���������� ������ �� ��������� ��������� �����
	* @param chessboards_path: ��� (����) � ����� � ������������� ��������� �����
	*/
	void ExecuteCameraCalibration(std::string chessboards_path);
	/*!
	* @brief ��������� �������� ���������
	* @param input_file_name: ��� (����) �������� �����
	* @param output_file_name: ��� (����) ��������� �����
	*/
	void ExecuteVideoUndistortion(std::string input_file_name, std::string output_file_name);
	/*!
	* @brief ��������� �������� �������� (�������� �����)
	* @param input_file_name: ��� (����) �������� �����
	* @param output_file_name: ��� (����) ��������� �����
	*/
	void ExecuteVibrationMonitoring(std::string input_file_name, std::string output_file_name);
	/*!
	* @brief ��������� ���������� ��������
	* @param input_file_name: ��� (����) �������� �����
	* @param output_file_name: ��� (����) ��������� �����
	*/
	void ExecuteMotionMagnification(std::string input_file_name, std::string output_file_name);
	/*!
	* @brief ������ ����� ���� � �������� � ��������� �� Mat-������� menu_
	*/
	void CreateMenu();
	/*!
	* @brief ������������ ������ � ������� �� ��� �����
	*/
	void UpdateBtnColorAndText();
	/*!
	* @brief callback ������� ��� ����������� ������� (������������ ��� ��������� ����� �����)
	*/
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	/*!
	* @brief callback ������� ��� ����������� ������� (������������ ��� ��������� ����� �����)
	*/
	void DetectEvent(int event, int x, int y, int flags);
	/*!
	* ��������� ����
	*/
	void Idle();

private:
	/*!
	* @brief ���� ����� � �����������
	*/
	std::string params_file_name_;
	/*!
	* @brief �������� ����
	*/
	std::string winname_;
	/*!
	* @brief ����������� ����, �� ������� ����� ����������� ���������
	*/
	Mat menu_;
	/*!
	* @brief ���������, �������� � ���� ��������������-������ (���������� ��� �������� ���������� ������)
	*/
	std::vector<Rect> buttons_;
	/*!
	* @brief ������ ������
	*/
	std::string button_text_[4];
	/*!
	* @brief ������ ����
	*/
	int menu_width_;
	/*!
	* @brief ������ ����
	*/
	int menu_height_;
	/*!
	* @brief ���������� ������
	*/
	int button_amount_;
	/*!
	* @brief ������ ������
	*/
	int button_width_;
	/*!
	* @brief ������ ������
	*/
	int button_height_;
	/*!
	* @brief ������ ����� ��������
	*/
	int button_spacing_;
	/*!
	* @brief ������ �� ����������� ������ ������
	*/
	int top_offset_;
	/*!
	* @brief ������ �� ����������� ������ �����
	*/
	int bottom_offset_;
	/*!
	* @brief ������ � ���� ������ �� ������ �� ������
	*/
	int width_offset_;
	/*!
	* @brief �����
	*/
	int font;
	/*!
	* @brief �������� ������
	*/
	double font_scale;
	/*!
	* @brief ������� ������
	*/
	int thickness;
	/*!
	* @brief �������� ��� cv::getText()
	*/
	int baseline;

	/*!
	* @brief ��������� ��������� � callback ���������� �������
	*/
	Point2i last_mouse_position_;
	/*!
	* @brief ��������� ��� �������� ����� ������ ������
	*/
	std::vector<Scalar> buttons_color_;
	/*!
	* @brief ���� ���������� ������
	*/
	bool mode_chosen_;
	/*!
	* @brief ���� ������ �� ���������
	*/
	bool wanna_quit_;

private:
	/*!
	* @brief ��� (����) �������� ����� (�����) ��� ����������
	*/
	std::string input_calibration_video_;
	/*!
	* @brief ��� (����) �������� ����� (�����) ��� �������� ���������
	*/
	std::string input_distorted_video_;
	/*!
	* @brief ��� (����) ��������� ����� (�����) ����� �������� ���������
	*/
	std::string output_undistorted_video_;
	/*!
	* @brief ��� (����) �������� ����� (�����) ��� �������� ��������
	*/
	std::string input_vibration_detection_;
	/*!
	* @brief ��� (����) ��������� ����� (�����) ����� �������� ��������
	*/
	std::string output_vibration_detection_;
	/*!
	* @brief ���� � ����� � ������������ ��������� �����
	*/
	std::string chessboards_path_;
	/*!
	* @brief ��� (����) �������� ����� (�����) ��� �������� ��������
	*/
	std::string input_movement_enlargment_;
	/*!
	* @brief ��� (����) ��������� ����� (�����) ��� �������� ��������
	*/
	std::string output_movement_enlargement_;
	
};

#endif