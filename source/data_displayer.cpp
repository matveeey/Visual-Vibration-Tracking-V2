#include "VVT-V2\data_displayer.h"

DataDisplayer::DataDisplayer(std::vector<PointHandler> &vec_point_handlers, std::vector<Point2f> previous_points_coordinates, Point2i last_mouse_position)
	:vec_point_handlers_{ vec_point_handlers },
	previous_points_coordinates_{ previous_points_coordinates },
	last_mouse_position_{ last_mouse_position },
	mode_{ -1 }
{
	for (int i = 0; i < points_.size(); i++)
	{
		colors_.push_back(Scalar(0, 0, 255));
	}
}

void DataDisplayer::DrawData(Mat& frame)
{
	if (previous_points_coordinates_.size() != vec_point_handlers_.size())
	{
		std::cout << "DEBUG: ERROR - previous_points_coordinates_ size doesnt match vec_point_handlers_ size" << std::endl;
		return;
	}
	double res_mp_ = 1.5;
	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		Scalar circle_color;
		Point2f new_point = vec_point_handlers_[i].GetLastFoundCoordinates();
		Point2f text_coordinates = vec_point_handlers_[i].GetTextCoordinates();
		Point3f amplitude = vec_point_handlers_[i].GetCurrentAmplitude();
		if (vec_point_handlers_[i].IsInteracted(last_mouse_position_))
		{
			circle_color = Scalar(255, 150, 50);
		}
		else circle_color = Scalar(255, 75, 25);

		// отрисовываем линии точек
		line(frame, previous_points_coordinates_[i], new_point, Scalar(0, 255, 0), 1, LINE_AA);

		// отрисовываем круг вокруг точки
		circle(frame, new_point, 10, circle_color, 2);

		// отрисовываем частоту вибрации
		putText(
			frame,
			"hz: " + std::to_string(vec_point_handlers_[i].GetCurrentVibrationFrequency()),
			Point(text_coordinates.x + 15, text_coordinates.y + res_mp_ * i * 20 + 25),
			FONT_HERSHEY_PLAIN,
			res_mp_ * 1.25,
			Scalar(0, 69, 255),
			2
		);

		// отрисовываем амплитуды вибрации
		putText(
			frame,
			"x: " + std::to_string(amplitude.x),
			Point(text_coordinates.x, text_coordinates.y - res_mp_ * 20),
			FONT_HERSHEY_PLAIN,
			res_mp_ * 1,
			Scalar(0, 255, 255),
			2
		);
		putText(
			frame,
			"y: " + std::to_string(amplitude.y),
			Point(text_coordinates.x, text_coordinates.y - res_mp_ * 2 * 20),
			FONT_HERSHEY_PLAIN,
			res_mp_ * 1,
			Scalar(0, 255, 255),
			2
		);
		putText(
			frame,
			"z: " + std::to_string(amplitude.z),
			Point(text_coordinates.x, text_coordinates.y - res_mp_ * 3 * 20),
			FONT_HERSHEY_PLAIN,
			res_mp_ * 1,
			Scalar(0, 255, 255),
			2
		);
	}
}

