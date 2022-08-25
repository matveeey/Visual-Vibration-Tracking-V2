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

class VibrationDetector
{
	enum main_mode
	{
		DEFAULT,
		SELECTINGROI,
		PAUSE
	};

public:
	VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name);
	~VibrationDetector();
	// Выполняет детекцию вибрации
	void ExecuteVibrationDetection();

private:
	// Обслуживает очереди на удаление и создание. Необходимо для того, чтобы не нарушить нумерацию точек во время выполнения основного цикла
	void ServeTheQueues();
	// Создаем "обычную" точку
	void CreateNewPoint(Point2f mouse_coordinates);
	// Обработчик левых кликов
	void LeftClickHandler(Point2f mouse_coordinates);
	// Обработчик "цветных" точек
	void CreateNewColoredPoint(Point2f mouse_coordinates);
	// Удаляем объект "обычную" точку
	void DeletePoints(Point2i mouse_coordinates);
	// Удаляем объекты "цветная" точка
	void DeleteColoredPoints();
	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// Обычный режим
	void DefaultModeHandler(int event, int x, int y);
	// Режим выделения региона интереса
	void RoiSelectionHandler(int event, int x, int y);
	// Удаляет перспективные искажения
	Mat MakeWarpedFrame(Mat frame, std::vector<Point2i> warping_figure);
	// Трекает точки алгоритмом Лукаса-Канаде и вызывает БПФ
	void TrackAndCalc();
	// Находит подходящие для трекинга точки на изображении
	std::vector<Point2f> FindGoodFeatures(Mat frame, Rect roi);
	// Рисует точки и выводит данные
	void DrawDebugLkWinRectangle(Mat& frame);
	// Транслирует координаты ресайзнутого окна в пространство координат нересайзнутого окна
	Point2f TranslateCoordinates(Point2f point);

private:
	// Фреймпроцессор
	FrameHandler* frame_handler;
	// Флаг работающей программы
	bool running_;
	// Коэффициент автоскейла размеров окна в зависимости от разрешения исходного видео
	float res_mp_;
	// Псевдо-id (порядковывй номер точки)
	int point_id_;
	// Последняя позиция курсора в главном окне
	Point last_mouse_position_;
	// Режим работы 
	int current_mode_;

	// NORMAL MODE
	// 
	// Очередь на создание "обычных" точек
	std::vector<Point2i> l_click_queue_;
	// Очередь на создание "цветных" точек
	std::vector<Point2f> c_point_queue_;
	// Очередь на удаление точек
	std::vector<Point2i> delete_queue_;
	// Очередь на отрисовку гистограм
	std::vector<Point2i> histograms_queue_;
	// Предыдущие позиции точек
	std::vector<Point2f> previous_points_coordinates_;
	// Points handling
	std::vector<LonelyPointHandler*> vec_lonely_point_handlers_;
	std::vector<ColoredPointHandler*> vec_colored_point_handlers_;

	// Не ресайзнутый фрейм
	Mat current_tracking_frame_;
	// Ресайзнутый фрейм
	Mat current_tracking_frame_resized_;

	// R MODE
	// 
	// Флаг происходящего выделения ROI
	bool roi_selecting_;
	// Флаг произведенного выделения ROI
	bool roi_selected_;
	// Координаты первой противоположной точки на краю диагонали прямоугольника (top left, bottom left, top right, bottom right - смотря в какую сторону проводить ROI)
	Point2i tl_click_coords_;
	

	int point_offset_;
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

	
	// for detecting the click
	bool point_selected_;
	Point2i click_coords_;

	// for rectangle
	Mat unchanged_frame_;
	Point2i br_click_coords_;
	Point2i mouse_move_coords_;
	bool right_button_down_;
	bool colors_inited_;
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
	// Размер окна для алгоритма Лукаса-Канаде
	int lk_win_size_;
	// Количество слоев пирамиды изображений в алгоритме Л-К
	int level_amount_;

	// useful parameters
	int number_of_points_;
	int number_of_vibrating_pts_;
	bool vibration_inited_;

	// Fast Fourier Transform
	int fps_;
	int update_rate_;
	double frame_time_;
};

#endif