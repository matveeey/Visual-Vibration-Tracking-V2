#include "VVT-V2/vibrating_point.h"

void VibratingPoint::ExecuteFFT()
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

	// Вызов отрисовщика гистограммы
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

void VibratingPoint::AddNewPointPosition(Point2f position)
{
	// Обновляем прямоугольник взаимодействия
	interaction_box_ = Rect(
		Point2i(position.x - interaction_offset_, position.y - interaction_offset_),
		Point2i(position.x + interaction_offset_, position.y + interaction_offset_)
	);

	point_coordinates_.push_back(position);
}

void VibratingPoint::AddNewPointTime(double time)
{
	point_time_coordinates_.push_back(time);
}

bool VibratingPoint::IsInteracted(Point2i coordinates)
{
	interacted_ = interaction_box_.contains(coordinates);
	return interacted_;
}

Point2f VibratingPoint::GetLastFoundCoordinates()
{
	return point_coordinates_.back();
}

std::vector<double> VibratingPoint::GetCurrentVibrationFrequency()
{
	return frequencies_;
}

Point3f VibratingPoint::GetCurrentAmplitude()
{
	return amplitude_;
}
