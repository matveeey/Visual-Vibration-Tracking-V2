#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// standart headers
#include <iostream>
//#include <sstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

// Класс предназначен для создания объектов-гистограмм, на которые можно выводить различную информацию
class Histogram
{
public:
	Histogram(int width, int height, int x_limit, int id);
	~Histogram();

	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// Отрисовывает гистограмму
	void ShowHistogram();
	// Устанавливает X-ы
	void SetXValues(std::vector<double> x_values);
	// Устанавливает Y-и
	void SetYValues(std::vector<float> y_values);
	// Обновляет статус флага отрисовки гистограммы
	void SetHistogramFlag(bool flag);

private:
	// Подгатавливаем histogram_background_ для дальнейшего использования
	void InitHistogramBackground();
	// Вычисляет значения для гистограммы
	Mat CalcHistogram();
	// Проверяет, соответствует ли точка координате мыши
	bool IsInteracted(int x, int interval);
	// Выводит значение частоты рядом с мышкой
	void PlotMouseValue(Mat& frame, int value_idx);
	// Для ограничения кол-ва знаков
	std::string to_string_with_precision(const float value, const int n = 6);

protected:
	// Флаг для отрисовки гистограммы
	bool is_histogram_plotted_;

private:
	// Название окна
	std::string winname_;
	// Оффсет рамки гистограммы
	float histogram_offset_;
	// Высота и ширина окна гистограммы (включая оффсет)
	int histogram_frame_width_;
	int histogram_frame_height_;
	// Сама гистограмма
	Mat histogram_;
	// Фон для гистограммы
	Mat histogram_background_;
	// Значения по x, y
	std::vector<double> x_values_;
	std::vector<float> y_values_;
	// Отступ для подписей оси
	float axis_signature_offset_;
	// Интервал для делений оси и количество этих делений
	float signature_interval_;
	int signature_amount_;
	// Максимальное (предельное) значение по оси X
	int x_limit_;
	// Координаты мыши
	Point2i last_mouse_coordinates_;


};

#endif