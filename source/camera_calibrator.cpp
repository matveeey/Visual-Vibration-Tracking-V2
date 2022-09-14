#include "CamCalib\camera_calibrator.h"

CameraCalibrator::CameraCalibrator(std::string chessboards_path) :
	chessboards_folder_path_{ chessboards_path },
	winname_{ "Camera Calibrator" }
{
	// наших углов должен быть 10 и 7 по горизонтали и вертикали соответственно
	pattern_size_ = Size(10, 7);

	txt_file_name_ = "parameters.txt";
	namedWindow(winname_, WINDOW_NORMAL);
}

CameraCalibrator::~CameraCalibrator()
{
	destroyWindow(winname_);
}

int CameraCalibrator::ExecuteCameraCalibration()
{
	std::cout << "Starting camera calibration..." << std::endl;

	// Загружаем изображения в память
	LoadImages(chessboards_folder_path_);

	auto it = images_with_chessboard_.begin();
	while (it != images_with_chessboard_.end())
	{
		Mat current_image = *it;
		if (FindCorners(current_image, pattern_size_, corners_))
		{
			std::cout << "Pattern found" << std::endl;
			drawChessboardCorners(current_image, pattern_size_, corners_, true);
			vec_of_corners_.push_back(corners_);
		}

		it++;
		AddTips(current_image, "Current frame: " + std::to_string(std::distance(images_with_chessboard_.begin(), it)) + "/" + std::to_string(images_with_chessboard_.size()));
		imshow(winname_, current_image);

		int code = waitKey(20);
		switch (code)
		{
			// Пауза
			// Клавиши - пробел (ASCII code)
		case 32:
		{
			waitKey(0);
			break;
		}
		case 'q':
		{
			it = images_with_chessboard_.end();
			return 0;
			break;
		}
		}
	}

	// Создаем координатную плоскость
	std::vector<Point3f> object_points;
	for (int i = 0; i < pattern_size_.height; i++)
	{
		for (int j = 0; j < pattern_size_.width; j++)
		{
			object_points.push_back(Point3f(i, j, 0));
		}
	}
	std::vector<std::vector<Point3f>> vec_of_object_points(vec_of_corners_.size());
	for (int i = 0; i < vec_of_corners_.size(); i++)
	{
		vec_of_object_points[i] = object_points;
	}
	/////////////////////////////////////////////

	std::cout << "Camera calibration..." << std::endl;
	float error = calibrateCamera
	(
		vec_of_object_points,
		vec_of_corners_,
		Size(frame_height_, frame_width_),
		camera_matrix_,
		distortion_coefficients_,
		rotation_vector_,
		translation_vector_
	);
	std::cout << "Error is " << error << std::endl;
	std::cout << "camera_matrix_ is " << camera_matrix_ << std::endl;
	std::cout << "distortion_coefficients_ is " << distortion_coefficients_ << std::endl;

	for (int i = 0; i < rotation_vector_.size(); i++)
	{
		std::cout << "rotation_vector_ is " << rotation_vector_[i] << std::endl;
	}
	for (int i = 0; i < translation_vector_.size(); i++)
	{
		std::cout << "translation_vector_ is " << translation_vector_[i] << std::endl;
	}

	// saving found parameters to a txt file
	SaveFoundParamsToFile(camera_matrix_, distortion_coefficients_);
	return 1;
}

int CameraCalibrator::LoadImages(std::string chessboards_path)
{
	Mat image_with_chessboard;
	std::string image_name = chessboards_path;
	chessboards_amount_ = 0;
	
	image_with_chessboard = ReadNextImage(image_name);

	// Инициализируем ширину и высоту кадра
	frame_width_ = image_with_chessboard.size().width;
	frame_height_ = image_with_chessboard.size().height;

	while (!image_with_chessboard.empty())
	{
		images_with_chessboard_.push_back(image_with_chessboard);
		imshow(winname_, image_with_chessboard);
		image_with_chessboard = ReadNextImage(image_name);

		int code = waitKey(20);
		switch (code)
		{
			// Пауза
			// Клавиши - пробел (ASCII code)
		case 32:
		{
			waitKey(0);
			break;
		}
		case 'q':
		{
			return 0;
			break;
		}
		}
	}
	return 1;
}

Mat CameraCalibrator::ReadNextImage(std::string path)
{
	Mat image;
	path += std::to_string(chessboards_amount_++);
	path += EXTENTION;
	image = imread(path);
	return image;
}

void CameraCalibrator::AddTips(Mat& frame, std::string tip)
{
	int font = FONT_HERSHEY_PLAIN;
	double font_scale = 1.5;
	int thickness = 2;

	putText(
		frame,
		tip,
		Point(frame.cols * 0.05, frame.rows * 0.05),
		font,
		font_scale,
		Scalar(0, 255, 0),
		thickness
	);
}

bool CameraCalibrator::FindCorners(Mat input_frame, Size pattern_size, std::vector<Point2f>& corners)
{
	Mat gray_frame;
	if (input_frame.channels() > 2)
		cvtColor(input_frame, input_frame, COLOR_BGR2GRAY);

	input_frame.copyTo(gray_frame);

	bool pattern_found = findChessboardCorners
	(
		gray_frame,
		pattern_size,
		corners,
		CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK
	);

	if (pattern_found)
		cornerSubPix
		(
			gray_frame, corners,
			Size(11, 11), // window size
			Size(-1, -1), // zero zone
			TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 40, 0.001)
		);

	return pattern_found;
}

void CameraCalibrator::SaveFoundParamsToFile(Mat camera_matrix, Mat dist_coeffs_2be_written)
{
	std::string filename = HelperFunctions::GenerateCsvFilename("parameters_");

	fx_ = camera_matrix.at<double>(0, 0);
	fy_ = camera_matrix.at<double>(1, 1);
	px_ = camera_matrix.at<double>(0, 2);
	py_ = camera_matrix.at<double>(1, 2);

	std::vector<double> distortion_coefficients;
	distortion_coefficients =
	{
		dist_coeffs_2be_written.at<double>(0,0),
		dist_coeffs_2be_written.at<double>(0,1),
		dist_coeffs_2be_written.at<double>(0,2),
		dist_coeffs_2be_written.at<double>(0,3),
		dist_coeffs_2be_written.at<double>(0,4)
	};

	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::trunc);

	file << "fx;" + HelperFunctions::ToStringWithPrecision(fx_) << std::endl;
	file << "fy;" + HelperFunctions::ToStringWithPrecision(fy_) << std::endl;
	file << "px;" + HelperFunctions::ToStringWithPrecision(px_) << std::endl;
	file << "py;" + HelperFunctions::ToStringWithPrecision(py_) << std::endl;
	file << "dist;" + HelperFunctions::ToStringWithPrecision(distortion_coefficients[0]) + ";" + HelperFunctions::ToStringWithPrecision(distortion_coefficients[1])
		+ ";" + HelperFunctions::ToStringWithPrecision(distortion_coefficients[2]) + ";" + HelperFunctions::ToStringWithPrecision(distortion_coefficients[3]) + ";" + HelperFunctions::ToStringWithPrecision(distortion_coefficients[4]);

	file.close();
}