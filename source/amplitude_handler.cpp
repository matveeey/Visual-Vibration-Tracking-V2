#include "VVT-V2\amplitude_handler.h"

AmplitudeHandler::AmplitudeHandler()

{

}

AmplitudeHandler::~AmplitudeHandler()
{

}

void AmplitudeHandler::CollectTrackedPoints(Point2f coordinates_of_points)
{
	this->coordinates_of_point_.push_back(coordinates_of_points);
	if (coordinates_of_point_.size() > 10)
	{
		coordinates_of_point_.erase(coordinates_of_point_.begin());
		std::cout << "coordinates of point size is: " << coordinates_of_point_.size() << std::endl;
	}
}

Point2f AmplitudeHandler::CalculateAmplitude()
{
	float max_x = 0;
	float min_x = 2000;
	float max_y = 0;
	float min_y = 2000;

	for (int i = 0; i < coordinates_of_point_.size(); i++)
	{
		if (coordinates_of_point_[i].x > max_x)
			max_x = coordinates_of_point_[i].x;
		if (coordinates_of_point_[i].x < min_x)
			min_x = coordinates_of_point_[i].x;
		if (coordinates_of_point_[i].y > max_y)
			max_y = coordinates_of_point_[i].y;
		if (coordinates_of_point_[i].y < min_x)
			min_y = coordinates_of_point_[i].y;
	}

	Scalar mean_value = mean(coordinates_of_point_);

	float amplitude_in_percents_x = (max_x - mean_value.val[0]) / (max_x + max_y - mean_value.val[0] - mean_value.val[1]);//sqrt((max_x / mean_value.val[0]) * (max_x / mean_value.val[0]) + (1 - min_x / mean_value.val[0]) * (1 - min_x / mean_value.val[0]));
	float amplitude_in_percents_y = (max_y - mean_value.val[1]) / (max_x + max_y - mean_value.val[0] - mean_value.val[1]);//sqrt((max_y / mean_value.val[1]) * (max_y / mean_value.val[1]) + (1 - min_y / mean_value.val[1]) * (1 - min_y / mean_value.val[1]));

	std::cout << "amplitude_in_percents_x: " << amplitude_in_percents_x << std::endl;
	std::cout << "amplitude_in_percents_y: " << amplitude_in_percents_y << std::endl;

	if (!amplitudes_.empty())
		return amplitudes_[amplitudes_.size() - 1];
	else return Point2f(amplitude_in_percents_x, amplitude_in_percents_y);
}

Mat AmplitudeHandler::GetWarpedFrame(Mat frame, Rect r, float w, float h)
{
	Point2f src[4] = { Point2f(r.tl().x, r.br().y), r.br(), Point2f(r.br().x, r.tl().y), r.tl() };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	Mat warped_frame;
	warpPerspective(frame, warped_frame, matrix, Point(w, h));
	return warped_frame;
}

double AmplitudeHandler::DistanceBtwn2Points(Point2f first_point, Point2f second_point)
{
	return sqrt((second_point.x - first_point.x) * (second_point.x - first_point.x) + (second_point.y - first_point.y) * (second_point.y - first_point.y));
}
