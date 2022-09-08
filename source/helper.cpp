#include "VVT-V2/helper.h"

std::string HelperFunctions::GenerateCsvFilename(std::string additional_text)
{
	// Имя файла, в который будет производиться запись
	std::string csv_file_name;
	// Начало названия выходного файла
	csv_file_name = "C:/Users/seeyo/source/repos/Visual-Vibration-Tracking-V2/docs/output_";
	// Добавляем дополнительный текст при желании
	csv_file_name += additional_text;
	// Время
	auto time = std::time(nullptr);
	// Записываем текущее время
	std::stringstream date_n_time;
	date_n_time << std::put_time(std::localtime(&time), "%F_%T");
	// Переводим дату и время в строковый тип
	std::string date_n_time_str = date_n_time.str();
	// Меняем ":" на "-" для корректного имени файла
	std::replace(date_n_time_str.begin(), date_n_time_str.end(), ':', '-');
	// Добавляем дату и время к имени файла + расширение файла
	csv_file_name += date_n_time_str + ".csv";

	std::cout << csv_file_name << std::endl;

	return csv_file_name;
}

cv::Scalar HelperFunctions::RatioToRgb(double ratio)
{
	//
	int normalized = static_cast<int>(ratio * 256 * 4);

	// find the region for this position
	int region = normalized / 256;

	// find the distance to the start of the closest region
	int x = normalized % 256;

	int r = 0, g = 0, b = 0;

	switch (region)
	{
	case 0: r = 0;		g = 0;		b = 255;	g += x; break; // синий:сине-зеленый
	case 1: r = 0;		g = 255;	b = 255;	b -= x; break; // сине-зеленый:зеленый
	case 2: r = 0;		g = 255;	b = 0;		r += x; break; // зеленый:желтый
	case 3: r = 255;	g = 255;	b = 0;		g -= x; break; // желтый:красный
	}
	
	return cv::Scalar(b, g, r);
}

void HelperFunctions::FindLocalMaxima(cv::Mat& input_array, cv::Mat& local_maxima, double threshold_value)
{
	cv::Mat dilated_input_image, thresholded_input_image, thresholded_input_8bit;
	dilate(input_array, dilated_input_image, cv::Mat());
	compare(input_array, dilated_input_image, local_maxima, cv::CMP_EQ);
	threshold(input_array, thresholded_input_image, threshold_value, 255, cv::THRESH_BINARY);
	thresholded_input_image.convertTo(thresholded_input_8bit, CV_8U);
	bitwise_and(local_maxima, thresholded_input_8bit, local_maxima);
}

