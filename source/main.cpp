#include "VVT-V2\vibration_detector.h"
#include "CamCalib\video_undistorter.h"
#include "CamCalib\camera_calibrator.h"
#include "MovEn/movement_enlargement.h"

enum MODE
{
	CALIBRATION,
	UNDISTORTION,
	VIBRATION_MONITORING,
	MOVEMENT_ENLARGEMENT
};

void ExecuteCameraCalibration(std::string input_file_name, std::string chessboards_path)
{
	CameraCalibrator camera_calibrator(input_file_name, chessboards_path);
	camera_calibrator.ExecuteCameraCalibration();
}

void ExecuteVideoUndistortion(std::string input_file_name, std::string output_file_name)
{
	std::string params;

	std::cout << "Please attach the parameters.txt file: ";
	std::cin >> params;
	std::cout << std::endl;

	VideoUndistorter video_undistorter(input_file_name, output_file_name, params);
	video_undistorter.ExecuteVideoUndistortion();
}

void ExecuteVibrationMonitoring(std::string input_file_name, std::string output_file_name)
{
	VibrationDetector vibration_detector(input_file_name, output_file_name, MAIN_WINDOW_NAME);
	vibration_detector.ExecuteVibrationDetection();
}

void ExecuteMotionMagnification(std::string input_file_name, std::string output_file_name)
{
	MovementEnlarger movement_enlarger(input_file_name, output_file_name);
	//movement_enlarger.ExecuteLaplaceEnlargement();
	movement_enlarger.ExecuteRieszEnlargement();
}

int Execute(std::string txt_file_name, int mode)
{
	std::ifstream params_file;
	params_file.open(txt_file_name);

	if (!params_file.is_open())
	{
		std::cout << "Wrong filename" << std::endl;
		return 0;
	}

	std::string input_calibration_video;
	std::string input_distorted_video;
	std::string output_undistorted_video;
	std::string input_vibration_detection;
	std::string output_vibration_detection;
	std::string chessboards_path;
	std::string input_movement_enlargment;
	std::string output_movement_enlargement;

	std::getline(params_file, input_calibration_video);
	std::getline(params_file, input_distorted_video);
	std::getline(params_file, output_undistorted_video);
	std::getline(params_file, input_vibration_detection);
	std::getline(params_file, output_vibration_detection);
	std::getline(params_file, chessboards_path);
	std::getline(params_file, input_movement_enlargment);
	std::getline(params_file, output_movement_enlargement);

	// Вызов нужного режима
	switch (mode)
	{
	case MODE::CALIBRATION:
		std::cout << "u've chosen camera calibration" << std::endl;
		ExecuteCameraCalibration(input_calibration_video, chessboards_path);
		break;
	case MODE::UNDISTORTION:
		std::cout << "u've chosen undistortion" << std::endl;
		ExecuteVideoUndistortion(input_distorted_video, output_undistorted_video);
		break;
	case MODE::VIBRATION_MONITORING:
		std::cout << "u've chosen vibration monitoring" << std::endl;
		ExecuteVibrationMonitoring(input_vibration_detection, output_vibration_detection);
		break;
	case MODE::MOVEMENT_ENLARGEMENT:
		std::cout << "u've chosen movement enlarging" << std::endl;
		ExecuteMotionMagnification(input_movement_enlargment, output_movement_enlargement);
		break;
	}
	return 1;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cout << "Not enough arguments\nAttach a .txt" << std::endl;
		return 0;
	}

	std::string program_name;
	std::string txt_file_name;
	std::string type;

	if (argc > 1)
	{
		program_name = argv[0];
		txt_file_name = argv[1];
		type = argv[2];
	}
	int code = std::stoi(type);

	return Execute(txt_file_name, code);
}