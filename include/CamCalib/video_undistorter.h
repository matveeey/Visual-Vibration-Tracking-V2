#ifndef VIDEO_UNDISTORTER_H
#define VIDEO_UNDISTORTER_H

// OpenCV headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/videoio/videoio.hpp>

#include <iostream>
#include <fstream>
#include <iomanip>

// my headers
#include "VVT-V2/helper.h"

using namespace cv;

/*!
* @brief Класс для удаления дисторсии на видео
*
* Объект класса осуществляет чтение и сохранение кадров с удаленной дисторсией в выходную последовательность (видео)
* 
* @todo Работу с кадрами (чтение, грейскейл) следует выполнять с помощью FrameHandler. На данный момент это происходит не так. Внутри этого класса своя реализация чтения видео.\n
* cv::VideoCapture способно считывать любую последовательность изображений, не обязательно видео, можно и набор изображений по какому-то пути (из папки, например).\n
* Это не успел сделать
*/
class VideoUndistorter
{
public:
	/*!
	* @brief Конструктор этого класса
	* @param input_file_name: имя (путь) входного видео для обработки
	* @param output_file_name: имя (путь) выходдного видео после обработки
	* @param txt_file_name: имя (путь) файла с "коэффициентами дисторсии" линзы
	* @note CameraCalibrator
	*/
	VideoUndistorter(std::string input_file_name_, std::string output_file_name, std::string txt_file_name);
	~VideoUndistorter();
	/*!
	* @brief Основная часть. Выполняет удаление дисторсии из видео
	* @return 0 - в случае выхода из программы (смотри тело функции). 1 - в противном
	* @todo Лучше переделать такую логику)
	*/
	int ExecuteVideoUndistortion();

private:
	/*!
	* @brief По аналогии с FrameHandler::AddTips добавляет подсказки пользователя. В данном случае это - номер текущего обрабатываемого кадра и общее кол-во кадров.\n
	* @param frame: кадр, на который будут добавлены подсказки
	* @param tip: строка с подсказкой
	* @todo Вынести её в какой-то отдельный модуль, чтобы не было дублирования кода
	*/
	void AddTips(Mat& frame, std::string tip);
	/*!
	* @brief Сохраняет кадр в выходную последовательность (видео)
	* @param frame: кадр, который будет сохранён
	* @todo Предложения по улучшению описаны описании класса (VideoUndistorter)
	*/
	void SaveFrame(Mat frame);
	/*!
	* @brief Загружает параметры (коэффициенты fx, fy, cx, cy), необходимые для удаления дисторсии. Были найдены в CameraCalibrator
	* @param txt_file_name: путь к файлу с коэффициентами
	*/
	void LoadFoundParamsFromFile(std::string txt_file_name);
	/*!
	* @brief Разделяет строку на некоторое количество подстрок по специальному символу - разделителю
	* @param input_string: исходная строка
	* @param delimiter: разделитель, по которому исходная строка будет разделена
	* @return Вектор, состоящий из разделенных частей исходной строки (вектор из векторов)
	*/
	std::vector<std::string> SplitString(std::string line, std::string delimiter);

private:
	/*!
	* @brief Название главного окна
	*/
	std::string winname_;
	/*!
	* @brief "Поток" из кадров входного видео (последовательности)
	*/
	VideoCapture* input_cap_;
	/*!
	* @brief "Поток" из кадров выходного видео (последовательности)
	*/
	VideoWriter* output_cap_;
	/*!
	* @brief Ширина кадра (входного и выходного)
	*/
	double frame_width_;
	/*!
	* @brief Ширина кадра (входного и выходного)
	*/
	double frame_height_;
	/*!
	* @brief Количество кадров входного видео
	*/
	double frame_count_;
	/*!
	* @brief ФПС входного видео
	*/
	double fps_;
	/*!
	* @brief Путь к входному видео
	*/
	std::string input_file_name_;
	/*!
	* @brief Путь к выходному видео
	*/
	std::string output_file_name_;
	/*!
	* @brief Та самая матрица, присущая конкретной паре - линща + камера.
	* @see CameraCalibrator
	*/
	Mat camera_matrix_;
	/*! @brief Коэффициенты дисторсии
	* @note CameraCalibrator
	*/
	Mat distortion_coefficients_;
	/*!
	* @brief Текущий кадр с дисторсией
	*/
	Mat current_frame_;
	/*!
	* @brief Текущий кадр с удалённой дисторсией
	*/
	Mat undistorted_frame_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double fx_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double fy_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double px_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	* @see CameraCalibrator
	*/
	double py_;
	/*!
	* @brief Путь к файлу с коэффициентами
	* @see CameraCalibrator
	*/
	std::string txt_file_name_;
};

#endif