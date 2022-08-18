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
	/*Histogram();*/
	Histogram(int width, int height, int x_limit, int id);
	~Histogram();

	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// Отрисовывает гистограмму
	void plot_histogram();
	// Устанавливает X-ы
	void set_x_values(std::vector<double> x_values);
	// Устанавливает Y-и
	void set_y_values(std::vector<float> y_values);
	// Обновляет статус флага отрисовки гистограммы
	void SetHistogramFlag(bool flag);

private:
	// Вычисляет значения для гистограммы
	Mat calc_histogram();

protected:
	// Флаг для отрисовки гистограммы
	bool is_histogram_plotted_;

private:
	// Название окна
	std::string winname_;

	float histogram_offset_;
	int histogram_frame_width_;
	int histogram_frame_height_;
	Mat histogram_;

	std::vector<double> x_values_;
	std::vector<float> y_values_;
	std::vector<int> values_accordance_;

	int x_limit_;


};

#endif