Mat DataDisplayer::AddTipText(Mat frame)
{
	std::vector<std::string> tip_text = { "SPACE pause", "C calib mode for amplitude", "R set ROI", "Q quit" };
	Point local_offset = Point(0, 20);
	Point top_left_offset = Point(5, local_offset.y);
	switch (mode_)
	{
	case -1:
	{
		putText(/*frame_*/frame, tip_text[0], top_left_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		putText(/*frame_*/frame, tip_text[1], top_left_offset + local_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		putText(/*frame_*/frame, tip_text[2], top_left_offset + local_offset * 2, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		putText(/*frame_*/frame, tip_text[3], top_left_offset + local_offset * 3, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 0:
	{

		putText(/*frame_*/frame, tip_text[0], top_left_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 1:
	{
		putText(/*frame_*/frame, tip_text[1], top_left_offset + local_offset, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 2:
	{
		putText(/*frame_*/frame, tip_text[2], top_left_offset + local_offset * 2, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	case 3:
	{
		putText(/*frame_*/frame, tip_text[3], top_left_offset + local_offset * 3, FONT_HERSHEY_PLAIN, 1, Scalar(255, 100, 100), 1, 8, false);
		break;
	}
	}
	return frame;
}

void DataDisplayer::ProcessFrame(Mat tmp_frame)
{
	// "le kostyl", going to be fixed later
	tmp_frame_ = tmp_frame;
	//ClearFrame();

	// drawing ROI and updating colors
	//UpdateDisplayingRectangle();
	UpdateColors();
	//std::cout << colors_.size() << std::endl;

	for (int i = 0; i < points_.size(); i++)
	{
		// drawing points with their own colors - frequencies
		circle(/*frame_*/tmp_frame_, points_[i], 1, colors_[i], FILLED);
	}

	AddGradient();
	tmp_frame_ = AddTipText(tmp_frame_);
}

Mat DataDisplayer::GetFrame()
{
	return /*frame_*/tmp_frame_;
}

void DataDisplayer::SetRoi(Rect roi)
{
	roi_ = roi;
}

void DataDisplayer::UpdateFrequencies(std::vector<float> frequencies, double range)
{
	frequencies_ = frequencies;
	range_ = range;
}

void DataDisplayer::UpdateDisplayingPoints(std::vector<Point2f> points)
{
	points_ = points;
}

void DataDisplayer::SetMode(int mode)
{
	mode_ = mode;
}

void DataDisplayer::UpdateDisplayingRectangle()
{
	rectangle(frame_, roi_.tl(), roi_.br(), Scalar(0, 255, 0), 1);
}

void DataDisplayer::UpdateColors()
{
	// translate frequency [0; range_] to [0; 255] int color value;
	if (!frequencies_.empty())
	{
		//std::cout << "UPDATING COLORS" << std::endl;
		//std::cout << frequencies_.size() << std::endl;
		//std::cout << points_.size() << std::endl;
		
		colors_.clear();
		for (int i = 0; i < points_.size(); i++)
		{
			std::vector<int> color = Rgb(frequencies_[i] / range_);
			colors_.push_back(Scalar(color[0], color[1], color[2]));
		}
	}
	else
	{
		for (int i = 0; i < points_.size(); i++)
		{
			colors_.push_back(Scalar(0, 0, 0));
		}
	}
}

void DataDisplayer::AddGradient()
{
	// эта штука захардкожена, сюда не надо смотреть((
	// потом нормально сделаю

	Point top_left = Point(15, frame_.rows - 150);
	Point bottom_right = Point(frame_.cols - 15, frame_.rows - 50);
	Rect gradient_bounds(top_left, bottom_right);
	rectangle(/*frame_*/tmp_frame_, gradient_bounds.tl(), gradient_bounds.br(), Scalar(0, 255, 0), 1);

	Point position;
	position = Point(top_left.x - 2, bottom_right.y + 15);

	for (int i = 0; i <= 6; i++)
	{
		putText(/*frame_*/tmp_frame_, std::to_string(i * 3), position, FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);
		position = Point(position.x + (frame_.cols - 30)/5 - 1, bottom_right.y + 15);
	}

	int rect_width = bottom_right.x - top_left.x;
	int rect_height = top_left.y - bottom_right.y;

	int slices = 32;
	int dx = rect_width / slices;
	Point tl = gradient_bounds.tl();
	Point br = Point(tl.x + dx, bottom_right.y);

	for (int i = 0; i < slices; i++)
	{
		Scalar scalar_color;
		std::vector<int> vec_color = Rgb((double)(i) / (double)(slices));
		//std::cout << vec_color[0] << std::endl;

		scalar_color[0] = vec_color[0];
		scalar_color[1] = vec_color[1];
		scalar_color[2] = vec_color[2];

		rectangle(/*frame_*/tmp_frame_, tl, br, scalar_color, FILLED);

		tl = Point(tl.x + dx, tl.y);
		br = Point(br.x + dx, br.y);

	}
}

// not mine, taken from stackoverflow
std::vector<int> DataDisplayer::Rgb(double ratio)
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
	case 0: r = 255; g = 0;   b = 0;   g += x; break;
	case 1: r = 255; g = 255; b = 0;   r -= x; break;
	case 2: r = 0;   g = 255; b = 0;   b += x; break;
	case 3: r = 0;   g = 255; b = 255; g -= x; break;
	//case 4: r = 0;   g = 0;   b = 255; r += x; break;
	//case 5: r = 255; g = 0;   b = 255; b -= x; break;
	}
	return { r, g, b };
}

//void VibrationDisplayer::ContourHandler(std::vector<std::vector<Point>> contour_shapes)
//{
//	std::vector<Point> contour_points;
//	// рассматриваем каждый найденный контур
//	for (int i = 0; i < contour_shapes.size(); i++)
//	{
//		
//	}
//	// imshow
//}
//
//void VibrationDisplayer::GetContourHeadPoints(std::vector<std::vector<Point>> contour_shapes, std::vector<Point>& contour_head_points)
//{
//	// рассматриваем каждый найденный контур
//	for (int i = 0; i < contour_shapes.size(); i++)
//	{
//		std::cout << "cum" << std::endl;
//		if (!contour_shapes[i].empty())
//		{
//			std::cout << "cum" << std::endl;
//			contour_head_points.push_back(contour_shapes[i][0]);
//		}
//	}
//}


