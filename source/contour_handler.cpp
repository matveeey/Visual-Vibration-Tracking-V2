#include <VVT-V2/contour_handler.h>

ContourHandler::ContourHandler(Mat frame, Rect ROI)
{
	std::vector<Vec4i> hierarchy = { 0 };
	Mat prep_frame(frame, ROI);
	cvtColor(prep_frame, prep_frame, COLOR_BGR2GRAY);
	GaussianBlur(prep_frame, prep_frame, Size(7, 7), 0);
	Canny(prep_frame, prep_frame, 64, 192);

	findContours(prep_frame, contour_shapes_, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	contour_shapes_ = TranslateCoords(ROI.tl(), contour_shapes_);
}

void ContourHandler::DrawContours(Mat& frame)
{
	for (int i = 0; i < contour_shapes_.size(); i++)
	{
		drawContours(frame, contour_shapes_, i, Scalar(255, 0, 0), 6);
	}
}

std::vector<std::vector<Point2i>> ContourHandler::GetContours()
{
	return contour_shapes_;
}

std::vector<Point2f> ContourHandler::GetContinousContours()
{
	std::vector<Point2f> continous_contour_shapes;

	for (int i = 0; i < contour_shapes_.size(); i++)
	{
		for (int j = 0; j < contour_shapes_[i].size(); j++)
		{
			continous_contour_shapes.push_back(contour_shapes_[i][j]);
		}
	}

	return continous_contour_shapes;
}

std::vector<std::vector<Point2i>> ContourHandler::TranslateCoords(Point2i offset, std::vector<std::vector<Point2i>> contour_shapes)
{
	for (int i = 0; i < contour_shapes.size(); i++)
	{
		for (int j = 0; j < contour_shapes[i].size(); j++)
		{
			contour_shapes[i][j].x += offset.x;
			contour_shapes[i][j].y += offset.y;
		}
	}
	return contour_shapes;
}
