#ifndef POINT_HANDLER_H
#define POINT_HANDLER_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/histogram.h"

// other headers
#include "VVT-V2/PeakFinder.h"

using namespace cv;

class PointHandler : public Histogram
{
public:
	PointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient);
	~PointHandler();
	// Применяет БПФ (FFT) для координат (позиций) точки
	void ExecuteFft();
	// Частотный фильтр для нижней части диапазона
	void Filter(std::vector<float>& magnitudes);
	// Возвращает true в случае попадания координат из входного значения в interaction_box точки
	bool IsInteracted(Point2i coordinates);
	// Добавляет новые координаты точки извне
	void AddNewCoordinate(Point2f position);
	// Добавляет новое значение времени кадра для трекаемой точки извне
	void AddFrameTimePos(double time);
	// Отрисовывает точку и данные, связанные с ней
	void Draw(Mat& frame);

private:
	// Отрисовывает гистограмму
	void DrawHistogram();
	// Отрисовывает точку на кадре
	void DrawPoint(Mat& frame);
	// Отрисовывает линию перемещения точки
	void DrawPointTrack(Mat& frame);
	// Отрисовывает прямоугольник взаимодействия
	void DrawInteractionRectangle(Mat& frame);
	// Отрисовывает текстовые данные рядом с точкой
	void DrawTextData(Mat& frame);


// Геттеры
public:
	// TEMPORARY
	std::vector<double> GetX();
	std::vector<float> GetY();
	// Возвращает последний элемент вектора координат
	Point2f GetLastFoundCoordinates();
	// Возвращает текущие частоты точки
	std::vector<double> GetCurrentVibrationFrequency();
	// Возвращает текущую найденную амплитуду
	Point3f GetCurrentAmplitude();
	// Возвращает точку, необходимую для отображения координат
	Point2f GetTextCoordinates();
	// Возвращает координаты прямоугольника взаимодействия
	Rect GetInteractionRect();

private:
	// Обновляем текущий цвет точки
	void UpdatePointColor();
private:
	// Коэффициент рескейла для отрисовки данных
	float resizing_coefficient_;
	// ID точки (нет блин почки)
	int point_id_;
	// Частота обновления частоты, цвета и тп
	int update_rate_;
	// Прямоугольник для взаимодействия с точкой
	Rect interaction_box_;
	// Половина длины прямоугольника взаимодействия
	int interaction_offset_;
	// True - в случае взаимодействия, false - в противном
	bool interacted_;
	// Цвет точки
	Scalar point_color_;
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