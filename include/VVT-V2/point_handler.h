#ifndef POINT_HANDLER_H
#define POINT_HANDLER_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>
//#include <opencv2/ml.hpp>

// other headers
#include "VVT-V2/PeakFinder.h"

using namespace cv;

class PointHandler
{
public:
	PointHandler(Point2f init_coordinates, int update_rate, double sampling_rate);
	~PointHandler();
	// Применяет БПФ (FFT) для координат (позиций) точки
	void ExecuteFft();
	// Возвращает последний элемент вектора координат
	Point2f GetLastFoundCoordinates();
	// Возвращает текущую частоту точки
	double GetCurrentVibrationFrequency();
	// Возвращает текущую найденную амплитуду
	Point3f GetCurrentAmplitude();
	// Возвращает точку, необходимую для отображения координат
	Point2f GetTextCoordinates();
	// Возвращает true в случае попадания координат из входного значения в interaction_box точки
	bool IsInteracted(Point2i coordinates);
	// Добавляет новые координаты точки извне
	void AddNewCoordinate(Point2f position);
	// Добавляет новое значение времени кадра для трекаемой точки извне
	void AddFrameTimePos(double time);

private:

private:
	// Частота обновления частоты, цвета и тп
	int update_rate_;
	// Прямоугольник для взаимодействия с точкой
	Rect interaction_box_;
	// Половина длины прямоугольника взаимодействия
	int interaction_offset_;
	// Амплитуды вибрации по x, y, z
	Point3f amplitude_;
	// Контейнеры для координат (позиций) точки, её частот и временных координат
	std::vector<Point2f> point_coordinates_;
	std::vector<double> frequencies_;
	std::vector<double> point_time_coordinates_;
	// Параметры БПФ (FFT)
	double sampling_rate_;
};

#endif