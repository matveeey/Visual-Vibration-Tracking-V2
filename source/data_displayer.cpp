#include "VVT-V2\data_displayer.h"

void DataDisplayer::OutputVibrationParameters(Mat& frame, Point2f point, int res_mp, Point2f amplitude)
{
	for (int j = 0; j < static_cast<int>(vec_of_frequencies_.size()); j++)
	{
		if (!vec_of_frequencies_.empty())
		{
			putText(
				frame,
				"hz: " + std::to_string(vec_of_frequencies_[j]),
				Point(point.x + 15, point.y + res_mp * j * 20 + 25),
				FONT_HERSHEY_PLAIN,
				res_mp * 1.25,
				Scalar(0, 69, 255),
				2
			);
		}
	}
	if (!vec_of_frequencies_.empty())
	{
		putText(
			frame,
			"x: " + std::to_string(amplitude.x),
			Point(point.x, point.y - res_mp * 20),
			FONT_HERSHEY_PLAIN,
			res_mp * 1,
			Scalar(0, 255, 255),
			2
		);
		putText(
			frame,
			"y: " + std::to_string(amplitude.y),
			Point(point.x, point.y - res_mp * 2 * 20),
			FONT_HERSHEY_PLAIN,
			res_mp * 1,
			Scalar(0, 255, 255),
			2
		);
	}

}

void DataDisplayer::SetVectorOfFrequencies(std::vector<float> vec_of_frequencies)
{
	vec_of_frequencies_ = vec_of_frequencies;
}
