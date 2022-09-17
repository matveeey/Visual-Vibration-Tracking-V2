#ifndef CAMERA_CALIBRATOR_H
#define CAMERA_CALIBRATOR_H

// OpenCV headers
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <thread>
#include <chrono>

// my headers
#include "CamCalib\video_undistorter.h"
#include "VVT-V2/helper.h"

constexpr auto EXTENTION = ".JPG";

using namespace cv;

/*!
* @brief Класс для нахождения коэффициентов для конкретной линзы (с помощью которой было снято видео)
*
* Объект класса осуществляет чтение кадров/фотографий с шахматной доской в различных местах на кадре и вычисляет коэффициенты матрицы [] линзы
* TODO: добавить вид этой матрицы (3 х 3)
*/
class CameraCalibrator
{
public:
	/*!
	* @brief Конструктор этого класса
	* @param chessboards_path: имя (путь) к папке с изображениями шахматных досок
	* @note VideoUndistorter
	*/
	CameraCalibrator(std::string chessboards_path);
	~CameraCalibrator();

	int ExecuteCameraCalibration();

private:
	/*!
	* @brief Загружает фотографии (изображения) шахматных досок в память из пути к папке.
	* @note По умолчанию название изображений в пути должно быть по типа: "<номер изображения>.png"
	*/
	int LoadImages(std::string chessboards_path);
	/*!
	* @brief Считывает новое изображение и увеличивает переменную chessboard_amount_ на один
	* @param path: путь к папке с изображениями
	* @return cv::Mat прочитанное изображение
	*/
	Mat ReadNextImage(std::string path);
	/*!
	* @brief По аналогии с FrameHandler::AddTips добавляет подсказки пользователя. В данном случае это - номер текущего обрабатываемого кадра и общее кол-во кадров.\n
	* @param frame: кадр, на который будут добавлены подсказки
	* @param tip: строка с подсказкой
	* @todo Вынести её в какой-то отдельный модуль, чтобы не было дублирования кода
	*/
	void AddTips(Mat& frame, std::string tip);
	/*!
	* @brief Находит шахматный паттерн размером, заданным в конструкторе класса (CameraCalibrator::CameraCalibrator)
	* @param input_frame: кадр, на котором будет осуществлён поиск паттерна
	* @param pattern_size: размер паттерна (по дефолту я выбрал 6х9)
	* @param corners: адрес на вектор, в который будут записаны найденные координаты узлов паттерна
	* @return true - в случае, если паттерн нашёлся, false - в противном случае
	*/
	bool FindCorners(Mat input_frame, Size pattern_size, std::vector<Point2f>& corners);
	/*!
	* @brief Сохраняет найденные параметры (коэффициенты fx, fy, cx, cy), необходимые для удаления дисторсии
	* @param camera_matrix: матрица, которая будет записана в файл
	* @param dist_coeffs_2be_written: коэффициенты дисторсии, которые будут записаны в файл
	*/
	void SaveFoundParamsToFile(Mat camera_matrix, Mat dist_coeffs_2be_written);
	
private:
	/*!
	* @brief Путь к папке с фотографиями "шахматной доски"
	*/
	std::string chessboards_folder_path_;
	/*!
	* @brief Количество фотографий шахматных досок (находится в CameraCalibrator::LoadImages)
	*/
	int chessboards_amount_;
	/*!
	* @brief Вектор загруженных фотографий с шахматными досками
	*/
	std::vector<Mat> images_with_chessboard_;
	/*!
	* @brief "Поток" из кадров входного видео (последовательности)
	*/
	VideoCapture* input_cap_;
	/*!
	* @brief Ширина кадра
	*/
	double frame_width_;
	/*!
	* @brief Высота кадра
	*/
	double frame_height_;
	/*!
	* @brief Название окна
	*/
	std::string winname_;
	/*!
	* @brief Путь к текстовому файлу, в который будут сохранены найденные коэффициенты
	*/
	std::string txt_file_name_;
	/*!
	* @brief Матрица, в которой находятся найденные коэффициенты px, py, cx, cy
	*/
	Mat camera_matrix_;
	/*!
	* @brief Коэффициенты дисторсии
	*/
	Mat distortion_coefficients_;
	std::vector<Mat> rotation_vector_;
	std::vector<Mat> translation_vector_;
	/*!
	* @brief Найденные узлы шахматной доски, найденные на конкретном изображении
	*/
	std::vector<Point2f> corners_;
	/*!
	* @brief Контейнер для хранения найденных узлов шахматных досок
	*/
	std::vector<std::vector<Point2f>> vec_of_corners_;
	/*!
	* @brief Размер паттерна, по которому будут искаться шахматные доски
	*/
	Size pattern_size_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	*/
	double fx_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	*/
	double fy_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	*/
	double px_;
	/*!
	* @brief Коэффициент матрицы VideoUndistorter::camera_matrix_
	*/
	double py_;

};

#endif