#ifndef CONTOUR_HANDLER_H
#define CONTOUR_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/helper.h"

using namespace cv;

/*!
* @brief Дочерний (от VibratingPoint) класс. Предназначен для помещения множества цветных точек на кадр и отслеживания вибрации каждой из них
*
* Объект класса - "цветная" точка на видео, положение которой отслеживается с течением времени для вычисления частоты вибрации этой точки
*
* Точку можно отрисовать методом Draw() на кадре
*
* Также при взаимодействии с точкой (VibratingPoint::IsInteracted()) будет выводиться гистограмма частот в отдельное окно (DrawHistogram())
*/

class ContourHandler
{
public:
	ContourHandler(Mat frame, Rect ROI);
	void DrawContours(Mat& frame);
	std::vector<std::vector<Point2i>> GetContours();
	std::vector<Point2f> GetContinousContours();

private:
	template<typename T>
	std::vector<T> MakeTwiceThinner(std::vector<T> src);

	template<typename T>
	float CalculateDistance(T point1, T point2);

private:
	std::vector<std::vector<Point>>contour_shapes_;
	float minimal_distance_;

};

#endif
