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
	// Открывает нужные пути и тп
	void LoadPaths();
	// Выполнить калибровку камеры по картинкам в файле
	void ExecuteCameraCalibration(std::string input_file_name, std::string chessboards_path);
	// Выполнить удаление дисторсии
	void ExecuteVideoUndistortion(std::string input_file_name, std::string output_file_name);
	// Выполнить детекцию вибрации (основной режим)
	void ExecuteVibrationMonitoring(std::string input_file_name, std::string output_file_name);
	// Выполнить увеличение движения
	void ExecuteMotionMagnification(std::string input_file_name, std::string output_file_name);
	// Строит сетку меню с кнопками и надписями на Mat-объекте menu_
	void CreateMenu();
	// Раскрашивает кнопки и наносит на них текст
	void UpdateBtnColorAndText();
	// callback function for determining the event click on mouse
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	// "helper" function for implementing callback function as a method of C++ class
	void DetectEvent(int event, int x, int y, int flags);
	// Основной алгоритм
	void Idle();

private:
	// Путь файла с параметрами
	std::string params_file_name_;
	// Название окна
	std::string winname_;
	// Mat-объект. По сути картинка
	Mat menu_;
	// "Сетка" ГУИ
	std::vector<Rect> buttons_;
	// Тексты кнопок
	std::string button_text_[4];
	// Высота и ширина меню
	int menu_width_;
	int menu_height_;
	// Количество кнопок
	int button_amount_;
	// Высота и ширина кнопок
	int button_width_;
	int button_height_;
	// Различные отступы
	int button_spacing_;
	int top_offset_;
	int bottom_offset_;
	int width_offset_;
	// Атрибуты текста
	int font;
	double font_scale;
	int thickness;
	int baseline;

	// Последние найденные в callback координаты курсора
	Point2i last_mouse_position_;
	// Контейнер для хранения цвета каждой кнопки
	std::vector<Scalar> buttons_color_;
	// Флаг выбранного режима
	bool mode_chosen_;
	// Флаг выхода из программы
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