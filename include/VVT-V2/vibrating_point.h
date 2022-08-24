#ifndef VIBRATING_POINT_H
#define VIBRATING_POINT_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// other headers
#include "VVT-V2/PeakFinder.h"

using namespace cv;

// Абстрактный класс для создания прародителя точки
class VibratingPoint
{
public:
	void ExecuteFFT();
	// Добавляет новую пространственную координату
	void AddNewPointPosition(Point2f position);
	// Добавляет новую временную координату
	void AddNewPointTime(double time);
	// Прaверяет точку на наличие взаимодействия
	bool IsInteracted(Point2i coordinates);
	// Отрисовывает точку на кадре
	virtual void Draw(Mat& frame) = 0;

protected:
	// Отрисовывает гистограмму магнитуд частот колебаний точки
	virtual void DrawHistogram() = 0;
	// Обновляем текущий цвет точки
	virtual void UpdatePointColor() = 0;

public:
	// Возвращает последний элемент вектора координат
	Point2f GetLastFoundCoordinates();
	// Возвращает текущие частоты точки
	std::vector<double> GetCurrentVibrationFrequency();
	// Возвращает текущую найденную амплитуду
	Point3f GetCurrentAmplitude();

protected:
	// Прямоугольник для взаимодействия с точкой
	Rect interaction_box_;
	// Половина длины прямоугольника взаимодействия
	int interaction_offset_;
	// True - в случае взаимодействия, false - в противном
	bool interacted_;
	// Амплитуды вибрации по x, y, z
	Point3f amplitude_;
	// Контейнеры для координат (позиций) точки, её частот и временных координат
	std::vector<Point2f> point_coordinates_;
	std::vector<double> frequencies_;
	std::vector<double> point_time_coordinates_;
	// Параметры БПФ (FFT)
	double sampling_rate_;

	// TEMPORARY
	std::vector<double> x_;
	std::vector<float> y_;

};

#endif