#ifndef FRAME_HANDLER_H
#define FRAME_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/helper.h"

using namespace cv;

/*!
@brief ����� ��� ������ � ���������� ������ �����, � ����� ��������� �� ���������
*
* �������� �����
@note ����������
*/
class FrameHandler
{
public:
	/*!
	* ����������� ������ ��������� � �������� ���������� ���������:
	* @param input_file_name - ��� (����) �������� ����� ��� ���������
	* @param output_file_name - ��� (����) ���������� ����� ����� ���������
	* @param main_window_name - �������� �������� ����
	*/
	FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string main_window_name);
	~FrameHandler();
	// ��������� ��������� ���� �����
	void ReadNextFrame();
	// ������� � ������� ���� ����������� �����������
	// fullscreen - ���� �������������� ������
	void ShowFrame(Mat frame, bool fullscreen);
	// ���������� ���� � ������� output_Cap
	void WriteFrame(Mat frame);
	// ���������� ��� ����� �������������
	Mat ConcatenateFramesHorizontally(Mat left_frame, Mat right_frame);
	// ���������� ��� ����� �����������
	Mat ConcatenateFramesVertically(Mat top_frame, Mat bottom_frame);

// �������
public:
	// ���������� ����� ���� � ������
	Mat GetGrayFrame(Mat frame_to_be_grayed);
	// ���������� ������� ������ input_cap
	bool GetInputCapStatus();
	// ���������� ��������� ����������� �� ������ ������ ����
	Mat GetCurrentFrame();
	// ���������� �������� ����, � ������� ��������� �����
	String GetWindowName();
	// ���������� ������� ����� ����� (������� � ��)
	double GetCurrentTimeOfFrame();
	// ���������� ������� ��������� ����� ������������ �������
	int GetCurrentPosOfFrame();
	// ���������� ����� ���������� ������
	int GetAmountOfFrames();
	// ���������� FPS ������������ �����
	int GetInputFps();
	// ���������� ������ ������������ �����
	int GetFrameWidth();
	// ���������� ������ ������������ �����
	int GetFrameHeight();
	// ���������� ����������� ���������������
	float GetTextResizeFactor();
	// ��������� ��������� ��� ������������ �� ����
	Mat AddTips(Mat frame, int mode);
	// ���������� ����� � ���������� � ���������. ������ ����� - ������ ��������� �����. ������ ������������ ������
	Mat GenerateGradScale(int left_limit, int right_limit, int colored_point_mode);

private:
	// �������������� ����������� ��������������� ������ � ����������� �� ���������� �����
	void InitTextResizeFactors();

protected:
	// ���������� ����������� ����
	Mat current_frame_;
	// ��������� �� ������-��������� ������� ������ �����
	VideoCapture* input_cap_;
	// ����, ����������� true ��� ������� ������ ����� � false, ���� ��������� ���� �� ��� ������
	bool input_cap_status_;
	// ��������� �� ������, ����������� ����� � ������������������ (�����) � ����������� �� ���������� ������ (cv::fourcc)
	VideoWriter* output_cap_;
	// ��� �������� ����
	std::string main_window_name_;

	// ���� �������������� ������
	bool fullscreen_;

	// ��������� �����
	// ������ ����� ��������� �����
	double input_frame_width_;
	// ������ ����� ��������� �����
	double input_frame_height_;
	// ����������� ������ ��������� �����
	double input_frame_size_ratio_;
	// ��� ��������� �����
	double input_fps_;
	// ������� ����� ����� �� ������������������
	double current_time_of_frame_;
	// ������� ����� ����� �� ������������������
	int current_pos_of_frame_;
	// ����������� ��������������� ������ (������� �� ���������� ��������� �����)
	float text_resize_factor_;
	// ������ ��������� ��� ������������
	std::vector<std::string> tip_text_;

};

#endif