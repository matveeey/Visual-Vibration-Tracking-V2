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

// Класс позволяет создать объект, который может выводить видео в окне
class FrameHandler
{
public:
	FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string window_name);
	~FrameHandler();
	// Считывает следующий кадр видео
	void ReadNextFrame();
	// Выводит в окно принимаемый кадр
	void ShowFrame(Mat frame);
	// Записывает кадр с помощью output_Cap
	void WriteFrame(Mat frame);
	// Объединяет два кадра горизонтально
	Mat ConcatenateFrames(Mat left_frame, Mat right_frame);

// Геттеры
public:
	// Добавляет подсказки и шкалы на кадр
	Mat AddTips(Mat frame, int mode);
	// Возвращает серый кадр с блюром
	Mat GetGrayFrame(Mat frame_to_be_grayed);
	// Масштабирует и возвращает кадр в зависимости от разрешения видео
	Mat ResizeFrame(Mat frame);
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
	float GetResizingCoefficient();

protected:
	Mat input_frame_;
	Mat current_frame_;

	VideoCapture* input_cap_;
	bool input_cap_status_;
	VideoWriter* output_cap_;
	std::string input_path_;
	std::string output_path_;

	// Параметры видео
	double input_frame_width_;
	double input_frame_height_;
	double input_fps_;
	// Имя главного окна
	std::string window_name_;
	// Текущая информация о кадре
	double current_time_of_frame_;
	int current_pos_of_frame_;
	// Для масштабирования изображения
	Size resized_resolution_;
	float resizing_coefficient_;
	// Тексты подсказок для пользователя
	std::vector<std::string> tip_text_;

private:
	void ResizeResolution();
};

#endif