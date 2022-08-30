#ifndef MOVEMENT_ENLARGEMENT_H
#define MOVEMENT_ENLARGEMENT_H

// C++ standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/frame_handler.h"
#include "MovEn/eulerian_enlargement.h"

using namespace cv;

/*!
* @brief Дочерний (от EulerEnlarger) класс для визуального увеличения вибрации на видео
*/
class MovementEnlarger : public EulerEnlarger
{
public:
	/*!
	* @brief Конструктор этого класса
	* @param input_file_name: имя (путь) входного файла (видео)
	* @param output_file_name: имя (путь) выходного файла (видео)
	*/
	MovementEnlarger(std::string input_file_name, std::string output_file_name);
	~MovementEnlarger();
	/*!
	* @brief Выполняет визуальное увеличение движения
	*/
	void ExecuteLaplaceEnlargement();

private:
	/*!
	* @brief имя (путь) входного файла (видео)
	*/
	std::string input_file_name_;
	/*!
	* @brief имя (путь) выходного файла (видео)
	*/
	std::string output_file_name_;
	/*!
	* @brief Имя окна, в которое будет выводиться обработанный кадр
	*/
	std::string win_name_;

	/*!
	* @brief Изображение (кадр), движение на котором будет увеличено
	*/
	Mat frame_to_be_enlarged_;
	/*!
	* @brief Изображение (кадр), движение на котором было увеличено
	*/
	Mat frame_enlarged_;
	/*!
	* @brief Mat-сущность, содержащая информацию об увеличенном движении
	*/
	Mat frame_with_motion_info_;
	
};

#endif