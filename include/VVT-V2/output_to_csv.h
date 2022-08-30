#ifndef OUTPUT_TO_CSV_H
#define OUTPUT_TO_CSV_H

// my headers
#include "VVT-V2/vibrating_point.h"

// standart headers
#include <iostream>
#include <fstream>

/*!
* @brief ����� ��� ���������� ���������� (��������� � ���������������� ���������, ID �����)
*
* ��� ��������� ����� ����� ��������� ���: output_year-mm-dd_hh-mm-ss.csv
* 
* ��� ������������ ��� ��������� ������� ����� ������, � �� ����� ������ ������ ���������. ������� ����� ��������� ���������� ������ ����� � ���� ���� (� ����� ���������)
*/
class OutputToCsv {
public:
	/*!
	* @brief ����������� ����� ������
	* @param output_file_name: ��� ��������� �����.
	* @param point_coordinates: ������ �� ��������� ���������������� ��������� �����
	* @param point_time_coordinates: ������ �� ��������� ��������� ��������� �����
	* @param point_id: ID �����
	*/
	OutputToCsv(std::string output_file_name, std::vector<Point2f>& point_coordinates, std::vector<double>& point_time_coordinates, int point_id);
	/*!
	* @brief ��������� ������ ��� ������
	*/
	void Write();

private:
	/*!
	* @brief ��� (����) �����, � ������� ����� ������������� ������
	*/
	// 
	std::string output_file_name_;
	/*!
	* @brief ��������� ��� ���������������� ��������� (������� �� ������) �����
	*/
	std::vector<Point2f>* coordinates_;
	/*!
	* @brief ��������� ��� ��������� ��������� (������� �� ������) �����
	*/
	std::vector<double>* time_coordinates_;
	/*!
	* @brief ID �����
	*/
	int point_id_;
};

#endif