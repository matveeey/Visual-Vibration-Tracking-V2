#include "VVT-V2/lonely_point_handler.h"

LonelyPointHandler::LonelyPointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient, std::string output_csv_filename) :
	Histogram{ 600, 300, sampling_rate / 2, point_id },
	OutputToCsv{ output_csv_filename, point_coordinates_, point_time_coordinates_, point_id },
	update_rate_{ update_rate },
	point_id_{ point_id },
	resizing_coefficient_{ 1.25f / resizing_coefficient }
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

void LonelyPointHandler::Filter(std::vector<float>& magnitudes)
{
	float cutoff_freq_limit = 0.1f;
	for (int i = 0; i < static_cast<int>(magnitudes.size() * cutoff_freq_limit); i++)
	{
		float coeff = 0.1f / (1.0f / magnitudes[i]);
		magnitudes[i] = magnitudes[i] * coeff;
	}
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
	// Количество строк частот, выводимых на экран
	int lines_amount = 3;
	// отрисовываем частоту вибрации
	for (int j = 0; j < ((frequencies_.size()) && (lines_amount)); j++)
	{
		putText(
			frame,
			"hz: " + std::to_string(frequencies_[j]),
			Point(point_coordinates_.front().x + 15, point_coordinates_.front().y + resizing_coefficient_ * j * 20),
			FONT_HERSHEY_PLAIN,
			resizing_coefficient_ * 1,
			Scalar(0, 69, 255),
			2
		);
	}

	// отрисовываем амплитуды вибрации и ID точки
	putText(
		frame,
		"ID: " + std::to_string(point_id_),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - resizing_coefficient_ * 20),
		FONT_HERSHEY_PLAIN,
		resizing_coefficient_ * 1,
		Scalar(100, 255, 100),
		2
	);
	putText(
		frame,
		"x: " + std::to_string(amplitude_.x),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - resizing_coefficient_ * 2 * 20),
		FONT_HERSHEY_PLAIN,
		resizing_coefficient_ * 1,
		Scalar(0, 255, 255),
		2
	);
	putText(
		frame,
		"y: " + std::to_string(amplitude_.y),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - resizing_coefficient_ * 3 * 20),
		FONT_HERSHEY_PLAIN,
		resizing_coefficient_ * 1,
		Scalar(0, 255, 255),
		2
	);
	// uncomment когда появится 3-я координата для амплитуды :)
	/*putText(
		frame,
		"z: " + std::to_string(amplitude_.z),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - res_mp_ * 4 * 20),
		FONT_HERSHEY_PLAIN,
		res_mp_ * 1,
		Scalar(0, 255, 255),
		2
	);*/
}