#ifndef MOVEMENT_ENLARGEMENT_H
#define MOVEMENT_ENLARGEMENT_H

// C++ standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/frame_handler.h"
#include "MovEn/laplace_enlargement.h"
#include "MovEn/riesz_enlargement.h"

using namespace cv;

class MovementEnlarger : public LaplaceEnlarger, RieszEnlarger
{
public:
	MovementEnlarger(std::string input_file_name, std::string output_file_name);
	~MovementEnlarger();
	// Выполняет визуальное увеличение движения
	void ExecuteLaplaceEnlargement();
	void ExecuteRieszEnlargement();

private:
	


private:
	// IO
	std::string input_file_name_;
	std::string output_file_name_;
	std::string win_name_;

	// Кадры
	Mat frame_to_be_enlarged_;
	Mat frame_enlarged_;
	Mat frame_with_motion_info_;
	
};

#endif