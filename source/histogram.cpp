#include "VVT-V2/histogram.h"

//Histogram::Histogram()
//{
//}

Histogram::Histogram(int width, int height, int x_limit, int id) :
	histogram_frame_width_{ width },
	histogram_frame_height_{ height },
	x_limit_{ x_limit },
	is_histogram_plotted_{ false },
	scale_coefficient_{ 5 }

{
	// Инициализация гистограммы пустым кадром черного цвета
	histogram_offset_ = static_cast<float>(histogram_frame_width_) * 0.05;
	histogram_ = Mat(Size(histogram_frame_width_, histogram_frame_height_), CV_32F, Scalar(0, 0, 0));
	// Инициализируем название окна
	winname_ = "histogram " + std::to_string(id);
}

Histogram::~Histogram()
{
	// Пока это закоменчено, тк есть проблема с выделением памяти. Когда решу, раскоменчу
	//destroyWindow(winname_);
}

void Histogram::OnMouse(int event, int x, int y, int flags, void* userdata)
{
	Histogram* d = static_cast<Histogram*>(userdata);
	d->DetectEvent(event, x, y, flags);
}

void Histogram::DetectEvent(int event, int x, int y, int flags)
{
	switch (event) {
		// ЛКМ была прожата вниз
	case EVENT_LBUTTONDOWN:
	{
		is_histogram_plotted_ = false;
		destroyWindow(winname_);
		break;
	}
	case EVENT_MOUSEMOVE:
	{
		last_mouse_coordinates_.x = x;
		last_mouse_coordinates_.y = y;
		break;
	}
	}
}

void Histogram::plot_histogram()
{
	if (is_histogram_plotted_)
	{
		histogram_ = calc_histogram();
		imshow(winname_, histogram_);
		setMouseCallback(winname_, OnMouse, (void*)this);
	}
}

void Histogram::set_x_values(std::vector<double> x_values)
{
	x_values_ = x_values;
}

void Histogram::set_y_values(std::vector<float> y_values)
{
	y_values_ = y_values;
}

void Histogram::SetHistogramFlag(bool flag)
{
	is_histogram_plotted_ = flag;
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

	float max_value = 0.0;
	float min_value = 10.0;

	for (int i = 0; i < y_values_.size(); i++)
	{
		if (y_values_[i] > max_value)
			max_value = y_values_[i];
		if (y_values_[i] < min_value)
			min_value = y_values_[i];
	}

	scale_coefficient_ = (min_value / max_value);

	std::cout << y_values_.size() << std::endl;
	for (int i = 0; i < y_values_.size(); i++)
	{
		float x_0 = histogram_offset_ + i * interval + interval;
		float y_0 = histogram_frame_height_ - histogram_offset_;
		float x_1 = x_0;
		float y_1 = histogram_frame_height_ - histogram_offset_ - y_values_[i] / max_value * (histogram_frame_height_ - 2 * histogram_offset_);

		// Если мышь указывает на текущее значение, выводим его
		if (IsInteracted(static_cast<int>(x_0), interval))
		{
			PlotMouseValue(frame, i);
		}
		line(frame, Point2f(x_0, y_0), Point2f(x_1, y_1), Scalar(255, 255, 255), 1, LINE_AA);
	}

	return frame;
}

bool Histogram::IsInteracted(int x, int interval)
{
	if (interval > 1)
	{
		Rect interaction_box = Rect(Point2f(x - interval * 0.9, -1), Point2f(x + interval * 0.9, 1));
		return ((interaction_box.contains(Point2i(last_mouse_coordinates_.x, 0))) ? true : false);
	}
	return ((last_mouse_coordinates_.x == x) ? true : false);
}

void Histogram::PlotMouseValue(Mat& frame, int value_idx)
{
	putText(
		frame,
		to_string_with_precision(x_values_[value_idx], 1),
		Point2f(last_mouse_coordinates_.x + histogram_offset_ * 0.5, last_mouse_coordinates_.y + histogram_offset_ * 0.2),
		FONT_HERSHEY_PLAIN,
		1,
		Scalar(255, 255, 255)
	);
}

std::string Histogram::to_string_with_precision(const float value, const int n)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << value;
	return out.str();
}
