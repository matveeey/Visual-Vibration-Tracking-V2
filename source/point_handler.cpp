#include "VVT-V2/point_handler.h"

PointHandler::PointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient) :
	Histogram{ 600, 300, sampling_rate / 2, point_id },
	update_rate_{ update_rate },
	interaction_offset_{ 50 },
	sampling_rate_{ sampling_rate },
	point_id_{ point_id },
	// TEMPORARY
	resizing_coefficient_{ 1.25f / resizing_coefficient }
{
	// ƒобавл€ем первые координаты клика мышью
	point_coordinates_.push_back(init_coordinates);
	interaction_box_ = Rect(
		Point2i(init_coordinates.x - interaction_offset_, init_coordinates.y - interaction_offset_),
		Point2i(init_coordinates.x + interaction_offset_, init_coordinates.y + interaction_offset_)
	);

	frequencies_.push_back(0.0);
	amplitude_ = Point3f(0.0, 0.0, 0.0);
}

PointHandler::~PointHandler()
{
	std::cout << "PointHandler destructor" << std::endl;
}

void PointHandler::ExecuteFft()
{
	bool absolute_peak = false;

	std::vector<Point2f> fft_result;
	std::vector<Point2f> p2;
	std::vector<Point2f> p1;
	std::vector<double> frequencies;
	std::vector<int> indexes_of_peak_frequencies;
	std::vector<float> magnitudes;

	std::vector<double> peak_frequencies;

	int samples_amount = point_coordinates_.size();

	// subtracting mean value so the samples would be fluctuating around zero
	std::vector<cv::Point2f> vec_meaned_coordinates_of_point_;

	Scalar mean_coordinates_of_point_ = mean(point_coordinates_);
	//std::cout << mean_coordinates_of_point_ << std::endl;
	double meaned_x = mean_coordinates_of_point_.val[0];
	double meaned_y = mean_coordinates_of_point_.val[1];

	///
	/// for amplitude calculation
	///
	float max_x = 0;
	float min_x = 5000;
	float max_y = 0;
	float min_y = 5000;

	for (int i = 0; i < point_coordinates_.size(); i++)
	{
		///
		if (point_coordinates_[i].x > max_x)
			max_x = point_coordinates_[i].x;
		if (point_coordinates_[i].x < min_x)
			min_x = point_coordinates_[i].x;
		if (point_coordinates_[i].y > max_y)
			max_y = point_coordinates_[i].y;
		if (point_coordinates_[i].y < min_x)
			min_y = point_coordinates_[i].y;
		///


		vec_meaned_coordinates_of_point_.push_back(Point2f(point_coordinates_[i].x - meaned_x, point_coordinates_[i].y - meaned_y));
		//std::cout << vec_meaned_coordinates_of_point_[i] << std::endl;
	}

	///
	float amplitude_in_percents_x = (max_x - meaned_x) / (max_x + max_y - meaned_x - meaned_y);//sqrt((max_x / mean_value.val[0]) * (max_x / mean_value.val[0]) + (1 - min_x / mean_value.val[0]) * (1 - min_x / mean_value.val[0]));
	float amplitude_in_percents_y = (max_y - meaned_y) / (max_x + max_y - meaned_x - meaned_y);//sqrt((max_y / mean_value.val[1]) * (max_y / mean_value.val[1]) + (1 - min_y / mean_value.val[1]) * (1 - min_y / mean_value.val[1]));
	amplitude_.x = amplitude_in_percents_x;
	amplitude_.y = amplitude_in_percents_y;
	///

	dft(vec_meaned_coordinates_of_point_, fft_result);

	// computing two-sided spectrum P2
	for (int i = 0; i < fft_result.size(); i++)
	{
		Point2f tmp;
		tmp.x = abs(fft_result[i].x / samples_amount);
		tmp.y = abs(fft_result[i].y / samples_amount);
		p2.push_back(tmp);
	}

	// computing single-sided spectrum P1 based on P2 and the even-valued signal length (the same as size_of_vecs_)
	for (int i = 0; i < p2.size(); i++)
	{
		size_t idx = 0;
		idx = (int)(((double)(i)) / 2.0 + 1.0);
		p1.push_back(p2[idx]);
	}

	// possible frequencies
	for (int i = 0; i < samples_amount; i++)
	{
		float tmp = sampling_rate_ * ((double)(i) / 2) / samples_amount;
		frequencies.push_back(tmp);
	}

	for (int i = 0; i < p1.size(); i++)
	{
		float current_magnitude = sqrt(p1[i].x * p1[i].x + p1[i].y * p1[i].y);
		magnitudes.push_back(current_magnitude);
	}

	//Filter(magnitudes);

	/////////////////////////////////////

	if (point_coordinates_.size() > 5)
	{
		std::fstream file;
		file.open("C:/Users/seeyo/source/repos/Visual-Vibration-Tracking-V2/docs/magnitudes.txt", 'w');

		/*for (int i = 0; i < magnitudes.size(); i++)
		{
			std::string tmp = std::to_string(frequencies[i]) + " " + std::to_string(magnitudes[i]) + " " + std::to_string(p1[i].x) + " " + std::to_string(p1[i].y);
			file << tmp << std::endl;
		}*/

		for (int i = 0; i < point_coordinates_.size(); i++)
		{
			std::string tmp = std::to_string(point_coordinates_[i].x) + " " + std::to_string(point_coordinates_[i].y) + " " + std::to_string(point_time_coordinates_[i]);
			file << tmp << std::endl;
		}

		file.close();
	}

	/////////////////////////////////////

	x_ = frequencies;
	y_ = magnitudes;

	// ¬ызов отрисовщика гистограммы
	DrawHistogram();

	if (!absolute_peak)
	{
		// searching peaks in output vector of magnitudes
		PeakFinder::findPeaks(magnitudes, indexes_of_peak_frequencies, false, 1);

		// this function works strange, it doesn't always returns the indexes
		// so i decided to do the following
		int maxIdx = 0;
		if (indexes_of_peak_frequencies.empty())
		{
			//std::cout << "alternative way" << std::endl;
			double tmp_max_value = 0.0;
			// here we start from 1 and not 0
			// it's called "lyutiy kostyl"
			for (int i = 1; i < magnitudes.size(); i++)
			{
				if (magnitudes[i] > tmp_max_value)
				{
					tmp_max_value = magnitudes[i];
					maxIdx = i;
				}

			}
			peak_frequencies.push_back(frequencies[maxIdx]);
		}


		// filling in vector of peak_frequencies with the just found peak frequencies 
		for (int i = 0; i < indexes_of_peak_frequencies.size(); i++)
		{
			peak_frequencies.push_back(frequencies[indexes_of_peak_frequencies[i]]);

			//std::cout << "Max is " << frequencies[indexes_of_peak_frequencies[i]] << std::endl;
		}
	}
	// DEBUG
	/*std::cout << "frequencies amount: " << indexes_of_peak_frequencies.size() << std::endl;*/
	if (absolute_peak)
	{
		int maxIdx = 0;
		double tmp_max_value = 0.0;
		// here we start from 1 and not 0
		// it's called "lyutiy kostyl"
		for (int i = 1; i < magnitudes.size(); i++)
		{
			if (magnitudes[i] > tmp_max_value)
			{
				tmp_max_value = magnitudes[i];
				maxIdx = i;
			}

		}
		peak_frequencies.push_back(frequencies[maxIdx]);
	}

	frequencies_ = peak_frequencies;
}

