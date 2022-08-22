#include "VVT-V2\frame_handler.h"

FrameHandler::FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string window_name) :
	input_path_{ input_file_name },
	input_cap_{ new VideoCapture(input_file_name) },
	input_cap_status_{ false },
	output_path_{ output_file_name },
	output_cap_{ new VideoWriter(output_file_name, VideoWriter::fourcc('D', 'I', 'V', 'X'), input_cap_->get(CAP_PROP_FPS), Size(input_cap_->get(CAP_PROP_FRAME_WIDTH), input_cap_->get(CAP_PROP_FRAME_HEIGHT))) },
	window_name_{ window_name },
	current_time_of_frame_{ 0 },
	current_pos_of_frame_{ 0 },
	input_fps_{ input_cap_->get(CAP_PROP_FPS) },
	input_frame_height_{ input_cap_->get(CAP_PROP_FRAME_HEIGHT) },
	input_frame_width_{ input_cap_->get(CAP_PROP_FRAME_WIDTH) }
{
	std::cout << "Input fps is: " << input_fps_ << std::endl;
	// создаём окно
	namedWindow(window_name_);
	// 
	ResizeResolution();
}

FrameHandler::~FrameHandler()
{
	destroyWindow(window_name_);
	this->input_cap_->release();
	delete this->input_cap_;
	this->output_cap_->release();
	delete this->output_cap_;
}

void FrameHandler::ReadNextFrame()
{
	input_cap_status_ = input_cap_->read(input_frame_);
	current_time_of_frame_ = input_cap_->get(CAP_PROP_POS_MSEC);
	current_pos_of_frame_ = input_cap_->get(CAP_PROP_POS_FRAMES);
	input_frame_.copyTo(current_frame_);
}

void FrameHandler::ShowFrame(Mat frame)
{
	imshow(window_name_, frame);
}

void FrameHandler::WriteFrame(Mat frame)
{
	output_cap_->write(frame);
}

Mat FrameHandler::GetGrayFrame(Mat frame_to_be_grayed)
{
	Mat grayed_frame;

	cvtColor(frame_to_be_grayed, grayed_frame, COLOR_BGR2GRAY);
	
	if (!frame_to_be_grayed.empty())
	{		
		GaussianBlur(grayed_frame, grayed_frame, Size(3, 3), 0);
	}
	
	return grayed_frame;
}

Mat FrameHandler::ResizeFrame(Mat frame)
{
	if (!resized_resolution_.empty())
		resize(frame, frame, resized_resolution_);
	return frame;
}

Mat FrameHandler::ConcatenateFrames(Mat left_frame, Mat right_frame)
{
	Mat dst;
	std::vector<Mat> vec_of_frames;
	vec_of_frames.push_back(left_frame);
	vec_of_frames.push_back(right_frame);
	hconcat(vec_of_frames, dst);
	return dst;
}

bool FrameHandler::GetInputCapStatus()
{
	return input_cap_status_;
}

Mat FrameHandler::GetCurrentFrame()
{
	return current_frame_;
}

String FrameHandler::GetWindowName()
{
	return window_name_;
}

double FrameHandler::GetCurrentTimeOfFrame()
{
	return current_time_of_frame_;
}

int FrameHandler::GetCurrentPosOfFrame()
{
	return current_pos_of_frame_;
}

int FrameHandler::GetAmountOfFrames()
{
	return input_cap_->get(CAP_PROP_FRAME_COUNT);
}

int FrameHandler::GetInputFps()
{
	return input_fps_;
}

int FrameHandler::GetFrameWidth()
{
	return input_frame_width_;
}

int FrameHandler::GetFrameHeight()
{
	return input_frame_height_;
}

void FrameHandler::ResizeResolution()
{
	if (input_frame_width_ > input_frame_height_)
	{
		switch (static_cast<int>(input_frame_height_))
		{
		case (480):
		{
			std::cout << "0" << std::endl;
			resized_resolution_ = Size(input_frame_width_ * 2, input_frame_height_ * 2);
			break;
		}
		case (720):
		{
			std::cout << "1" << std::endl;
			resized_resolution_ = Size(input_frame_width_, input_frame_height_);
			break;
		}
		case (1080):
		{
			std::cout << "2" << std::endl;
			resized_resolution_ = Size(input_frame_width_ * 0.67, input_frame_height_ * 0.67);
			break;
		}
		case (1520):
		{
			std::cout << "3" << std::endl;
			resized_resolution_ = Size(input_frame_width_ * 0.25, input_frame_height_ * 0.25);
			break;
		}
		case (2160):
		{
			std::cout << "4" << std::endl;
			resized_resolution_ = Size(input_frame_width_ * 0.25, input_frame_height_ * 0.25);
			break;
		}
		}
	}
	else
	{
		switch (static_cast<int>(input_frame_width_))
		{
		case (480):
		{
			resized_resolution_ = Size(static_cast<int>(input_frame_width_ * 2), static_cast<int>(input_frame_height_ * 2));
			break;
		}
		case (720):
		{
			resized_resolution_ = Size(static_cast<int>(input_frame_width_), static_cast<int>(input_frame_height_));
			break;
		}
		case (1080):
		{
			resized_resolution_ = Size(input_frame_width_ * 0.5, input_frame_height_ * 0.5);
			break;
		}
		case (1520):
		{
			resized_resolution_ = Size(input_frame_width_ * 0.25, input_frame_height_ * 0.25);
			break;
		}
		case (2160):
		{
			resized_resolution_ = Size(input_frame_width_ * 0.25, input_frame_height_ * 0.25);
			break;
		}
		}
	}
}
