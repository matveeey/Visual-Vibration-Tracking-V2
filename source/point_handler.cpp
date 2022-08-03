#include "VVT-V2/point_handler.h"

PointHandler::PointHandler(int frequency_update_rate) :
	frequency_update_rate_{ frequency_update_rate }
{

}

void PointHandler::AddPosition(Point2f position)
{
	positions_.push_back(position);
}

void PointHandler::AddFrequency(double frequency)
{
	frequencies_.push_back(frequency);
}

void PointHandler::AddFrameTimePos(double frame_time)
{
	frame_time_positions_.push_back(frame_time);
}
