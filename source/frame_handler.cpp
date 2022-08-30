#include "VVT-V2\frame_handler.h"

FrameHandler::FrameHandler(const std::string input_file_name, const std::string output_file_name, const std::string main_window_name) :
	input_cap_{ new VideoCapture(input_file_name) },
	input_cap_status_{ false },
	output_cap_{ new VideoWriter(output_file_name, VideoWriter::fourcc('D', 'I', 'V', 'X'), input_cap_->get(CAP_PROP_FPS), Size(input_cap_->get(CAP_PROP_FRAME_WIDTH), input_cap_->get(CAP_PROP_FRAME_HEIGHT))) },
	main_window_name_{ main_window_name },
	current_time_of_frame_{ 0 },
	current_pos_of_frame_{ 0 },
	fullscreen_{ false },
	input_fps_{ input_cap_->get(CAP_PROP_FPS) },
	input_frame_height_{ input_cap_->get(CAP_PROP_FRAME_HEIGHT) },
	input_frame_width_{ input_cap_->get(CAP_PROP_FRAME_WIDTH) },
	input_frame_size_ratio_{ input_frame_width_ / input_frame_height_ },
	text_resize_factor_{ 1.0f }
{
	std::cout << "Input fps is: " << input_fps_ << std::endl;
	// создаём окно
	namedWindow(main_window_name_, WINDOW_NORMAL);

	// Находим коэффициент масштабирования для текста
	InitTextResizeFactors();
	
	// Инициализируем текст подсказок
	tip_text_.push_back("Default Mode");
	tip_text_.push_back("ROI selecting mode (Press 'R')");
	tip_text_.push_back("Pause (Press 'Space')");
	tip_text_.push_back("Fullscreen (Press 'F')");
}

FrameHandler::~FrameHandler()
{
	destroyWindow(main_window_name_);
	this->input_cap_->release();
	delete this->input_cap_;
	this->output_cap_->release();
	delete this->output_cap_;
}
void FrameHandler::ReadNextFrame()
{
	input_cap_status_ = input_cap_->read(current_frame_);
	current_time_of_frame_ = input_cap_->get(CAP_PROP_POS_MSEC);
	current_pos_of_frame_ = input_cap_->get(CAP_PROP_POS_FRAMES);
}

void FrameHandler::ShowFrame(Mat frame, bool fullscreen)
{
	fullscreen_ = fullscreen;

	if (fullscreen_)
	{
		setWindowProperty(main_window_name_, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	}
	else
	{
		setWindowProperty(main_window_name_, WND_PROP_FULLSCREEN, WINDOW_NORMAL);
	}

	imshow(main_window_name_, frame);
}

void FrameHandler::WriteFrame(Mat frame_to_be_written)
{
	output_cap_->write(frame_to_be_written);
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
	return main_window_name_;
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

Mat FrameHandler::AddTips(Mat frame, int mode)
{
	// Параметры шрифта
	int font = FONT_HERSHEY_PLAIN;
	double font_scale = 1.5 * text_resize_factor_;
	int thickness = 2;
	int baseline = 0;
	int line_spacing = 5;
	int border_offset = 15;
	Scalar default_tip_color(150, 150, 150);
	Scalar mode_color(100, 255, 100);
	// Выводим тексты подсказок для режимов
	for (int i = 0; i < tip_text_.size() - 1; i++)
	{
		Scalar font_color = default_tip_color;
		if (i == mode)
		{
			font_color = mode_color;
			// Замена кнопки для переключения режимов подсвечивания точек. Пока так)
		}
		putText(
			frame,
			tip_text_[i],
			Point2i(
				border_offset,
				border_offset * input_frame_size_ratio_ + i * (getTextSize(tip_text_[i], font, font_scale, thickness, &baseline).height + line_spacing)
			),
			font,
			font_scale,
			font_color,
			thickness,
			4
		);
	}

	// Выводим текст подсказки для полноэкранного режима
	Scalar font_color = default_tip_color;

	if (fullscreen_)
		font_color = Scalar(255, 100, 100);
	putText(
		frame,
		tip_text_.back(),
		Point2i(
			(input_frame_width_ - border_offset - (getTextSize(tip_text_.back(), font, font_scale, thickness, &baseline).width)),
			border_offset * input_frame_size_ratio_
		),
		font,
		font_scale,
		font_color,
		thickness,
		4
	);

	return frame;
}

Mat FrameHandler::GenerateGradScale(int left_limit, int right_limit, int colored_point_mode)
{
	// Параметры шрифта
	int font = FONT_HERSHEY_PLAIN;
	double font_scale = 1 * text_resize_factor_;
	int thickness = 1;
	int baseline = 0;
	int font_offset = 5;
	int font_height = getTextSize("tmp", font, font_scale, thickness, &baseline).height;
	// Параметры окна шкалы
	int mat_width = input_frame_width_;
	int scale_height = 10;
	int mat_height = scale_height + font_height + font_offset;
	int scale_left_offset = input_frame_width_ * 0.1;
	int scale_width = input_frame_width_ - scale_left_offset;

	Mat frame = Mat(Size(mat_width, mat_height), current_frame_.type(), Scalar(0, 0, 0));
	float interval = static_cast<float>(scale_width) / static_cast<float>(right_limit - left_limit + 1);
	// Флаг текущей итерации
	int iteration = 0;

	// Отрисовываем цветные прямоугольнички
	for (int x = left_limit; x <= right_limit; x++)
	{
		Rect scale_unit = Rect(
			Point2f(interval * iteration + scale_left_offset, 0),
			Point2f(interval + interval * iteration + scale_left_offset, scale_height)
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
			Point2f(
				scale_unit.tl().x + (interval) / 2 - current_text_size.width / 2,
				scale_unit.br().y + scale_height + (current_text_size.height) / 2
			),
			font,
			font_scale,
			Scalar(255, 255, 255),
			thickness,
			4
		);

		iteration++;
	}

	// TEMPORARY!!!
	// Отдельно выводим два варианта режима при активном ROI - подсвечивание точек по амплитуде/частоте
	std::string coloring_points_mode_tips[3] = { "Default", "Coloring by Frequency", "Coloring by Amplitude" };
	Size current_text_size = getTextSize(coloring_points_mode_tips[colored_point_mode], font, font_scale, thickness, &baseline);

	std::cout << "current mode: " << coloring_points_mode_tips[colored_point_mode] << std::endl;

	putText(
		frame,
		coloring_points_mode_tips[colored_point_mode],
		Point2f(
			scale_left_offset / 2 - current_text_size.width / 2,
			mat_height / 2 + current_text_size.height / 2
		),
		font,
		font_scale,
		Scalar(255, 255, 255),
		thickness,
		4
	);

	return frame;
}

void FrameHandler::InitTextResizeFactors()
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
			break;
		}
		case (720):
		{
			text_resize_factor_ = 0.67f;
			break;
		}
		case (1080):
		{
			text_resize_factor_ = 1.0f;
			break;
		}
		case (1520):
		{
			text_resize_factor_ = 1.40f;
			break;
		}
		case (2160):
		{
			text_resize_factor_ = 2.0f;
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
			break;
		}
		case (720):
		{
			text_resize_factor_ = 0.67f;
			break;
		}
		case (1080):
		{
			text_resize_factor_ = 1.0f;
			break;
		}
		case (1520):
		{
			text_resize_factor_ = 1.4f;
			break;
		}
		case (2160):
		{
			text_resize_factor_ = 2.0f;
			break;
		}
		}
	}
}
