#ifndef VIBRATION_DETECTOR_H
#define VIBRATION_DETECTOR_H

// VREMENNO
#define MAIN_WINDOW_NAME "Video"
#define V_MONITOR_WINDOW_NAME "Vibration Monitor"

// standart headers
#include <iostream>
#include <vector>

// OpenCV headers
#include <opencv2/video/tracking.hpp>

// my headers
#include "VVT-V2/lonely_point_handler.h"
#include "VVT-V2/colored_point_handler.h"
#include "VVT-V2/frame_handler.h"
#include "VVT-V2/helper.h"
#include "VVT-V2/contour_handler.h"

/*!
* @brief ����� ��� �������� ��������-���������� ��������
*
* ������ ������ �������� �������� �������� � ���������. ������ ���� ������� ������� ��������� �������, ��������� � ������������ ������ �������. ��.
*/
class VibrationDetector 
{
	enum main_mode
	{
		DEFAULT,
		SELECTINGROI,
		PAUSE
	};

	enum colored_point_mode
	{
		COLORING_DEFAULT,
		COLORING_BASED_ON_FREQUENCY,
		COLORING_BASED_ON_AMPLITUDE
	};

public:
	VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name);
	~VibrationDetector();
	// ��������� �������� ��������
	void ExecuteVibrationDetection();

private:
	// ����������� ������� �� �������� � ��������. ���������� ��� ����, ����� �� �������� ��������� ����� �� ����� ���������� ��������� �����
	void ServeTheQueues();
	// ������� "�������" �����
	void CreateNewPoint(Point2f mouse_coordinates);
	// ���������� ����� ������
	void LeftClickHandler(Point2f mouse_coordinates);
	// ���������� "�������" �����
	void CreateNewColoredPoint(Point2f mouse_coordinates);
	// ������� ������ "�������" �����
	void DeletePoints(Point2i mouse_coordinates);
	// ������� ������� "�������" �����
	void DeleteColoredPoints();
	// ������� "�������" � "�������" �����, ���� ������� ������� � ��� ������� ������
	void FindAndDeleteUncofidentPoints();
	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// ������� �����
	void DefaultModeHandler(int event, int x, int y);
	// ����� ��������� ������� ��������
	void RoiSelectionModeHandler(int event, int x, int y);
	// ������� ������������� ���������
	Mat MakeWarpedFrame(Mat frame, std::vector<Point2i> warping_figure);
	// ������� ����� ���������� ������-������ � �������� ���
	void TrackAndCalc();
	// ������� ���������� ��� �������� ����� �� �����������
	std::vector<Point2f> FindGoodFeatures(Mat frame, Rect roi);
	// ������ ����� � ������� ������
	void DrawDebugLkWinRectangle(Mat& frame);
	// ������������, ������ � ������� �� ����� ������������ ���� � �������
	void DrawAndOutput(Mat& frame);

