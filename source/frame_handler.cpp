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
	input_frame_width_{ input_cap_->get(CAP_PROP_FRAME_WIDTH) },
	input_frame_size_ratio_{ input_frame_width_ / input_frame_height_ },
	text_resize_factor_{ 1.0f },

	lk_win_size_{ 0 }
{
	std::cout << "Input fps is: " << input_fps_ << std::endl;
	// создаём окно
	namedWindow(window_name_, WINDOW_NORMAL);
	// Находим коэффициент масштабирования для текста
	CalculateTextResizeFactors();
	
	// Инициализируем текст подсказок
	tip_text_.push_back("Default Mode");
	tip_text_.push_back("ROI selecting mode (Press 'R')");
	tip_text_.push_back("Pause (Press 'Space')");
	tip_text_.push_back("Fullscreen (Press 'F')");
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

Mat FrameHandler::AddTips(Mat frame, int mode)
{
	// Параметры шрифта
	int font = FONT_HERSHEY_PLAIN;
	double font_scale = 1;
	int thickness = 2;
	int baseline = 0;
	int line_spacing = 5;
	Scalar default_color(150, 150, 150);
	Scalar mode_color(100, 255, 100);
	for (int i = 0; i < tip_text_.size(); i++)
	{
		Scalar font_color = default_color;
		if (i == mode)
			font_color = mode_color;
		putText(
			frame,
			tip_text_[i],
			Point2i(15 * (1 / text_resize_factor_), 15 * (1 / text_resize_factor_) + i * (getTextSize(tip_text_[i], font, font_scale, thickness, &baseline).height + line_spacing)),
			font,
			font_scale,
			font_color,
			thickness,
			4
		);
	}
	return frame;
}

Scalar RatioToRgb(double ratio)
{
	// we want to normalize ratio so that it fits in to 6 regions
	// where each region is 256 units long
	int normalized = int(ratio * 256 * 4);

	// find the region for this position
	int region = normalized / 256;

	// find the distance to the start of the closest region
	int x = normalized % 256;

	int r = 0, g = 0, b = 0;
	switch (region)
	{
	case 0: r = 255; g = 0;   b = 0;   g += x; break; // красный
	case 1: r = 255; g = 255; b = 0;   r -= x; break; // желтый
	case 2: r = 0;   g = 255; b = 0;   b += x; break; // зеленый
	case 3: r = 0;   g = 255; b = 255; g -= x; break; // сине-зеленый
	case 4: r = 0;   g = 0;   b = 255; r += x; break; // синий
		//case 5: r = 255; g = 0;   b = 255; b -= x; break;
	}
	return Scalar(b, g, r);
}

Mat FrameHandler::GenerateGradScale(int left_limit, int right_limit)
{
	// Параметры шрифта
	int font = FONT_HERSHEY_PLAIN;
	double font_scale = 1;
	int thickness = 1;
	int baseline = 0;
	int line_spacing = 5;
	// Параметры окна шкалы
	int scale_height = 20;
	int scale_bottom_offset = scale_height / 2;
	int scale_width = input_frame_width_;

	Mat frame = Mat(Size(scale_width, scale_height), input_frame_.type(), Scalar(0, 0, 0));
	int interval = scale_width / (right_limit - left_limit + 1);
	// Флаг текущей итерации
	int iteration = 0;

	for (int x = left_limit; x <= right_limit; x++)
	{
		Rect scale_unit = Rect(
			Point2i(interval * iteration, 0),
			Point2i(interval + interval * iteration, scale_height - scale_bottom_offset)
		);
		rectangle(
			frame,
			scale_unit,
			RatioToRgb(static_cast<double>(x) / static_cast<double>(right_limit)),
			FILLED
		);

		std::string scale_signature = std::to_string(x);
		Size current_text_size = getTextSize(scale_signature, font, font_scale, thickness, &baseline);

		putText(
			frame,
			scale_signature,
			Point2i(
				scale_unit.tl().x + (interval) / 2 - current_text_size.width / 2,
				scale_unit.br().y + (scale_height - scale_bottom_offset) / 2 + (current_text_size.height) / 2
			),
			font,
			font_scale,
			Scalar(255, 255, 255),
			thickness,
			4
		);

		iteration++;
	}

	return frame;
}

Mat FrameHandler::GetGrayFrame(Mat frame_to_be_grayed)
{
	Mat grayed_frame;
	
	if (!frame_to_be_grayed.empty())
	{
		// Конвертируем входной Mat из РГБ(БГР) в грейскейл
		cvtColor(frame_to_be_grayed, grayed_frame, COLOR_BGR2GRAY);
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

Mat FrameHandler::ConcatenateFramesHorizontally(Mat left_frame, Mat right_frame)
{
	Mat dst;
	std::vector<Mat> vec_of_frames;
	vec_of_frames.push_back(left_frame);
	vec_of_frames.push_back(right_frame);
	hconcat(vec_of_frames, dst);
	return dst;
}

Mat FrameHandler::ConcatenateFramesVertically(Mat top_frame, Mat bottom_frame)
{
	Mat dst;
	std::vector<Mat> vec_of_frames;
	vec_of_frames.push_back(top_frame);
	vec_of_frames.push_back(bottom_frame);
	vconcat(vec_of_frames, dst);
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

float FrameHandler::GetTextResizeFactor()
{
	return text_resize_factor_;
}

void FrameHandler::CalculateTextResizeFactors()
{
	std::cout << "frame width: " << input_frame_width_ << std::endl;
	std::cout << "frame height: " << input_frame_height_ << std::endl;

	if (static_cast<int>(input_frame_size_ratio_))
	{
		switch (static_cast<int>(input_frame_height_))
		{
		case (480):
		{
			text_resize_factor_ = 0.44f;
			resized_resolution_ = Size(input_frame_width_ * text_resize_factor_, input_frame_height_ * text_resize_factor_);
			break;
		}
		case (720):
		{
			text_resize_factor_ = 0.67f;
			resized_resolution_ = Size(input_frame_width_, input_frame_height_);
			break;
		}
		case (1080):
		{
			text_resize_factor_ = 1.0f;
			resized_resolution_ = Size(input_frame_width_ * text_resize_factor_, input_frame_height_ * text_resize_factor_);
			break;
		}
		case (1520):
		{
			text_resize_factor_ = 1.40f;
			resized_resolution_ = Size(input_frame_width_ * text_resize_factor_, input_frame_height_ * text_resize_factor_);
			break;
		}
		case (2160):
		{
			text_resize_factor_ = 2.0f;
			resized_resolution_ = Size(input_frame_width_ * text_resize_factor_, input_frame_height_ * text_resize_factor_);
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
			text_resize_factor_ = 0.44f;
			resized_resolution_ = Size(static_cast<int>(input_frame_width_ * text_resize_factor_), static_cast<int>(input_frame_height_ * text_resize_factor_));
			break;
		}
		case (720):
		{
			text_resize_factor_ = 0.67f;
			resized_resolution_ = Size(static_cast<int>(input_frame_width_), static_cast<int>(input_frame_height_));
			break;
		}
		case (1080):
		{
			text_resize_factor_ = 1.0f;
			resized_resolution_ = Size(input_frame_width_ * text_resize_factor_, input_frame_height_ * text_resize_factor_);
			break;
		}
		case (1520):
		{
			text_resize_factor_ = 1.4f;
			resized_resolution_ = Size(input_frame_width_ * text_resize_factor_, input_frame_height_ * text_resize_factor_);
			break;
		}
		case (2160):
		{
			text_resize_factor_ = 2.0f;
			resized_resolution_ = Size(input_frame_width_ * text_resize_factor_, input_frame_height_ * text_resize_factor_);
			break;
		}
		}
	}
}
