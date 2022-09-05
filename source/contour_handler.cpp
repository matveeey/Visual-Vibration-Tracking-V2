#include <VVT-V2/contour_handler.h>

ContourHandler::ContourHandler(Mat frame, Rect ROI) :
	minimal_distance_{ 5.0f }
{
	std::vector<Vec4i> hierarchy = { 0 };
	Mat prep_frame(frame, ROI);
	
	if (frame.channels() > 2)
		cvtColor(prep_frame, prep_frame, COLOR_BGR2GRAY);

	GaussianBlur(prep_frame, prep_frame, Size(7, 7), 0);
	threshold(prep_frame, prep_frame, 80, 255, cv::THRESH_BINARY);
	Canny(prep_frame, prep_frame, 64, 192);
	Canny(prep_frame, prep_frame, 64, 192);

	findContours(prep_frame, contour_shapes_, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, ROI.tl());

	/*int threshold1 = 1, threshold2 = 255;
	int size_dim = 3;
	int canny_threshold1 = 64, canny_threshold2 = 192;
	namedWindow("trackbars");
	createTrackbar("threshold1", "trackbars", &threshold1, 255, NULL);
	createTrackbar("threshold2", "trackbars", &threshold2, 255, NULL);
	createTrackbar("blur size", "trackbars", &size_dim, 10, NULL);
	createTrackbar("canny1", "trackbars", &canny_threshold1, 255, NULL);
	createTrackbar("canny2", "trackbars", &canny_threshold2, 255, NULL);

	while (true)
	{
		Mat prep_frame(frame, ROI);
		cvtColor(prep_frame, prep_frame, COLOR_BGR2GRAY);
		Size ksize = Size(size_dim, size_dim);
		GaussianBlur(prep_frame, prep_frame, ksize, 0);
		threshold(prep_frame, prep_frame, threshold1, threshold2, cv::THRESH_BINARY);
		GaussianBlur(prep_frame, prep_frame, ksize, 0);
		Canny(prep_frame, prep_frame, canny_threshold1, canny_threshold2);

		findContours(prep_frame, contour_shapes_, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, ROI.tl());

		for (int i = 0; i < contour_shapes_.size(); i++)
		{
			drawContours(prep_frame, contour_shapes_, i, Scalar(0, 0, 255), 2);
		}
		imshow("After canny", prep_frame);
		waitKey(1000);
	}*/
	
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

	// Проходимся по вектороу контуров (вектор векторов) и проверяем на соответствие условию ниже
	auto extern_itt = contour_shapes_.begin();
	while (extern_itt != contour_shapes_.end())
	{
		std::vector<Point> shape = *extern_itt;

		auto intern_itt = shape.begin();
		while (intern_itt != shape.end())
		{
			if (CalculateDistance(*intern_itt, *(intern_itt + 1)) > minimal_distance_ && (contourArea(*extern_itt) > 100)) // То самое условие
			{
				continous_contour_shapes.push_back(*intern_itt);
			}
			intern_itt++;
		}

		extern_itt++;
	}

	// Ограничиваем длину контуров
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