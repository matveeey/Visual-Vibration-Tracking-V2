#include "CamCalib\video_undistorter.h"

VideoUndistorter::VideoUndistorter(std::string input_file_name, std::string output_file_name, std::string txt_file_name) :
	input_file_name_{ input_file_name },
	output_file_name_{ output_file_name },
	txt_file_name_{ txt_file_name }
{
	input_cap_ = new VideoCapture(input_file_name);

	frame_width_ = input_cap_->get(CAP_PROP_FRAME_WIDTH);
	frame_height_ = input_cap_->get(CAP_PROP_FRAME_HEIGHT);
	bitrate_ = input_cap_->get(CAP_PROP_BITRATE);
	frame_count_ = input_cap_->get(CAP_PROP_FRAME_COUNT);
	fps_ = input_cap_->get(CAP_PROP_FPS);

	output_cap_ = new VideoWriter
	(
		output_file_name_,
		cv::VideoWriter::fourcc('m', 'p', '4', 'v'), // codec
		fps_,
		Size(frame_width_, frame_height_)
	);

}

VideoUndistorter::~VideoUndistorter()
{
	input_cap_->release();
	output_cap_->release();

	delete input_cap_;
	delete output_cap_;

	destroyWindow(winname_);
}

void VideoUndistorter::ExecuteVideoUndistortion()
{
	// read distortion parameters from txt file
	LoadFoundParamsFromFile(txt_file_name_);

	std::cout << frame_count_ << std::endl;

	int frame_num = 0;
	Mat mapX, mapY;
	initUndistortRectifyMap(camera_matrix_, distortion_coefficients_, Matx33f::eye(), camera_matrix_, Size(frame_width_, frame_height_), CV_32FC1, mapX, mapY);
	//undistort each frame in loop
	while (input_cap_->get(CAP_PROP_POS_FRAMES) < frame_count_)
	{
		input_cap_->read(current_frame_);
		if (!current_frame_.empty())
		{
			std::cout << "Processing frame " << input_cap_->get(CAP_PROP_POS_FRAMES) << "/" << frame_count_ << std::endl;
			//imshow(winname_, current_frame_);
			
			//undistort(current_frame_, undistorted_frame_, camera_matrix_, distortion_coefficients_);
			remap(current_frame_, undistorted_frame_, mapX, mapY, INTER_LINEAR);
			imshow(winname_, undistorted_frame_);
			SaveFrame(undistorted_frame_);
			waitKey(20);
		}
	}

	std::cout << "Undistortion completed" << std::endl;
}

void VideoUndistorter::SaveFrame(Mat frame)
{
	output_cap_->write(frame);
}

void VideoUndistorter::LoadFoundParamsFromFile(std::string txt_file_name)
{
	
	std::vector<double> dist;

	std::ifstream file;
	std::string line;
	std::vector<std::vector<double>> read_data;

	file.open(txt_file_name);
	if (file.is_open())
	{
		std::cout << "Param file opened" << std::endl;

		while (!file.eof())
		{
			file >> line;
			std::vector<std::string> delimited_line = SplitString(line, ";");
			delimited_line.erase(delimited_line.begin());

			std::vector<double> data;
			double hui;
			for (int i = 0; i < delimited_line.size(); i++)
			{
				data.push_back(std::stod(delimited_line[i]));
			}
			read_data.push_back(data);
		}

		file.close();
	}

	fx_ = read_data[0][0];
	fy_ = read_data[1][0];
	px_ = read_data[2][0];
	py_ = read_data[3][0];
	dist = { read_data[4][0], read_data[4][1], read_data[4][2], read_data[4][3], read_data[4][4] };

	std::vector<double> intrin_arr = { fx_, 0, px_, 0, fy_, py_, 0, 0, 1 };

	camera_matrix_ = Mat(3, 3, CV_64F, intrin_arr.data());
	distortion_coefficients_ = Mat(1, dist.size(), CV_64F, dist.data());

	std::cout << camera_matrix_.size() << std::endl;
	std::cout << distortion_coefficients_.size() << std::endl;
	std::cout << camera_matrix_ << std::endl;
	std::cout << distortion_coefficients_ << std::endl;
}

std::vector<std::string> VideoUndistorter::SplitString(std::string input_string, std::string delimiter)
{
	int start_position = 0;
	int end_position;
	int delimiter_length = delimiter.length();

	std::string token;
	std::vector<std::string> left;

	while ((end_position = input_string.find(delimiter, start_position)) != std::string::npos) {
		token = input_string.substr(start_position, end_position - start_position);
		start_position = end_position + delimiter_length;
		left.push_back(token);
	}

	left.push_back(input_string.substr(start_position));
	return left;
}

