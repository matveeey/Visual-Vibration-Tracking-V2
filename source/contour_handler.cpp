#include <VVT-V2/contour_handler.h>

ContourHandler::ContourHandler(Mat frame, Rect* ROI) :
	minimal_distance_{ 5.0f }
{
	Mat prep_frame;
	std::vector<Vec4i> hierarchy = { 0 };

	// ѕроверка на то, был ли послан ROI или нет
	if (ROI != nullptr)
		prep_frame = Mat(frame, *ROI);
	else
		frame.copyTo(prep_frame);

	// ѕроверка на грейскейл
	if (frame.channels() > 2)
		cvtColor(prep_frame, prep_frame, COLOR_BGR2GRAY);

	// »маге процессинг
	GaussianBlur(prep_frame, prep_frame, Size(7, 7), 0);
	threshold(prep_frame, prep_frame, 80, 255, cv::THRESH_BINARY);
	Canny(prep_frame, prep_frame, 64, 192);
	Canny(prep_frame, prep_frame, 64, 192);

	// Ќаходим контуры и дополнительно, если ROI не пустой
	// транслируем координаты из координат "ROI-куска" в координаты исходного изображени€ (аналог contour_shapes_[i][j] += ROI.tl())
	if (ROI != nullptr)
		findContours(prep_frame, contour_shapes_, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, ROI->tl());
	else
		findContours(prep_frame, contour_shapes_, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
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

	// ѕроходимс€ по вектору контуров (std::vector<std::vector>>) и провер€ем на соответствие условию ниже
	auto extern_itt = contour_shapes_.begin();
	while (extern_itt != contour_shapes_.end())
	{
		std::vector<Point> shape = *extern_itt;

		auto intern_itt = shape.begin();
		while (intern_itt != shape.end())
		{
			// “о самое условие: минимальна€ дистанци€ между точками + размер контура
			if (CalculateDistance(*intern_itt, *(intern_itt + 1)) > minimal_distance_ && (contourArea(*extern_itt) > 100))
			{
				continous_contour_shapes.push_back(*intern_itt);
			}
			intern_itt++;
		}

		extern_itt++;
	}

	// ќграничиваем длину контуров
	while (continous_contour_shapes.size() > 500)
		continous_contour_shapes = MakeTwiceThinner(continous_contour_shapes);

	return continous_contour_shapes;
}

template<typename T>
std::vector<T> ContourHandler::MakeTwiceThinner(std::vector<T> src)
{
	std::vector<T> dst;

	auto it = src.begin();
	while (it != src.end() - 1)
	{
		if ((std::distance(src.begin(), it)) % 2 == 0)
			dst.push_back(*(it));
		it++;
	}

	return dst;
}

template<typename T>
float ContourHandler::CalculateDistance(T point1, T point2)
{
	return sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}