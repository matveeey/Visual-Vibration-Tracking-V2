#ifndef COLORED_POINT_HANDLER_H
#define COLORED_POINT_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// my headers
#include "VVT-V2/vibrating_point.h"
#include "VVT-V2/histogram.h"
#include "VVT-V2/output_to_csv.h"
#include "VVT-V2/helper.h"

using namespace cv;

enum mode
{
	DEFAULT,
	COLORING_BASED_ON_AMPLITUDE,
	COLORING_BASED_ON_FREQUENCY
};

// Класс описывает объект-маленькую точку с возможностью подсветки цветом в зависимости от амплитуды или частоты вибрации
class ColoredPointHandler : public Histogram, public VibratingPoint, public OutputToCsv
{
public:
	ColoredPointHandler(Point2f init_coordinates, int update_rate, double sampling_rate, int point_id, float resizing_coefficient, std::string output_csv_filename);
	~ColoredPointHandler();
	// Отрисовывает точку и данные, связанные с ней
	void Draw(Mat& frame) override;
	// Устанавливает нужный режим отрисовки цветом
	void SetMode(int mode);

private:
	// Отрисовывает гистограмму
	void DrawHistogram() override;
	// Отрисовывает точку на кадре
	void DrawPoint(Mat& frame);
	// Отрисовывает линию перемещения точки
	void DrawInteractionRectangle(Mat& frame);

private:
	// Обновляем текущий цвет точки
	void UpdatePointColor() override;

private:
	// Текущий режим работы (цвета на основе амплитуд/частот)
	int mode_;
	// Коэффициент рескейла для отрисовки данных
	float resizing_coefficient_;
	// ID точки (нет блин почки)
	int point_id_;
	// Частота обновления частоты, цвета и тп
	int update_rate_;
	// Цвет точки
	Scalar point_color_;
	// Флаг, отвечающий за тип окраски точки

};

#endif