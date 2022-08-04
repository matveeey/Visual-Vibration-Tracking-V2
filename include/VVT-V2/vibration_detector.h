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
#include "VVT-V2/point_handler.h"
#include "VVT-V2/contour_handler.h"
#include "VVT-V2\frame_handler.h"
#include "VVT-V2\fft_performer.h"
#include "VVT-V2\data_displayer.h"
#include "VVT-V2/vibration_displayer.h"

class VibrationDetector
{
public:
	VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name);
	~VibrationDetector();
	void ExecuteVibrationDetection();

private:
	// callback functions for detecting the click
	static void SelectPoint(int event, int x, int y, int flags, void* userdata);
	void ClickDetect(int event, int x, int y, int flags);
	
	// callback functions for detecting the click
	static void SelectRoi(int event, int x, int y, int flags, void* userdata);
	void DragDetect(int event, int x, int y, int flags);

	// for detecting intersection between cursor and point offset
	bool IntersectionCheck(int point_num);

	// Lucas-Kanade tracking
	void LucasKanadeTracking(Mat prev_img_gray, Mat next_img_gray, std::vector<Point2f>& prev_pts, std::vector<Point2f>& next_pts, std::vector<uchar>& status);

	void LucasKanadeDoubleSideTracking(Mat prev_img_gray, Mat next_img_gray, std::vector<Point2f>& prev_pts, std::vector<Point2f>& next_pts, std::vector<uchar> status);

	// draws a track of movements
	void DrawPoints(std::vector<Point2f> prev_pts, std::vector<Point2f> next_pts, Mat& frame, bool rectangle_needed);

private:
	bool running_;
	int res_mp_;

	// Points handling
	PointHandler* point_handler_;
	std::vector<PointHandler> vec_of_point_handlers_;
	int point_offset_;
	Point last_mouse_position_;
	Point point_to_be_deleted_;
	bool intersection_;
	std::vector<bool> vec_of_intersections_;
	int point_id_;
	bool interaction_;
	std::vector<bool> vec_of_interacts_;
	std::vector<Point> text_coords_;

	std::string input_file_name_;
	std::string output_file_name_;
	std::string window_name_;

	String main_window_name_;
	String v_monitor_window_name_;

	Mat current_tracking_frame_;
	
	// for detecting the click
	bool point_selected_;
	Point2f click_coords_;

	// for rectangle
	ContourHandler* contour_handler_;
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
	int sampling_frequency_;
	int frequency_update_rate_;
	FftPerformer* fft_performer_;
	std::vector<FftPerformer> vec_of_fft_performers_;

	// for coloring data
	std::vector<float> freqs_to_be_colored_;

	// for displaying data
	DataDisplayer* data_displayer_;
	std::vector<DataDisplayer> vec_of_data_displayer_;
	std::vector<float> vec_of_frequencies_; // for a certain point
};

#endif