#include "VVT-V2/colored_point_handler.h"

ColoredPointHandler::ColoredPointHandler(Point2f init_coordinates, double sampling_rate, int point_id, float radius_resize_factor, std::string output_csv_filename) :
	Histogram{ 600, 300, sampling_rate / 2, point_id, x_, y_ },
	OutputToCsv{ output_csv_filename, point_coordinates_, point_time_coordinates_, point_id },
	point_id_{ point_id },
	radius_resize_factor_{ radius_resize_factor },
	coloring_mode_{ COLORING_BASED_ON_FREQUENCY }
{
	interaction_offset_ = 3;
	sampling_rate_ = sampling_rate;

	// ƒобавл€ем первые координаты клика мышью
	point_coordinates_.push_back(init_coordinates);
	interaction_box_ = Rect(
		Point2i(init_coordinates.x - interaction_offset_, init_coordinates.y - interaction_offset_),
		Point2i(init_coordinates.x + interaction_offset_, init_coordinates.y + interaction_offset_)
	);
	interacted_ = false;

	default_point_radius_ = 4 * radius_resize_factor;
	current_point_radius_ = default_point_radius_;

	frequencies_.push_back(0.0);
	relative_amplitude_ = Point3f(0.0, 0.0, 0.0);

	sensivity_ = 0.01;
	confidence_level_ = 1.0;

	point_color_ = Scalar(0, 0, 255);
}

ColoredPointHandler::~ColoredPointHandler()
{
	Write();
}

void ColoredPointHandler::Draw(Mat& frame)
{
	// отрисовываем круг вокруг точки
	DrawPoint(frame);
	// отрисовываем гистограмму
	DrawHistogram();
}

void ColoredPointHandler::SetColoringMode(int coloring_mode)
{
	coloring_mode_ = coloring_mode;
}

void ColoredPointHandler::DrawHistogram()
{
	if (is_histogram_plotted_)
	{
		SetXValues(x_);
		SetYValues(y_);
		ShowHistogram();
	}
}

void ColoredPointHandler::DrawPoint(Mat& frame)
{
	UpdatePointColor();
	// отрисовываем круг вокруг точки
	circle(frame, point_coordinates_.back(), current_point_radius_, point_color_, FILLED);
}

void ColoredPointHandler::DrawInteractionRectangle(Mat& frame)
{
	rectangle(frame, interaction_box_, Scalar(0, 0, 255), 1);
}

void ColoredPointHandler::UpdatePointColor()
{
	switch (coloring_mode_)
	{
	case DEFAULT:
	{
		current_point_radius_ = default_point_radius_;
		point_color_ = Scalar(0, 0, 255);
		break;
	}
	case COLORING_BASED_ON_FREQUENCY:
	{
		current_point_radius_ = default_point_radius_;
		// переведЄм диапазон частоты [0; range_limit] в [0; 255] int color value;
		if (!frequencies_.empty())
		{
			point_color_ = HelperFunctions::RatioToRgb(main_frequency_ / (sampling_rate_ / 2));
		}
		break;
	}
	case COLORING_BASED_ON_AMPLITUDE:
	{
		current_point_radius_ = default_point_radius_;
		// переведЄм диапазон амплитуд [0; max_amplitude_] в [0; 255] int color value;
		double ratio = (current_amplitude_.x - extremum_amplitude_.second) / (extremum_amplitude_.first - extremum_amplitude_.second);
		point_color_ = HelperFunctions::RatioToRgb(ratio);
		if (point_color_ == Scalar(0, 0, 0))
		{
			std::cout << "ratio: " << ratio << std::endl;
			std::cout << "current_amplitude_.x: " << current_amplitude_.x << std::endl;
			std::cout << "max ampl: " << extremum_amplitude_.first << std::endl;
			std::cout << "min ampl: " << extremum_amplitude_.second << std::endl;
		}
		break;
	}
	}
	if (interacted_)
	{
		point_color_ = Scalar(255, 255, 255);
		current_point_radius_ *= 3;
	}
}
