#ifndef LONELY_POINT_HANDLER_H
#define LONELY_POINT_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/vibrating_point.h"
#include "VVT-V2/histogram.h"
#include "VVT-V2/output_to_csv.h"

using namespace cv;

// Класс описывает объект-крупную точку на видео, положение которой отслеживается с течением времени для вычисления частоты вибрации этой точки
class LonelyPointHandler : public Histogram, public VibratingPoint, public OutputToCsv
{
public:
	LonelyPointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient, std::string output_csv_filename);
	~LonelyPointHandler();
	// Частотный фильтр для нижней части диапазона
	void Filter(std::vector<float>& magnitudes);
	// Отрисовывает точку и данные, связанные с ней
	void Draw(Mat& frame) override;

private:
	// Отрисовывает гистограмму
	void DrawHistogram() override;
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
	// Возвращает точку, необходимую для отображения координат
	Point2f GetTextCoordinates();
	// Возвращает координаты прямоугольника взаимодействия
	Rect GetInteractionRect();

private:
	// Обновляем текущий цвет точки
	void UpdatePointColor() override;
private:
	// Коэффициент рескейла для отрисовки данных
	float resizing_coefficient_;
	// ID точки (нет блин почки)
	int point_id_;
	// Частота обновления частоты, цвета и тп
	int update_rate_;
	// Цвет точки
	Scalar point_color_;

};

#endif