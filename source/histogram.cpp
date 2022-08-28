#include "VVT-V2/histogram.h"

Histogram::Histogram(int width, int height, int x_limit, int id) :
	histogram_frame_width_{ width },
	histogram_frame_height_{ height },
	x_limit_{ x_limit },
	is_histogram_plotted_{ false },
	signature_amount_{ 8 },
	dead_zone_coeff_{ 0.1f }
{
	// ������������� ����������� ������ ������ ������� �����
	histogram_offset_ = static_cast<float>(histogram_frame_width_) * 0.05;
	axis_signature_offset_ = static_cast<float>(histogram_frame_width_) * 0.03;
	histogram_ = Mat(Size(histogram_frame_width_, histogram_frame_height_), CV_32F, Scalar(0, 0, 0));
	// �������������� �������� ����
	winname_ = "histogram " + std::to_string(id);
	// ��������� �������� ����� ��������� ���
	signature_interval_ = (histogram_.cols - histogram_offset_ * 2.0) / static_cast<float>(signature_amount_ - 1);
	// �������������� ��� �����������
	InitHistogramBackground();
}

Histogram::~Histogram()
{
	std::cout << "Histogram destructor" << std::endl;

	if (is_histogram_plotted_)
		destroyWindow(winname_);
}

void Histogram::OnMouse(int event, int x, int y, int flags, void* userdata)
{
	Histogram* d = static_cast<Histogram*>(userdata);
	d->DetectEvent(event, x, y, flags);
}

void Histogram::DetectEvent(int event, int x, int y, int flags)
{
	switch (event) {
		// ��� ���� ������� ����
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

void Histogram::ShowHistogram()
{
	if (is_histogram_plotted_)
	{
		histogram_ = CalcHistogram();
		imshow(winname_, histogram_);
		setMouseCallback(winname_, OnMouse, (void*)this);
	}
}

void Histogram::SetXValues(std::vector<double> x_values)
{
	x_values_ = x_values;
}

void Histogram::SetYValues(std::vector<float> y_values)
{
	y_values_ = y_values;
}

void Histogram::SetHistogramFlag(bool flag)
{
	is_histogram_plotted_ = flag;
}

void Histogram::SetHistogramWindowProperty(int prop_id, int prop_value)
{
	setWindowProperty(winname_, prop_id, prop_value);
}

void Histogram::InitHistogramBackground()
{
	Mat frame = Mat(Size(histogram_frame_width_, histogram_frame_height_), CV_32F, Scalar(0, 0, 0));

	// ������������ ����� �����������
	rectangle(frame, Rect(
		Point2f(histogram_offset_ * 0.5, histogram_offset_ * 0.5), // top-left
		Point2f(histogram_frame_width_ - histogram_offset_ * 0.5, histogram_frame_height_ - histogram_offset_ * 0.5 - axis_signature_offset_)), // bottom-right
		Scalar(255, 255, 255),
		1
	);

	// ������������ �������
	for (int i = 0; i < signature_amount_; i++)
	{
		float signature_start = static_cast<int>(dead_zone_coeff_ * x_limit_);
		float signature_interval = static_cast<float>(x_limit_ - signature_start) / static_cast<float>(signature_amount_ - 1);
		float value = signature_start + signature_interval * i;
		putText(
			frame,
			to_string_with_precision(value, 1),
			Point2f(histogram_offset_ / 2.0 + i * signature_interval_, histogram_frame_height_ - axis_signature_offset_ / 2.0),
			FONT_HERSHEY_PLAIN,
			1,
			Scalar(255, 255, 255)
		);
	}

	frame.copyTo(histogram_background_);
}

Mat Histogram::CalcHistogram()
{
	// ������� ������� ����
	for (int i = 0; i < static_cast<int>(x_values_.size() * dead_zone_coeff_); i++)
	{
		x_values_.erase(x_values_.begin());
		y_values_.erase(y_values_.begin());
	}

	float interval = ((histogram_.cols) - histogram_offset_ * 2.0) / y_values_.size();
	
	Mat frame = histogram_background_.clone();

	// �������������� ������������ � ����������� �������� ��������
	float max_value = 0.0;

	for (int i = 0; i < y_values_.size(); i++)
	{
		if (y_values_[i] > max_value)
			max_value = y_values_[i];
	}


	// ��������� ��������
	for (int i = 0; i < y_values_.size(); i++)
	{
		float x_0 = histogram_offset_ + i * interval + interval;
		float y_0 = histogram_frame_height_ - histogram_offset_ - axis_signature_offset_;
		float x_1 = x_0;
		float y_1 = y_0 - y_values_[i] / max_value * (histogram_frame_height_ - 2 * histogram_offset_ - axis_signature_offset_);

		// ���� ���� ��������� �� ������� ��������, ������������ ��� �������� ����� � ��������
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
