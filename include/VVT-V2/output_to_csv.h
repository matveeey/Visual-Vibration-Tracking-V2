#ifndef OUTPUT_TO_CSV_H
#define OUTPUT_TO_CSV_H

// my headers
#include "VVT-V2/vibrating_point.h"

// standart headers
#include <iostream>
#include <fstream>

/*!
* @brief Класс для сохранения метаданных (временных и пространственных координат, ID точки)
*
* Имя выходного файла имеет следующий вид: output_year-mm-dd_hh-mm-ss.csv
* 
* Оно генерируется вне дочернего объекта этого класса, а во время начала работы программы. Поэтому можно сохранять метаданные разных точек в один файл (с одним названием)
*/
class OutputToCsv {
public:
	/*!
	* @brief Конструктор этого класса
	* @param output_file_name: имя выходного файла.
	* @param point_coordinates: ссылка на контейнер пространственных координат точки
	* @param point_time_coordinates: ссылка на контейнер временных координат точки
	* @param point_id: ID точки
	*/
	OutputToCsv(std::string output_file_name, std::vector<Point2f>& point_coordinates, std::vector<double>& point_time_coordinates, int point_id);
	/*!
	* @brief Добавляет данные для записи
	*/
	void Write();

private:
	/*!
	* @brief Имя (путь) файла, в который будет производиться запись
	*/
	// 
	std::string output_file_name_;
	/*!
	* @brief Контейнер для пространственных координат (позиций на экране) точки
	*/
	std::vector<Point2f>* coordinates_;
	/*!
	* @brief Контейнер для временных координат (позиций на экране) точки
	*/
	std::vector<double>* time_coordinates_;
	/*!
	* @brief ID точки
	*/
	int point_id_;
};

#endif