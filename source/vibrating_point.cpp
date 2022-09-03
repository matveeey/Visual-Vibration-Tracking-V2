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
	}
	float amplitude_in_percents_x = 0.0f;
	float amplitude_in_percents_y = 0.0f;
	///
	if (point_coordinates_.size() > 1)
	{
		amplitude_in_percents_x = (max_x - meaned_x) / (max_x + max_y - meaned_x - meaned_y);//sqrt((max_x / mean_value.val[0]) * (max_x / mean_value.val[0]) + (1 - min_x / mean_value.val[0]) * (1 - min_x / mean_value.val[0]));
		amplitude_in_percents_y = (max_y - meaned_y) / (max_x + max_y - meaned_x - meaned_y);//sqrt((max_y / mean_value.val[1]) * (max_y / mean_value.val[1]) + (1 - min_y / mean_value.val[1]) * (1 - min_y / mean_value.val[1]));
		//std::cout << "ampl in percs_x: " << amplitude_in_percents_x << std::endl;
		//std::cout << "ampl in percs_y: " << amplitude_in_percents_y << std::endl;
	}
	
	

	if (!(std::isnan(amplitude_in_percents_x) || std::isnan(amplitude_in_percents_y)))
	{
		amplitude_.x = amplitude_in_percents_x;
		amplitude_.y = amplitude_in_percents_y;
		//std::cout << amplitude_ << std::endl;
	}
	else
	{
		//std::cout << "ti debil" << std::endl;
		/*std::cout << "max_x " << max_x << std::endl;
		std::cout << "meaned_x " << meaned_x << std::endl;
		std::cout << "max_y " << max_y << std::endl;
		std::cout << "meaned_y " << meaned_y << std::endl;*/
	}
	
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

	DeadzoneFilter(frequencies);
	DeadzoneFilter(magnitudes);

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

		main_frequency_ = frequencies[FindGlobalMaxIdx(magnitudes)];
		float max_diff = CalculateMaxDifferenceInVector(magnitudes);
		mag_max_differences_.push_back(max_diff);
		float mean_diff_of_max_diffs = 0;
		if (mag_max_differences_.size() > 1)
			mean_diff_of_max_diffs = CalculateMeanDifferenceInVector(mag_max_differences_);

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

template<class T>
void VibratingPoint::DeadzoneFilter(std::vector<T>& input_vector)
{
	float dead_zone_factor = 0.1f;
	for (int i = 0; i < static_cast<int>(input_vector.size() * dead_zone_factor); i++)
	{
		input_vector.erase(std::begin(input_vector));
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

void VibratingPoint::ResetMaxAmplitudeOverall()
{
	max_amplitude_ = 0.0;
}

void VibratingPoint::UpdateMaxAmplitudeOverall(double current_absolute_amplitude)
{
	if ((current_absolute_amplitude > max_amplitude_))
		if ((!(abs(current_absolute_amplitude) == std::numeric_limits<double>::infinity())) && (current_absolute_amplitude < 1.1 * max_amplitude_) || (max_amplitude_ == 0))
		{
			max_amplitude_ = current_absolute_amplitude;
		}
	/*std::cout << "curr ampl: " << current_absolute_amplitude << std::endl;
	std::cout << "max ampl: " << max_amplitude_ << std::endl;*/
}

template<typename T>
T VibratingPoint::CalculateMeanDifferenceInVector(std::vector<T> src)
{
	std::vector<T> difference_vector;
	
	auto it = src.begin();
	while (it != (src.end() - 1))
	{
		difference_vector.push_back(*(it + 1) - *it);
		it++;
	}

	T mean_difference;

	it = difference_vector.begin();
	mean_difference = *it;
	it++;

	while (it != difference_vector.end())
	{
		mean_difference += *it;
		it++;
	}
	mean_difference /= difference_vector.size();

	return mean_difference;
}

template<typename T>
T VibratingPoint::CalculateMaxDifferenceInVector(std::vector<T> src)
{
	std::vector<T> difference_vector;

	auto it = src.begin();
	while (it != (src.end() - 1))
	{
		difference_vector.push_back(*(it + 1) - *it);
		it++;
	}

	Scalar cv_scalar_mean_val = mean(src);
	T mean_val = cv_scalar_mean_val[0];

	return difference_vector[FindGlobalMaxIdx(difference_vector)] / mean_val;
}

template<typename T>
int VibratingPoint::FindGlobalMaxIdx(std::vector<T> src)
{
	auto it = src.begin();
	int idx = 0;
	T last_max = 0;
	while (it != src.end())
	{
		if (*it > last_max)
		{
			idx = std::distance(src.begin(), it);
			last_max = *it;
		}
		it++;
	}

	return idx;
}

void VibratingPoint::SetSensivity(double sensivity)
{
	std::cout << "sens: " << sensivity << std::endl;
	sensivity_ = sensivity;
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

Point3f VibratingPoint::GetCurrentAmplitude()
{
	return amplitude_;
}

double VibratingPoint::GetCurrentConfidenceLevel()
{
	return confidence_level_;
}

double VibratingPoint::GetMaxAmplitudeOverall()
{
	return max_amplitude_;
}
