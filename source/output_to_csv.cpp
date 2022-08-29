#include "VVT-V2/output_to_csv.h"

OutputToCsv::OutputToCsv(std::string output_file_name, std::vector<Point2f>& point_coordinates, std::vector<double>& point_time_coordinates, int point_id) :
	output_file_name_{ output_file_name },
	coordinates_{ &point_coordinates },
	time_coordinates_{ &point_time_coordinates },
	point_id_{ point_id }
{
}

void OutputToCsv::Write()
{
	std::ofstream file;
	file.open(output_file_name_, std::fstream::app);
	// Запишем ID точки
	file << "Point " + std::to_string(point_id_) + "\n";
	// Запишем временные позиции точки
	file << "Time pos;";
	for (int i = 0; i < time_coordinates_->size(); i++)
	{
		file << std::to_string(time_coordinates_->at(i)) + ";";
	}
	file << "\n";
	// Запишем координаты 
	// Сначала по X
	file << "X pos; ";
	for (int i = 0; i < coordinates_->size(); i++)
	{
		file << std::to_string(coordinates_->at(i).x) + ";";
	}
	file << "\n";
	file << "Y pos; ";
	// Затем по Y
	for (int i = 0; i < coordinates_->size(); i++)
	{
		file << std::to_string(coordinates_->at(i).y) + ";";
	}
	file << "\n";

	file.close();
	
}
