#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

// Класс предназначен для создания объектов-гистограмм, на которые можно выводить различную информацию
class Histogram
{
public:
	Histogram(int width, int height, int x_limit);

	// Отрисовывает гистограмму
	void plot_histogram();
	// Устанавливает X-ы
	void set_x_values(std::vector<double> x_values);
	// Устанавливает Y-и
	void set_y_values(std::vector<float> y_values);

private:
	// Вычисляет значения для гистограммы
	Mat calc_histogram();

private:
	float histogram_offset_;
	int histogram_frame_width_;
	int histogram_frame_height_;
	Mat histogram_;

	std::vector<double> x_values_;
	std::vector<float> y_values_;

	int x_limit_;


};

#endif