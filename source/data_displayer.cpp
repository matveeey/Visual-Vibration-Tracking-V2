#include "VVT-V2\data_displayer.h"

void DataDisplayer::OutputVibrationParameters(Mat& frame, Point2f point, int res_mp)
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

}

void DataDisplayer::SetVectorOfFrequencies(std::vector<float> vec_of_frequencies)
{
	vec_of_frequencies_ = vec_of_frequencies;
}
