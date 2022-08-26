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

using namespace cv;

// ����� ��������� ������� ������, ������� ����� �������� ����� � ����
class FrameHandler
{
public:
	FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string window_name);
	~FrameHandler();
	// ��������� ��������� ���� �����
	void ReadNextFrame();
	// ������� � ���� ����������� ����
	void ShowFrame(Mat frame);
	// ���������� ���� � ������� output_Cap
	void WriteFrame(Mat frame);
	// ���������� ��� ����� �������������
	Mat ConcatenateFrames(Mat left_frame, Mat right_frame);

// �������
public:
	// ��������� ��������� � ����� �� ����
	Mat AddTips(Mat frame, int mode);
	// ���������� ����� ���� � ������
	Mat GetGrayFrame(Mat frame_to_be_grayed);
	// ������������ � ���������� ���� � ����������� �� ���������� �����
	Mat ResizeFrame(Mat frame);
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
	float GetResizingCoefficient();

protected:
	Mat input_frame_;
	Mat current_frame_;

	VideoCapture* input_cap_;
	bool input_cap_status_;
	VideoWriter* output_cap_;
	std::string input_path_;
	std::string output_path_;

	// ��������� �����
	double input_frame_width_;
	double input_frame_height_;
	double input_fps_;
	// ��� �������� ����
	std::string window_name_;
	// ������� ���������� � �����
	double current_time_of_frame_;
	int current_pos_of_frame_;
	// ��� ��������������� �����������
	Size resized_resolution_;
	float resizing_coefficient_;
	// ������ ��������� ��� ������������
	std::vector<std::string> tip_text_;

private:
	void ResizeResolution();
};

#endif