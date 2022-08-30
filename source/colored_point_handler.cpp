#include "VVT-V2/colored_point_handler.h"

ColoredPointHandler::ColoredPointHandler(Point2f init_coordinates, double sampling_rate, int point_id, float radius_resize_factor, std::string output_csv_filename) :
	Histogram{ 600, 300, sampling_rate / 2, point_id },
	OutputToCsv{ output_csv_filename, point_coordinates_, point_time_coordinates_, point_id },
	point_id_{ point_id },
	radius_resize_factor_{ 1.25f / radius_resize_factor },
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

	point_radius_ = 10 * radius_resize_factor_;
	frequencies_.push_back(0.0);
	amplitude_ = Point3f(0.0, 0.0, 0.0);

	// TEMPORARY
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
	circle(frame, point_coordinates_.back(), point_radius_, point_color_, FILLED);
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
		point_radius_ = 1;
		point_color_ = Scalar(0, 0, 255);
		break;
	}
	case COLORING_BASED_ON_FREQUENCY:
	{
		point_radius_ = 1;
		// переведЄм диапазон частоты [0; range_limit] в [0; 255] int color value;
		if (!frequencies_.empty())
		{
			for (int i = 0; i < frequencies_.size(); i++)
			{
				point_color_= HelperFunctions::RatioToRgb(frequencies_[i] / (sampling_rate_ / 2));
			}
		}
		break;
	}
	case COLORING_BASED_ON_AMPLITUDE:
	{
		point_radius_ = 1;
		// переведЄм диапазон амплитуд [0; max_amplitude_] в [0; 255] int color value;
		double amplitude = sqrt(amplitude_.x * amplitude_.x + amplitude_.y * amplitude_.y);
		point_color_ = HelperFunctions::RatioToRgb(amplitude / max_amplitude_);
		break;
	}
	}
	if (interacted_)
	{
		point_color_ = Scalar(255, 255, 255);
		point_radius_ *= 10;
	}
}
