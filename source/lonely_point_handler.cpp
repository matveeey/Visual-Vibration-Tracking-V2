#include "VVT-V2/lonely_point_handler.h"

LonelyPointHandler::LonelyPointHandler(Point2f init_coordinates, double sampling_rate, int point_id, float text_resize_factor, std::string output_csv_filename) :
	Histogram{ 600, 300, sampling_rate / 2, point_id, x_, y_ },
	OutputToCsv{ output_csv_filename, point_coordinates_, point_time_coordinates_, point_id },
	point_id_{ point_id },
	text_resize_factor_{ text_resize_factor }
{
	interaction_offset_ = 50;
	sampling_rate_ = sampling_rate;

	// Добавляем первые координаты клика мышью
	point_coordinates_.push_back(init_coordinates);
	interaction_box_ = Rect(
		Point2i(init_coordinates.x - interaction_offset_, init_coordinates.y - interaction_offset_),
		Point2i(init_coordinates.x + interaction_offset_, init_coordinates.y + interaction_offset_)
	);
	interacted_ = false;

	frequencies_.push_back(0.0);
	amplitude_ = Point3f(0.0, 0.0, 0.0);
}

LonelyPointHandler::~LonelyPointHandler()
{
	Write();
}

Point2f LonelyPointHandler::GetTextCoordinates()
{
	if (!point_coordinates_.empty())
		return point_coordinates_[0];
	else return Point2f(0.0, 0.0);
}

Rect LonelyPointHandler::GetInteractionRect()
{
	return interaction_box_;
}

void LonelyPointHandler::UpdatePointColor()
{
	if (interacted_)
		point_color_ = Scalar(255, 150, 50); // синий светлее
	else
		point_color_ = Scalar(255, 75, 25); // синий темнее
}

void LonelyPointHandler::Draw(Mat& frame)
{
	// отрисовываем прямоугольник взаимодействия (если оно есть)
	DrawInteractionRectangle(frame);
	// отрисовываем линии точек
	DrawPointTrack(frame);
	// отрисовываем круг вокруг точки
	DrawPoint(frame);
	// отрисовываем гистограмму
	DrawHistogram();
	// отрисовываем частоту вибрации
	DrawTextData(frame);
}

void LonelyPointHandler::DrawHistogram()
{
	if (is_histogram_plotted_)
	{
		SetXValues(x_);
		SetYValues(y_);
		ShowHistogram();
	}
}

void LonelyPointHandler::DrawPoint(Mat& frame)
{
	UpdatePointColor();
	// отрисовываем круг вокруг точки
	circle(frame, point_coordinates_.back(), 10, point_color_, 2);

}

void LonelyPointHandler::DrawPointTrack(Mat& frame)
{
	if (point_coordinates_.size() > 2)
		line(frame, point_coordinates_[point_coordinates_.size() - 2], point_coordinates_[point_coordinates_.size() - 1], Scalar(0, 255, 0), 1, LINE_AA);
}

void LonelyPointHandler::DrawInteractionRectangle(Mat& frame)
{
	if (interacted_)
		rectangle(frame, interaction_box_, Scalar(0, 255, 0), 2);
}

void LonelyPointHandler::DrawTextData(Mat& frame)
{
	int font = FONT_HERSHEY_PLAIN;
	double font_scale = 1.5 * text_resize_factor_;
	int thickness = 2;
	int line_spacing = 20;
	// Количество строк частот, выводимых на экран
	int lines_amount = 3;
	// отрисовываем частоту вибрации
	for (int j = 0; j < ((frequencies_.size()) & (lines_amount)); j++)
	{
		putText(
			frame,
			"hz: " + std::to_string(frequencies_[j]),
			Point(point_coordinates_.front().x + 15, point_coordinates_.front().y + text_resize_factor_ * j * line_spacing),
			font,
			font_scale,
			Scalar(0, 69, 255),
			thickness
		);
	}

	// отрисовываем амплитуды вибрации и ID точки
	putText(
		frame,
		"ID: " + std::to_string(point_id_),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - text_resize_factor_ * line_spacing),
		font,
		font_scale,
		Scalar(100, 255, 100),
		thickness
	);
	putText(
		frame,
		"x: " + std::to_string(amplitude_.x),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - text_resize_factor_ * 2 * line_spacing),
		font,
		font_scale,
		Scalar(0, 255, 255),
		thickness
	);
	putText(
		frame,
		"y: " + std::to_string(amplitude_.y),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - text_resize_factor_ * 3 * line_spacing),
		font,
		font_scale,
		Scalar(0, 255, 255),
		thickness
	);
	// uncomment когда появится 3-я координата для амплитуды :)
	/*putText(
		frame,
		"z: " + std::to_string(amplitude_.z),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - res_mp_ * 4 * line_spacing),
		FONT_HERSHEY_PLAIN,
		font_scale,
		Scalar(0, 255, 255),
		thickness
	);*/
}