private:
	/*!
	* @brief ��������������
	*/
	FrameHandler* frame_handler;
	/*!
	* @brief ���� ����������� ��������� ���������
	* @see ExecuteVibrationDetection()
	*/
	bool running_;
	/*!
	* @brief ����������� ���������� �������� ���� � ����������� �� ���������� ��������� �����
	*/
	float res_mp_;
	/*!
	* @brief ������-id (����������� ����� �����)
	*/
	int point_id_;
	/*!
	* @brief ��������� ������� ������� � ������� ����
	*/
	Point last_mouse_position_;
	/*!
	* @brief �������� ����� ������ (enum main_mode)
	*/
	int current_mode_;
	/*!
	* @brief ���� ��� �������������� ������
	*/
	bool fullscreen_;

	///
	// NORMAL MODE
	///
	/*!
	* @brief ������� �� �������� "�������" �����
	*/
	std::vector<Point2i> l_click_queue_;
	/*!
	* @brief ������� �� �������� "�������" �����
	*/
	std::vector<Point2f> c_point_queue_;
	/*!
	* @brief ������� �� �������� �����
	*/
	std::vector<Point2i> delete_queue_;
	/*!
	* @brief ������� �� ��������� ���������
	*/
	std::vector<Point2i> histograms_queue_;
	/*!
	* @brief ������ ������������ "��������" �����
	*/
	std::vector<LonelyPointHandler*> vec_lonely_point_handlers_;
	/*!
	* @brief ������ ������������ "�������" �����
	*/
	std::vector<ColoredPointHandler*> vec_colored_point_handlers_;

	///
	// ADJUST SENSIVITY MODE
	///
	/*!
	* @brief ���������������� ��� �������� �������� 
	* 
	* ���� ��� ��������� ���������������� ���������� ������� "A" ��� ���������� �����
	* 
	* ��� ���� �������� - ��� ������ ����� ����� �������, �� � ��� ������ �� ��������� �������� ����� ����� �����������
	*/
	int sensivity_in_percents_;

	/*!
	* @brief ������� Mat-instance ������ ��� ���������� ����� �� �����
	*/
	Mat current_tracking_frame_;
	/*!
	* @brief Mat-instance, ������� ����� ������� � ����
	* @see DrawAndOutput()
	*/
	Mat frame_to_be_shown_;
	/*!
	* @brief ����������� ������� ����� ����� ������
	* @see FrameHandler::GenerateGradScale() � DrawAndOutput()
	*/
	Mat grad_scale_;
	/*!
	* @brief ������� ����� ��������� ����� ��� ��������� ������� �������� (�� ���������/�� �������)
	*/
	int colored_point_mode_;

	///
	// ROI SELECTING MODE
	///
	// 
	/*!
	* @brief ����, ������������ ���������� �� � ������ ������ ��������� ROI
	*/
	bool roi_selecting_;
	/*!
	* @brief ����, ������������ ��� �� ������� ROI ��� ���
	*/
	bool roi_selected_;
	/*!
	* @brief ���������� ������ ����� ������������� (top left, bottom left, top right, bottom right - ������ "� ����� �������" �������� ROI � ������� ���� (�������))
	*/
	Point2i tl_click_coords_;
	
	/*!
	* @brief ��� (����) �������� �����
	*/
	std::string input_file_name_;
	/*!
	* @brief ��� (����) �����, � ������� ����� ��������� ����������
	*/
	std::string output_file_name_;
	/*!
	* @brief ��� (����) ����� ��� �������� ����������
	* @see OutputToCsv
	*/
	std::string output_csv_file_name_;
	/*!
	* @brief ��� �������� ����
	*/
	std::string window_name_;

	/*!
	* @brief ����, ������������, ���������� �� � ������ ������ ��������� ������ (��� ����������� ��������� ��������)
	*/
	bool warping_figure_selecting_;
	/*!
	* @brief ���������� ������ ��� ����������� �����������
	*/
	std::vector<Point> warping_figure_;

	/*!
	* @brief ���������� ���� ����� � ��������� � � ��������� ������ (FrameHandler::MakeGrayFrame())
	*/
	Mat prev_img_gray_;
	/*!
	* @brief ��������� (��� ������� ����� "�����������" - ������ ��� ������� ��� ���������) ���� ����� � ��������� � � ��������� ������ (FrameHandler::MakeGrayFrame())
	*/
	Mat next_img_gray_;
	/*!
	* @brief ������ ���� ��� ��������� ������-������
	*/
	int lk_win_size_;
	/*!
	* @brief ���������� ����� �������� ����������� � ��������� �-�
	*/
	int level_amount_;


	// ��� �������� �������������� �����
	/*!
	* @brief ��� �������� ����� (��� ������ ������ ������ ��������, ������� ����� ���������)
	*/
	int fps_;
	/*!
	* @brief ������� ����� ����� � �� (�������)
	*/
	double frame_time_;
};

#endif