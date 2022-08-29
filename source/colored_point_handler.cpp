#include "VVT-V2/colored_point_handler.h"

ColoredPointHandler::ColoredPointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient) :
	Histogram{ 600, 300, sampling_rate / 2, point_id },
	update_rate_{ update_rate },
	point_id_{ point_id },
	resizing_coefficient_{ 1.25f / resizing_coefficient },
	mode_{ COLORING_BASED_ON_FREQUENCY }
{
	interaction_offset_ = 3;
	sampling_rate_ = sampling_rate;

	// Добавляем первые координаты клика мышью
	point_coordinates_.push_back(init_coordinates);
	interaction_box_ = Rect(
		Point2i(init_coordinates.x - interaction_offset_, init_coordinates.y - interaction_offset_),
		Point2i(init_coordinates.x + interaction_offset_, init_coordinates.y + interaction_offset_)
	);
	interacted_ = false;

	point_radius_ = 10 * resizing_coefficient_;
	frequencies_.push_back(0.0);
	amplitude_ = Point3f(0.0, 0.0, 0.0);

	// TEMPORARY
	point_color_ = Scalar(0, 0, 255);
}

ColoredPointHandler::~ColoredPointHandler()
{
	OutputToCsv output_to_scv("C:/Users/seeyo/source/repos/Visual-Vibration-Tracking-V2/docs/output.csv", point_coordinates_, point_time_coordinates_, point_id_);
	output_to_scv.Write();
}

void ColoredPointHandler::Draw(Mat& frame)
{
	// отрисовываем круг вокруг точки
	DrawPoint(frame);
	// отрисовываем гистограмму
	DrawHistogram();
}

void ColoredPointHandler::SetMode(int mode)
{
	mode_ = mode;
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
	switch (mode_)
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
		// переведём диапазон частоты [0; range_limit] в [0; 255] int color value;
		if (!frequencies_.empty())
		{
			for (int i = 0; i < frequencies_.size(); i++)
			{
				point_color_= RatioToRgb(frequencies_[i] / (sampling_rate_ / 2));
			}
		}
		break;
	}
	case COLORING_BASED_ON_AMPLITUDE:
	{
		point_radius_ = 1;
		// переведём диапазон амплитуд [0; max_amplitude_] в [0; 255] int color value;
		double amplitude = sqrt(amplitude_.x * amplitude_.x + amplitude_.y * amplitude_.y);
		point_color_ = RatioToRgb(amplitude / max_amplitude_);
		break;
	}
	}
	if (interacted_)
	{
		point_color_ = Scalar(255, 255, 255);
		point_radius_ *= 10;
	}
}

Scalar ColoredPointHandler::RatioToRgb(double ratio)
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
