#ifndef OUTPUT_TO_CSV_H
#define OUTPUT_TO_CSV_H
#define _CRT_SECURE_NO_WARNINGS

#include "VVT-V2/vibrating_point.h"

// standart headers
#include <iostream>
#include <fstream>

class OutputToCsv {
public:
	OutputToCsv(std::string output_file_name, std::vector<Point2f>& point_coordinates, std::vector<double>& point_time_coordinates, int point_id);
	// Добавляет данные для записи
	void Write();

private:
	// Имя файла, в который будет производиться запись
	std::string output_file_name_;
	// Контейнеры для координат (позиций) точки и временных координат
	std::vector<Point2f>* coordinates_;
	std::vector<double>* time_coordinates_;
	// ID точки
	int point_id_;
};

#endif