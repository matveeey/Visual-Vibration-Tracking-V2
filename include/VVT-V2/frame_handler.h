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
* @brief Класс для чтения и сохранения кадров видео, а также различной их обработки
* 
* Объект класса захватывает последовательность кадров по имени (пути) входного видео, после чего, обращаясь к объекту класса можно считывать кадры с помощью метода ReadNextFrame
* 
* Для записи кадра необходимо использовать метод WriteFrame, который в качестве аргумента принимает cv::Mat объект (изображение) и записывает его в конец выходной последовательности (видео)
* @note Дополнительно добавляет на кадр подсказки для пользователя (метод AddTips) и градиент (GenerateGradScale)
* @note Дополнительно может конкатенировать (объединять в одно изображение два) cv::Mat-сущности по горизонтали или по вертикали
*/
class FrameHandler
{
public:
	/*!
	* @brief Конструктор этого класса
	* @param input_file_name: имя (путь) входного видео для обработки
	* @param output_file_name: имя (путь) выходдного видео после обработки
	* @param main_window_name: название главного окна
	*/
	FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string main_window_name);
	~FrameHandler();
	/*!
	* @brief Считывает следующий кадр видео
	*/
	void ReadNextFrame();
	/*!
	* @brief Выводит в главное окно принимаемое изображение
	* @param frame: изображение, которое нужно отобразить в окне
	* @param fullscreen: флаг полноэкрааного режима
	*/
	void ShowFrame(Mat frame, bool fullscreen);
	/*!
	* @brief Записывает кадр с помощью output_cap
	* @param frame: изображение, которое будет записано в последовательность (фактически добавлено в её конец)
	*/
	void WriteFrame(Mat frame);
	/*!
	* @brief Объединяет два кадра горизонтально
	* @param left_frame: кадр, который будет слева
	* @param right_frame: кадр, который будет справа
	* @return dst: объединённый кадр
	*/
	Mat ConcatenateFramesHorizontally(Mat left_frame, Mat right_frame);
	/*!
	* @brief Объединяет два кадра вертикально
	* @param top_frame: кадр, который будет сверху
	* @param bottom_frame: кадр, который будет снизу
	* @return dst: объединённый кадр
	*/
	Mat ConcatenateFramesVertically(Mat top_frame, Mat bottom_frame);

// Геттеры
public:
	/*!
	* @brief Возвращает серый кадр с блюром
	* @param frame_to_be_grayed: изображение, которое будет сконвертировано в grayscale и на которое будет наложен небольшой блюр (для удаления цифровых шумов)
	* @return Обработанный кадр (cv::Mat)
	*/
	Mat MakeGrayFrame(Mat frame_to_be_grayed);
	/*!
	* @brief Возвращает текущий статус input_cap
	* @return true: кадр был считан, false: кадр не был считан
	*/
	bool GetInputCapStatus();
	/*!
	* @brief Возвращает последний прочитанный на данный момент кадр
	* @return cv::Mat instance
	*/
	Mat GetCurrentFrame();
	/*!
	* @brief Возвращает название главного окна, в которое выводятся кадры
	* @return Название окна
	*/
	String GetMainWindowName();
	/*!
	* @brief Возвращает текущее время кадра (таймкод в мс)
	* @return Текущее время кадра в миллисекундах
	*/
	double GetCurrentTimeOfFrame();
	/*!
	* @brief Возвращает текущее положение кадра относительно первого
	* @return Номер кадра в последовательности
	*/
	int GetCurrentPosOfFrame();
	/*!
	* @brief Возвращает общее количество кадров
	* @return Количество кадров в последовательности
	*/
	int GetAmountOfFrames();
	/*!
	* @brief Возвращает FPS считываемого видео
	* @return FPS
	*/
	int GetInputFps();
	/*!
	* @brief Возвращает ширину исходного видео
	* @return Ширина исходного видео
	*/
	int GetFrameWidth();
	/*!
	* @brief Возвращает высоту считываемого видео
	* @return Высота исходного видео
	*/
	int GetFrameHeight();
	/*!
	* @brief Возвращает коэффициент масштабирования
	* @return Коэффициент масштабирования
	*/
	float GetTextResizeFactor();
	/*!
	* @brief Добавляет (отрисовывает) подсказки для пользователя на кадр
	* @param frame: изображение, на которое будут нанесены подсказки для пользователя
	* @param mode: текущий режим работы приложения
	* @return cv::Mat кадр с подсказками
	* @note Режимы работы приложения: default, ROI selecting, pause
	*/
	Mat AddTips(Mat frame, int mode);
	/*!
	* @brief Генерирует шкалу с градиентом и подписями. Ширина шкалы - ширина исходного видео. Высота регулируется внутри
	* @param left_limit: граница градации цветов слева
	* @param right_limit: граница градации цветов справа
	* @param colored_point_mode: режим подсветки цветных точек
	* @return cv::Mat сущность с изображением градиента шириной равной ширине исходного видео и настраиваемой внутри высотой
	* @note Режимы подсветки цветных точек: стандартный (default), подсветка по частоте вибрации, подсветка по амплитуде
	*/
	Mat GenerateGradScale(int left_limit, int right_limit, int colored_point_mode);

private:
	/*!
	* @brief Инициализирует коэффициент масштабирования текста в зависимости от разрешения видео
	*/
	void InitTextResizeFactors();

private:
	/*!
	@brief Последнний прочитанный кадр
	*/
	Mat current_frame_;
	/*!
	@brief Указатель на объект-захватчик входных кадров видео
	*/
	VideoCapture* input_cap_;
	/*!
	@brief Флаг, принимающий true при удачном чтении кадра и false, если следующий кадр не был считан
	*/
	bool input_cap_status_;
	/*!
	@brief Указатель на объект, сохраняющий кадры в последовательность (видео) в зависимости от выбранного кодека (cv::fourcc)
	*/
	VideoWriter* output_cap_;
	/*!
	@brief Имя главного окна
	*/
	std::string main_window_name_;

	/*!
	@brief Флаг полноэкранного режима
	*/
	bool fullscreen_;

	// Параметры видео
	/*!
	@brief Ширина кадра исходного видео
	*/
	double input_frame_width_;
	/*!
	@brief Высота кадра исходного видео
	*/
	double input_frame_height_;
	/*!
	@brief Соотношение сторон исходного видео
	*/
	double input_frame_size_ratio_;
	/*!
	@brief FPS исходного видео
	*/
	double input_fps_;
	/*!
	@brief Текущее время кадра из последовательности
	*/
	double current_time_of_frame_;
	/*!
	@brief Текущий номер кадра из последовательности
	*/
	int current_pos_of_frame_;
	/*!
	@brief Коэффициент масштабирования текста (зависит от разрешения исходного видео)
	*/
	float text_resize_factor_;
	/*!
	@brief Тексты подсказок для пользователя
	*/
	std::vector<std::string> tip_text_;

};

#endif