void PointHandler::Filter(std::vector<float>& magnitudes)
{
	float cutoff_freq_limit = 0.1f;
	for (int i = 0; i < static_cast<int>(magnitudes.size() * cutoff_freq_limit); i++)
	{
		float coeff = 0.1f / (1.0f / magnitudes[i]);
		magnitudes[i] = magnitudes[i] * coeff;
	}
}

Point2f PointHandler::GetLastFoundCoordinates()
{
	return point_coordinates_.back();
}

std::vector<double> PointHandler::GetCurrentVibrationFrequency()
{
	return frequencies_;
}

Point3f PointHandler::GetCurrentAmplitude()
{
	return amplitude_;
}

Point2f PointHandler::GetTextCoordinates()
{
	if (!point_coordinates_.empty())
		return point_coordinates_[0];
	else return Point2f(0.0, 0.0);
}

Rect PointHandler::GetInteractionRect()
{
	return interaction_box_;
}

void PointHandler::UpdatePointColor()
{
	if (interacted_)
		point_color_ = Scalar(255, 150, 50); // синий светлее
	else
		point_color_ = Scalar(255, 75, 25); // синий темнее
}

bool PointHandler::IsInteracted(Point2i coordinates)
{
	interacted_ = interaction_box_.contains(coordinates);
	return interacted_;
}

