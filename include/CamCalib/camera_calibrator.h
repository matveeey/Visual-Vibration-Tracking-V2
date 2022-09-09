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
	void AddTips(Mat& frame, std::string tip);

	bool FindCorners(Mat input_frame, Size pattern_size, std::vector<Point2f>& corners);
	void SaveFoundParamsToFile(Mat camera_matrix, Mat dist_coeffs_2be_written);
	
private:
	std::string chessboards_folder_path_;
	int chessboards_amount_;
	std::vector<Mat> images_with_chessboard_;

	VideoCapture* input_cap_;

	double frame_width_;
	double frame_height_;

	std::string winname_;
	std::string txt_file_name_;
	Mat camera_matrix_;
	Mat distortion_coefficients_;
	std::vector<Mat> rotation_vector_;
	std::vector<Mat> translation_vector_;

	Mat current_frame_;

	std::vector<Point2f> corners_;
	std::vector<std::vector<Point2f>> vec_of_corners_;
	Size pattern_size_;

	double fx_;
	double fy_;
	double px_;
	double py_;

};

#endif