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
#include "VVT-V2/data_displayer.h"
#include "VVT-V2/point_handler.h"
#include "VVT-V2/frame_handler.h"

class VibrationDetector 
{
public:
	VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name);
	~VibrationDetector();
	// Выполняет детекцию вибрации
	void ExecuteVibrationDetection();

private:
	// Обслуживает очереди на удаление и создание. Необходимо для того, чтобы не нарушить нумерацию точек во время выполнения основного цикла
	void ServeTheQueues();
	// Создаем объект точку
	void CreateNewPoint(Point2f mouse_coordinates);
	// Удаляем объект точку
	void DeletePoints(Point2i mouse_coordinates);
	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// Удаляет перспективные искажения
	Mat MakeWarpedFrame(Mat frame, std::vector<Point2i> warping_figure);
	// Трекает точки алгоритмом Лукаса-Канаде и вызывает БПФ
	void TrackAndCalc();
	// Рисует точки и выводит данные
	void DrawData(Mat& frame);

private:
	bool running_;
	int res_mp_;
	int point_id_;

	// NORMAL MODE
	// 
	// Очередь на создание точек
	std::vector<Point2i> create_queue_;
	// Очередь на удаление точек
	std::vector<Point2i> delete_queue_;
	// Предыдущие позиции точек
	std::vector<Point2f> previous_points_coordinates_;
	// Points handling
	std::vector<PointHandler> vec_point_handlers_;

	// R MODE
	// 
	// Флаг для выделения ROI
	bool roi_selecting_;
	// ROI хэндлер
	

	int point_offset_;
	Point last_mouse_position_;
	Point point_to_be_deleted_;
	bool intersection_;
	std::vector<bool> vec_of_intersections_;
	/*int point_id_;*/
	bool interaction_;
	std::vector<bool> vec_of_interacts_;
	std::vector<Point> text_coords_;

	std::string input_file_name_;
	std::string output_file_name_;
	std::string window_name_;

	String main_window_name_;
	String v_monitor_window_name_;

	Mat current_tracking_frame_;
	Mat copy_of_current_tracking_frame_;
	
	// for detecting the click
	bool point_selected_;
	Point2i click_coords_;

	// for rectangle
	Mat unchanged_frame_;
	Point2i tl_click_coords_;
	Point2i br_click_coords_;
	Point2i mouse_move_coords_;
	bool right_button_down_;
	bool rectangle_selected_;
	bool colors_inited_;
	Rect roi_;
	std::vector<Point2f> prev_vibrating_pts_;
	std::vector<Point2f> next_vibrating_pts_;

	// for warping rectangle
	bool warping_figure_selecting_;
	std::vector<Point> warping_figure_;

	// for Lucas-Kanade tracking
	Mat prev_img_gray_;
	Mat next_img_gray_;
	std::vector<Point2f> prev_pts_;
	std::vector<Point2f> next_pts_;
	std::vector<Point2f> contour_prev_pts_;
	std::vector<Point2f> contour_next_pts_;
	std::vector<uchar> status_; // 1 if features found, 0 if not
	std::vector<uchar> rect_status_;
	int lk_win_size_;

	// useful parameters
	int number_of_points_;
	int number_of_vibrating_pts_;
	bool vibration_inited_;

	// Fast Fourier Transform
	int fps_;
	int update_rate_;
};

#endif