#include "VVT-V2/vibrating_point.h"

void VibratingPoint::UpdateExtremumAmplitude(double extremum_amplitude)
{

}

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

	Scalar mean_coordinates_of_point_ = mean(point_coordinates_);
	//std::cout << mean_coordinates_of_point_ << std::endl;
	double meaned_x = mean_coordinates_of_point_.val[0];
	double meaned_y = mean_coordinates_of_point_.val[1];

	vec_meaned_coordinates_of_point_.clear();
	x_coordinates_.clear();
	y_coordinates_.clear();
	for (int i = 0; i < point_coordinates_.size(); i++)
	{
		vec_meaned_coordinates_of_point_.push_back(Point2f(point_coordinates_[i].x - meaned_x, point_coordinates_[i].y - meaned_y));
		x_coordinates_.push_back(point_coordinates_[i].x - meaned_x);
		y_coordinates_.push_back(point_coordinates_[i].y - meaned_y);
	}

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

	HelperFunctions::DeadzoneFilter(frequencies);
	HelperFunctions::DeadzoneFilter(magnitudes);

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

	bool candidate_for_being_full_zero = false;
	int zero_flag = 0;

	if (!absolute_peak)
	{
		// searching peaks in output vector of magnitudes
		
		PeakFinder::findPeaks(magnitudes, indexes_of_peak_frequencies, false, 1);

		int maxIdx = 0;

		zero_flag = indexes_of_peak_frequencies.size();

		double mag_sum = 0;

		// filling in vector of peak_frequencies with the just found peak frequencies 
		for (int i = 0; i < indexes_of_peak_frequencies.size(); i++)
		{
			peak_frequencies.push_back(frequencies[indexes_of_peak_frequencies[i]]);

			//std::cout << "mag: " << magnitudes[indexes_of_peak_frequencies[i]] << std::endl;
			mag_sum += magnitudes[indexes_of_peak_frequencies[i]];
			if ((magnitudes[indexes_of_peak_frequencies[i]]) < 0.01f)
				zero_flag--;
		}

		main_frequency_ = frequencies[HelperFunctions::FindGlobalMaxIdx(magnitudes)];
		float max_diff = HelperFunctions::CalculateMaxDifferenceInVector(magnitudes);
		mag_max_differences_.push_back(max_diff);
		float mean_diff_of_max_diffs = 0;
		if (mag_max_differences_.size() > 1)
			mean_diff_of_max_diffs = HelperFunctions::CalculateMeanDifferenceInVector(mag_max_differences_);

		if (mean_diff_of_max_diffs < sensivity_ || std::isnan(mean_diff_of_max_diffs))
			confidence_level_ -= 0.01;
		else
			confidence_level_ += 0.01;
		
		if (zero_flag == 0 && indexes_of_peak_frequencies.size() != 0)
		{
			frequencies_.clear();
			frequencies_.push_back(0.0);
		}
		else
		{
			frequencies_ = peak_frequencies;
		}
	}

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

void VibratingPoint::SetSensivity(double sensivity)
{
	sensivity_ = sensivity;
}

void VibratingPoint::CalculateAmplitude()
{
	///
	/// for amplitude calculation
	///
	float max_meaned_x = 0;
	float max_meaned_y = 0;

	// Создаем контей

	max_meaned_x = vec_meaned_coordinates_of_point_[HelperFunctions::FindGlobalMaxIdx(x_coordinates_)].x;
	max_meaned_y = vec_meaned_coordinates_of_point_[HelperFunctions::FindGlobalMaxIdx(y_coordinates_)].y;

	float relative_amplitude_x = 0.0f;
	float relative_amplitude_y = 0.0f;

	Scalar mean_of_mean_coordinates_of_point = mean(vec_meaned_coordinates_of_point_);
	double meaned_mean_x = mean_of_mean_coordinates_of_point.val[0];
	double meaned_mean_y = mean_of_mean_coordinates_of_point.val[1];

	///
	if (point_coordinates_.size() > 2)
	{
		relative_amplitude_x = (max_meaned_x) / (max_meaned_x + max_meaned_y);
		relative_amplitude_y = (max_meaned_y) / (max_meaned_x + max_meaned_y);
	}

	/*
	// FOR DEBUG PURPOSES
	if (std::isnan(relative_amplitude_x) || std::isnan(relative_amplitude_y))
	{
		std::cout << "!DEBUG first inf (x): " << relative_amplitude_x << " second inf (y): " << relative_amplitude_y << std::endl;
		std::cout << "!DEBUG max_meaned_x: " << vec_meaned_coordinates_of_point_[HelperFunctions::FindGlobalMaxIdx(x_coordinates)].x << " max_meaned_y: " << vec_meaned_coordinates_of_point_[HelperFunctions::FindGlobalMaxIdx(y_coordinates)].y << std::endl;
		std::cout << "vec size: " << point_coordinates_.size() << std::endl;
		for (int i = 0; i < point_coordinates_.size(); i++)
		{
			std::cout << i << "-th coords: " << point_coordinates_[i] << std::endl;
		}
	}

	if ((relative_amplitude_x > 1) || (relative_amplitude_y > 1))
	{
		std::cout << "!DEBUG first big (x): " << relative_amplitude_x << " second big (y): " << relative_amplitude_y << std::endl;
		std::cout << "!DEBUG max_meaned_x: " << max_meaned_x << " max_meaned_y: " << max_meaned_y << std::endl;
	}
	*/

	if (!(std::isnan(relative_amplitude_x) || std::isnan(relative_amplitude_y)))
	{
		relative_amplitude_.x = relative_amplitude_x;
		relative_amplitude_.y = relative_amplitude_y;
		current_amplitude_.x = max_meaned_x - meaned_mean_x;
		current_amplitude_.y = max_meaned_y - meaned_mean_y;
	}
}

Point2f VibratingPoint::GetLastFoundCoordinates()
{
	return point_coordinates_.back();
}

std::vector<double> VibratingPoint::GetCurrentVibrationFrequency()
{
	return frequencies_;
}

double VibratingPoint::GetCurrentMainFrequency()
{
	return main_frequency_;
}

Point3f VibratingPoint::GetRelativeAmplitude()
{
	return relative_amplitude_;
}

Point3f VibratingPoint::GetCurrentAmplitude()
{
	return current_amplitude_;
}

double VibratingPoint::GetCurrentConfidenceLevel()
{
	return confidence_level_;
}