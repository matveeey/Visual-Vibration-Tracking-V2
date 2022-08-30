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

// my headers

using namespace cv;

/*!
* @brief Класс для удаления дисторсии на видео
*
* Объект класса осуществляет чтение и сохранение кадров с удаленной дисторсией в выходную последовательность (видео)
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

	void ExecuteVideoUndistortion();

private:
	void SaveFrame(Mat frame);
	void LoadFoundParamsFromFile(std::string txt_file_name);
	std::vector<std::string> SplitString(std::string line, std::string delimiter);

private:
	VideoCapture* input_cap_;
	VideoWriter* output_cap_;

	double frame_width_;
	double frame_height_;
	double bitrate_;
	double frame_count_;
	double fps_;

	std::string input_file_name_;
	std::string output_file_name_;
	Mat camera_matrix_;
	Mat distortion_coefficients_;

	Mat current_frame_;
	Mat undistorted_frame_;

	std::ifstream input_file_;

	double fx_;
	double fy_;
	double px_;
	double py_;

	std::string txt_file_name_;
};

#endif