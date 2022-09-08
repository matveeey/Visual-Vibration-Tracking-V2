#include <VVT-V2/contour_handler.h>

ContourHandler::ContourHandler(Mat frame, Rect* ROI) :
	minimal_distance_{ 5.0f }
{
	Mat prep_frame;
	std::vector<Vec4i> hierarchy = { 0 };

	// �������� �� ��, ��� �� ������ ROI ��� ���
	if (ROI != nullptr)
		prep_frame = Mat(frame, *ROI);
	else
		frame.copyTo(prep_frame);

	// �������� �� ���������
	if (frame.channels() > 2)
		cvtColor(prep_frame, prep_frame, COLOR_BGR2GRAY);

	// ����� ����������
	GaussianBlur(prep_frame, prep_frame, Size(7, 7), 0);
	threshold(prep_frame, prep_frame, 80, 255, cv::THRESH_BINARY);
	Canny(prep_frame, prep_frame, 64, 192);
	Canny(prep_frame, prep_frame, 64, 192);

	// ������� ������� � �������������, ���� ROI �� ������
	// ����������� ���������� �� ��������� "ROI-�����" � ���������� ��������� ����������� (������ contour_shapes_[i][j] += ROI.tl())
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

	// ���������� �� ������� �������� (std::vector<std::vector>>) � ��������� �� ������������ ������� ����
	auto extern_itt = contour_shapes_.begin();
	while (extern_itt != contour_shapes_.end())
	{
		std::vector<Point> shape = *extern_itt;

		auto intern_itt = shape.begin();
		while (intern_itt != shape.end())
		{
			// �� ����� �������: ����������� ��������� ����� ������� + ������ �������
			if (CalculateDistance(*intern_itt, *(intern_itt + 1)) > minimal_distance_ && (contourArea(*extern_itt) > 100))
			{
				continous_contour_shapes.push_back(*intern_itt);
			}
			intern_itt++;
		}

		extern_itt++;
	}

	// ������������ ����� ��������
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