void PointHandler::AddNewCoordinate(Point2f position)
{
	// ќбновл€ем пр€моугольник взаимодействи€
	interaction_box_ = Rect(
		Point2i(position.x - interaction_offset_, position.y - interaction_offset_),
		Point2i(position.x + interaction_offset_, position.y + interaction_offset_)
	);

	point_coordinates_.push_back(position);
}

void PointHandler::AddFrameTimePos(double frame_time)
{
	point_time_coordinates_.push_back(frame_time);
}

void PointHandler::Draw(Mat& frame)
{
	// отрисовываем пр€моугольник взаимодействи€ (если оно есть)
	DrawInteractionRectangle(frame);
	// отрисовываем линии точек
	DrawPointTrack(frame);
	// отрисовываем круг вокруг точки
	DrawPoint(frame);
	// отрисовываем гистограмму
	DrawHistogram();
	// отрисовываем частоту вибрации
	DrawTextData(frame);
}

void PointHandler::DrawHistogram()
{
	if (is_histogram_plotted_)
	{
		SetXValues(x_);
		SetYValues(y_);
		ShowHistogram();
	}
}

void PointHandler::DrawPoint(Mat& frame)
{
	UpdatePointColor();
	// отрисовываем круг вокруг точки
	circle(frame, point_coordinates_.back(), 10, point_color_, 2);

}

void PointHandler::DrawPointTrack(Mat& frame)
{
	if (point_coordinates_.size() > 2)
		line(frame, point_coordinates_[point_coordinates_.size() - 2], point_coordinates_[point_coordinates_.size() - 1], Scalar(0, 255, 0), 1, LINE_AA);
}

void PointHandler::DrawInteractionRectangle(Mat& frame)
{
	if (interacted_)
		rectangle(frame, interaction_box_, Scalar(0, 255, 0), 2);
}

void PointHandler::DrawTextData(Mat& frame)
{
	// отрисовываем частоту вибрации
	for (int j = 0; j < frequencies_.size(); j++)
	{
		putText(
			frame,
			"hz: " + std::to_string(frequencies_[j]),
			Point(point_coordinates_.front().x + 15, point_coordinates_.front().y + resizing_coefficient_ * j * 20),
			FONT_HERSHEY_PLAIN,
			resizing_coefficient_ * 1.25,
			Scalar(0, 69, 255),
			2
		);
	}

	// отрисовываем амплитуды вибрации
	putText(
		frame,
		"x: " + std::to_string(amplitude_.x),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - resizing_coefficient_ * 20),
		FONT_HERSHEY_PLAIN,
		resizing_coefficient_ * 1,
		Scalar(0, 255, 255),
		2
	);
	putText(
		frame,
		"y: " + std::to_string(amplitude_.y),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - resizing_coefficient_ * 2 * 20),
		FONT_HERSHEY_PLAIN,
		resizing_coefficient_ * 1,
		Scalar(0, 255, 255),
		2
	);
	// uncomment когда по€витс€ 3-€ координата дл€ амплитуды :)
	/*putText(
		frame,
		"z: " + std::to_string(amplitude_.z),
		Point(point_coordinates_.front().x, point_coordinates_.front().y - res_mp_ * 3 * 20),
		FONT_HERSHEY_PLAIN,
		res_mp_ * 1,
		Scalar(0, 255, 255),
		2
	);*/
}


std::vector<double> PointHandler::GetX()
{
	return x_;
}

std::vector<float> PointHandler::GetY()
{
	return y_;
}
