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
@brief Класс для чтения и сохранения кадров видео, а также различной их обработки
*
* Основной текст
@note Объяснение
*/
class FrameHandler
{
public:
	/*!
	* Конструктор класса принимает в качестве аргументов следующее:
	* @param input_file_name - имя (путь) входного видео для обработки
	* @param output_file_name - имя (путь) выходдного видео после обработки
	* @param main_window_name - название главного окна
	*/
	FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string main_window_name);
	~FrameHandler();
	// Считывает следующий кадр видео
	void ReadNextFrame();
	// Выводит в главное окно принимаемое изображение
	// fullscreen - флаг полноэкранного режима
	void ShowFrame(Mat frame, bool fullscreen);
	// Записывает кадр с помощью output_Cap
	void WriteFrame(Mat frame);
	// Объединяет два кадра горизонтально
	Mat ConcatenateFramesHorizontally(Mat left_frame, Mat right_frame);
	// Объединяет два кадра вертикально
	Mat ConcatenateFramesVertically(Mat top_frame, Mat bottom_frame);

// Геттеры
public:
	// Возвращает серый кадр с блюром
	Mat GetGrayFrame(Mat frame_to_be_grayed);
	// Возвращает текущий статус input_cap
	bool GetInputCapStatus();
	// Возвращает последний прочитанный на данный момент кадр
	Mat GetCurrentFrame();
	// Возвращает название окна, в которое выводятся кадры
	String GetWindowName();
	// Возвращает текущее время кадра (таймкод в мс)
	double GetCurrentTimeOfFrame();
	// Возвращает текущее положение кадра относительно первого
	int GetCurrentPosOfFrame();
	// Возвращает общее количество кадров
	int GetAmountOfFrames();
	// Возвращает FPS считываемого видео
	int GetInputFps();
	// Возвращает ширину считываемого видео
	int GetFrameWidth();
	// Возвращает высоту считываемого видео
	int GetFrameHeight();
	// Возвращает коэффициент масштабирования
	float GetTextResizeFactor();
	// Добавляет подсказки для пользователя на кадр
	Mat AddTips(Mat frame, int mode);
	// Генерирует шкалу с градиентом и подписями. Ширина шкалы - ширина исходного видео. Высота регулируется внутри
	Mat GenerateGradScale(int left_limit, int right_limit, int colored_point_mode);

private:
	// Инициализирует коэффициент масштабирования текста в зависимости от разрешения видео
	void InitTextResizeFactors();

protected:
	// Последнний прочитанный кадр
	Mat current_frame_;
	// Указатель на объект-захватчик входных кадров видео
	VideoCapture* input_cap_;
	// Флаг, принимающий true при удачном чтении кадра и false, если следующий кадр не был считан
	bool input_cap_status_;
	// Указатель на объект, сохраняющий кадры в последовательность (видео) в зависимости от выбранного кодека (cv::fourcc)
	VideoWriter* output_cap_;
	// Имя главного окна
	std::string main_window_name_;

	// Флаг полноэкранного режима
	bool fullscreen_;

	// Параметры видео
	// Ширина кадра исходного видео
	double input_frame_width_;
	// Высота кадра исходного видео
	double input_frame_height_;
	// Соотношение сторон исходного видео
	double input_frame_size_ratio_;
	// ФПС исходного видео
	double input_fps_;
	// Текущее время кадра из последовательности
	double current_time_of_frame_;
	// Текущий номер кадра из последовательности
	int current_pos_of_frame_;
	// Коэффициент масштабирования текста (зависит от разрешения исходного видео)
	float text_resize_factor_;
	// Тексты подсказок для пользователя
	std::vector<std::string> tip_text_;

};

#endif