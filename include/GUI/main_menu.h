#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#define MAIN_MENU_WINDOW_NAME "Main menu"

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

enum MODE
{
	CALIBRATION,
	UNDISTORTION,
	VIBRATION_MONITORING,
	MOVEMENT_ENLARGEMENT
};


class MainMenu
{
public:
	MainMenu(std::string txt_file_name);

private:
	// ��������� ������ ���� � ��
	void LoadPaths();
	// ��������� ���������� ������ �� ��������� � �����
	void ExecuteCameraCalibration(std::string input_file_name, std::string chessboards_path);
	// ��������� �������� ���������
	void ExecuteVideoUndistortion(std::string input_file_name, std::string output_file_name);
	// ��������� �������� �������� (�������� �����)
	void ExecuteVibrationMonitoring(std::string input_file_name, std::string output_file_name);
	// ��������� ���������� ��������
	void ExecuteMotionMagnification(std::string input_file_name, std::string output_file_name);
	// ������ ����� ���� � �������� � ��������� �� Mat-������� menu_
	void CreateMenu();
	// ������������ ������ � ������� �� ��� �����
	void UpdateBtnColorAndText();
	// callback function for determining the event click on mouse
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	// "helper" function for implementing callback function as a method of C++ class
	void DetectEvent(int event, int x, int y, int flags);
	// �������� ��������
	void Idle();

private:
	// ���� ����� � �����������
	std::string params_file_name_;
	// �������� ����
	std::string winname_;
	// Mat-������. �� ���� ��������
	Mat menu_;
	// "�����" ���
	std::vector<Rect> buttons_;
	// ������ ������
	std::string button_text_[4];
	// ������ � ������ ����
	int menu_width_;
	int menu_height_;
	// ���������� ������
	int button_amount_;
	// ������ � ������ ������
	int button_width_;
	int button_height_;
	// ��������� �������
	int button_spacing_;
	int top_offset_;
	int bottom_offset_;
	int width_offset_;
	// �������� ������
	int font;
	double font_scale;
	int thickness;
	int baseline;

	// ��������� ��������� � callback ���������� �������
	Point2i last_mouse_position_;
	// ��������� ��� �������� ����� ������ ������
	std::vector<Scalar> buttons_color_;
	// ���� ���������� ������
	bool mode_chosen_;
	// ���� ������ �� ���������
	bool wanna_quit_;

private:
	std::string input_calibration_video_;
	std::string input_distorted_video_;
	std::string output_undistorted_video_;
	std::string input_vibration_detection_;
	std::string output_vibration_detection_;
	std::string chessboards_path_;
	std::string input_movement_enlargment_;
	std::string output_movement_enlargement_;
	
};

#endif