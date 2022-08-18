#include "VVT-V2/histogram.h"

Histogram::Histogram(int width, int height, int x_limit) :
	histogram_frame_width_{ width },
	histogram_frame_height_{ height },
	x_limit_{ x_limit }

{
	// Инициализация гистограммы пустым кадром черного цвета
	histogram_offset_ = static_cast<float>(histogram_frame_width_) * 0.05;
	histogram_ = Mat(Size(histogram_frame_width_, histogram_frame_height_), CV_32F, Scalar(0, 0, 0));
	namedWindow("histogram", WINDOW_AUTOSIZE);
}

void Histogram::plot_histogram()
{
	histogram_ = calc_histogram();
	imshow("histogram", histogram_);
}

void Histogram::set_x_values(std::vector<double> x_values)
{
	x_values_ = x_values;
}

void Histogram::set_y_values(std::vector<float> y_values)
{
	y_values_ = y_values;
}

Mat Histogram::calc_histogram()
{
	float interval = ((histogram_.cols) - histogram_offset_ * 2.0) / y_values_.size();
	
	Mat frame = Mat(Size(histogram_frame_width_, histogram_frame_height_), CV_32F, Scalar(0, 0, 0));
	
	rectangle(frame, Rect(
		Point2f(histogram_offset_ * 0.5, histogram_offset_ * 0.5),
		Point2f(histogram_frame_width_ - histogram_offset_ * 0.5, histogram_frame_height_ - histogram_offset_ * 0.5)),
		Scalar(255, 255, 255),
		1
	);


	for (int i = 0; i < y_values_.size(); i++)
	{
		float x_0 = histogram_offset_ + i * interval + interval;
		float y_0 = histogram_frame_height_ - histogram_offset_;
		float x_1 = x_0;
		float y_1 = histogram_frame_height_ - histogram_offset_ - y_values_[i] * histogram_frame_height_ * 7;

		line(frame, Point2f(x_0, y_0), Point2f(x_1, y_1), Scalar(255, 255, 255), 1, LINE_AA);
	}

	return frame;
}
