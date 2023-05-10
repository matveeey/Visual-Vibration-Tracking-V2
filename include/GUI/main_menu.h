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
* @brief Класс для создания начального меню выбора режима работы программы
*/
class MainMenu
{
	/*!
	* @brief Перечисление со списком основных режимов работы
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
	* @brief Конструктор этого класса
	* @param txt_file_name: имя (путь) текстового документа, содержащий нужные для работы пути к входным и выходным расположениям
	*/
	MainMenu(std::string txt_file_name);

private:
	/*!
	* @brief Открывает нужные пути
	*/
	void LoadPaths();
	/*!
	* @brief Запускает калибровку камеры по картинкам шахматных досок
	* @param chessboards_path: имя (путь) к папке с изображениями шахматных досок
	*/
	void ExecuteCameraCalibration(std::string chessboards_path);
	/*!
	* @brief Запускает удаление дисторсии
	* @param input_file_name: имя (путь) входного видео
	* @param output_file_name: имя (путь) выходного файла
	*/
	void ExecuteVideoUndistortion(std::string input_file_name, std::string output_file_name);
	/*!
	* @brief Запускает детекцию вибрации (основной режим)
	* @param input_file_name: имя (путь) входного видео
	* @param output_file_name: имя (путь) выходного файла
	*/
	void ExecuteVibrationMonitoring(std::string input_file_name, std::string output_file_name);
	/*!
	* @brief Запускает увеличение движения
	* @param input_file_name: имя (путь) входного видео
	* @param output_file_name: имя (путь) выходного файла
	*/
	void ExecuteMotionMagnification(std::string input_file_name, std::string output_file_name);
	/*!
	* @brief Строит сетку меню с кнопками и надписями на Mat-объекте menu_
	*/
	void CreateMenu();
	/*!
	* @brief Раскрашивает кнопки и наносит на них текст
	*/
	void UpdateBtnColorAndText();
	/*!
	* @brief callback функция для определения события (используется для хэндлинга клика мышью)
	*/
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	/*!
	* @brief callback функция для определения события (используется для хэндлинга клика мышью)
	*/
	void DetectEvent(int event, int x, int y, int flags);
	/*!
	* Запускает меню
	*/
	void Idle();

private:
	/*!
	* @brief Путь файла с параметрами
	*/
	std::string params_file_name_;
	/*!
	* @brief Название окна
	*/
	std::string winname_;
	/*!
	* @brief Изображение меню, на котором будет происходить отрисовка
	*/
	Mat menu_;
	/*!
	* @brief Контейнер, хранящий в себе прямоугольники-кнопки (фактически тут хранятся координаты кнопок)
	*/
	std::vector<Rect> buttons_;
	/*!
	* @brief Тексты кнопок
	*/
	std::string button_text_[4];
	/*!
	* @brief Ширина меню
	*/
	int menu_width_;
	/*!
	* @brief Высота меню
	*/
	int menu_height_;
	/*!
	* @brief Количество кнопок
	*/
	int button_amount_;
	/*!
	* @brief Ширина кнопки
	*/
	int button_width_;
	/*!
	* @brief Высота кнопки
	*/
	int button_height_;
	/*!
	* @brief Отступ между кнопками
	*/
	int button_spacing_;
	/*!
	* @brief Отступ от застаканных кнопок сверху
	*/
	int top_offset_;
	/*!
	* @brief Отступ от застаканных кнопок снизу
	*/
	int bottom_offset_;
	/*!
	* @brief Отступ с двух сторон от кнопки по ширине
	*/
	int width_offset_;
	/*!
	* @brief Шрифт
	*/
	int font;
	/*!
	* @brief Величина шрифта
	*/
	double font_scale;
	/*!
	* @brief Толщина шрифта
	*/
	int thickness;
	/*!
	* @brief Параметр для cv::getText()
	*/
	int baseline;

	/*!
	* @brief Последние найденные в callback координаты курсора
	*/
	Point2i last_mouse_position_;
	/*!
	* @brief Контейнер для хранения цвета каждой кнопки
	*/
	std::vector<Scalar> buttons_color_;
	/*!
	* @brief Флаг выбранного режима
	*/
	bool mode_chosen_;
	/*!
	* @brief Флаг выхода из программы
	*/
	bool wanna_quit_;

private:
	/*!
	* @brief Имя (путь) входного файла (видео) для калибровки
	*/
	std::string input_calibration_video_;
	/*!
	* @brief Имя (путь) входного файла (видео) для удаления дисторсии
	*/
	std::string input_distorted_video_;
	/*!
	* @brief Имя (путь) выходного файла (видео) после удаления дисторсии
	*/
	std::string output_undistorted_video_;
	/*!
	* @brief Имя (путь) входного файла (видео) для детекции вибрации
	*/
	std::string input_vibration_detection_;
	/*!
	* @brief Имя (путь) выходного файла (видео) после детекции вибрации
	*/
	std::string output_vibration_detection_;
	/*!
	* @brief Путь к папке с фотографиями шахматной доски
	*/
	std::string chessboards_path_;
	/*!
	* @brief Имя (путь) входного файла (видео) для усиления движения
	*/
	std::string input_movement_enlargment_;
	/*!
	* @brief Имя (путь) выходного файла (видео) для усиления движения
	*/
	std::string output_movement_enlargement_;
	
};

#endif