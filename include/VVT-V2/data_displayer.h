#ifndef DATA_DISPLAYER_H
#define DATA_DISPLAYER_H

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

class DataDisplayer
{
public:
	void OutputVibrationParameters(Mat& frame, Point2f point, int res_mp, Point2f amplitude);

	void SetVectorOfFrequencies(std::vector<float> vec_of_frequencies);
private:
	std::vector<float> vec_of_frequencies_;
};

#endif