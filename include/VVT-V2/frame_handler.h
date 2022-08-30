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
* @brief ����� ��� ������ � ���������� ������ �����, � ����� ��������� �� ���������
* 
* ������ ������ ����������� ������������������ ������ �� ����� (����) �������� �����, ����� ����, ��������� � ������� ������ ����� ��������� ����� � ������� ������ ReadNextFrame
* 
* ��� ������ ����� ���������� ������������ ����� WriteFrame, ������� � �������� ��������� ��������� cv::Mat ������ (�����������) � ���������� ��� � ����� �������� ������������������ (�����)
* @note ������������� ��������� �� ���� ��������� ��� ������������ (����� AddTips) � �������� (GenerateGradScale)
* @note ������������� ����� ��������������� (���������� � ���� ����������� ���) cv::Mat-�������� �� ����������� ��� �� ���������
*/
class FrameHandler
{
public:
	/*!
	* @brief ����������� ����� ������
	* @param input_file_name: ��� (����) �������� ����� ��� ���������
	* @param output_file_name: ��� (����) ���������� ����� ����� ���������
	* @param main_window_name: �������� �������� ����
	*/
	FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string main_window_name);
	~FrameHandler();
	/*!
	* @brief ��������� ��������� ���� �����
	*/
	void ReadNextFrame();
	/*!
	* @brief ������� � ������� ���� ����������� �����������
	* @param frame: �����������, ������� ����� ���������� � ����
	* @param fullscreen: ���� �������������� ������
	*/
	void ShowFrame(Mat frame, bool fullscreen);
	/*!
	* @brief ���������� ���� � ������� output_cap
	* @param frame: �����������, ������� ����� �������� � ������������������ (���������� ��������� � � �����)
	*/
	void WriteFrame(Mat frame);
	/*!
	* @brief ���������� ��� ����� �������������
	* @param left_frame: ����, ������� ����� �����
	* @param right_frame: ����, ������� ����� ������
	* @return dst: ����������� ����
	*/
	Mat ConcatenateFramesHorizontally(Mat left_frame, Mat right_frame);
	/*!
	* @brief ���������� ��� ����� �����������
	* @param top_frame: ����, ������� ����� ������
	* @param bottom_frame: ����, ������� ����� �����
	* @return dst: ����������� ����
	*/
	Mat ConcatenateFramesVertically(Mat top_frame, Mat bottom_frame);

// �������
public:
	/*!
	* @brief ���������� ����� ���� � ������
	* @param frame_to_be_grayed: �����������, ������� ����� ��������������� � grayscale � �� ������� ����� ������� ��������� ���� (��� �������� �������� �����)
	* @return ������������ ���� (cv::Mat)
	*/
	Mat MakeGrayFrame(Mat frame_to_be_grayed);
	/*!
	* @brief ���������� ������� ������ input_cap
	* @return true: ���� ��� ������, false: ���� �� ��� ������
	*/
	bool GetInputCapStatus();
	/*!
	* @brief ���������� ��������� ����������� �� ������ ������ ����
	* @return cv::Mat instance
	*/
	Mat GetCurrentFrame();
	/*!
	* @brief ���������� �������� �������� ����, � ������� ��������� �����
	* @return �������� ����
	*/
	String GetMainWindowName();
	/*!
	* @brief ���������� ������� ����� ����� (������� � ��)
	* @return ������� ����� ����� � �������������
	*/
	double GetCurrentTimeOfFrame();
	/*!
	* @brief ���������� ������� ��������� ����� ������������ �������
	* @return ����� ����� � ������������������
	*/
	int GetCurrentPosOfFrame();
	/*!
	* @brief ���������� ����� ���������� ������
	* @return ���������� ������ � ������������������
	*/
	int GetAmountOfFrames();
	/*!
	* @brief ���������� FPS ������������ �����
	* @return FPS
	*/
	int GetInputFps();
	/*!
	* @brief ���������� ������ ��������� �����
	* @return ������ ��������� �����
	*/
	int GetFrameWidth();
	/*!
	* @brief ���������� ������ ������������ �����
	* @return ������ ��������� �����
	*/
	int GetFrameHeight();
	/*!
	* @brief ���������� ����������� ���������������
	* @return ����������� ���������������
	*/
	float GetTextResizeFactor();
	/*!
	* @brief ��������� (������������) ��������� ��� ������������ �� ����
	* @param frame: �����������, �� ������� ����� �������� ��������� ��� ������������
	* @param mode: ������� ����� ������ ����������
	* @return cv::Mat ���� � �����������
	* @note ������ ������ ����������: default, ROI selecting, pause
	*/
	Mat AddTips(Mat frame, int mode);
	/*!
	* @brief ���������� ����� � ���������� � ���������. ������ ����� - ������ ��������� �����. ������ ������������ ������
	* @param left_limit: ������� �������� ������ �����
	* @param right_limit: ������� �������� ������ ������
	* @param colored_point_mode: ����� ��������� ������� �����
	* @return cv::Mat �������� � ������������ ��������� ������� ������ ������ ��������� ����� � ������������� ������ �������
	* @note ������ ��������� ������� �����: ����������� (default), ��������� �� ������� ��������, ��������� �� ���������
	*/
	Mat GenerateGradScale(int left_limit, int right_limit, int colored_point_mode);

private:
	/*!
	* @brief �������������� ����������� ��������������� ������ � ����������� �� ���������� �����
	*/
	void InitTextResizeFactors();

private:
	/*!
	@brief ���������� ����������� ����
	*/
	Mat current_frame_;
	/*!
	@brief ��������� �� ������-��������� ������� ������ �����
	*/
	VideoCapture* input_cap_;
	/*!
	@brief ����, ����������� true ��� ������� ������ ����� � false, ���� ��������� ���� �� ��� ������
	*/
	bool input_cap_status_;
	/*!
	@brief ��������� �� ������, ����������� ����� � ������������������ (�����) � ����������� �� ���������� ������ (cv::fourcc)
	*/
	VideoWriter* output_cap_;
	/*!
	@brief ��� �������� ����
	*/
	std::string main_window_name_;

	/*!
	@brief ���� �������������� ������
	*/
	bool fullscreen_;

	// ��������� �����
	/*!
	@brief ������ ����� ��������� �����
	*/
	double input_frame_width_;
	/*!
	@brief ������ ����� ��������� �����
	*/
	double input_frame_height_;
	/*!
	@brief ����������� ������ ��������� �����
	*/
	double input_frame_size_ratio_;
	/*!
	@brief FPS ��������� �����
	*/
	double input_fps_;
	/*!
	@brief ������� ����� ����� �� ������������������
	*/
	double current_time_of_frame_;
	/*!
	@brief ������� ����� ����� �� ������������������
	*/
	int current_pos_of_frame_;
	/*!
	@brief ����������� ��������������� ������ (������� �� ���������� ��������� �����)
	*/
	float text_resize_factor_;
	/*!
	@brief ������ ��������� ��� ������������
	*/
	std::vector<std::string> tip_text_;

};

#endif