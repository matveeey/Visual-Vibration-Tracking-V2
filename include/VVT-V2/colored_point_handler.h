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

/*!
* @brief Дочерний (от VibratingPoint) класс. Предназначен для помещения множества цветных точек на кадр и отслеживания вибрации каждой из них
*
* Объект класса - "цветная" точка на видео, положение которой отслеживается с течением времени для вычисления частоты вибрации этой точки
*
* Точку можно отрисовать методом Draw() на кадре
*
* Также при взаимодействии с точкой (VibratingPoint::IsInteracted()) будет выводиться гистограмма частот в отдельное окно (DrawHistogram())
*/
class ColoredPointHandler : public Histogram, public VibratingPoint, public OutputToCsv
{
	/*!
	* @brief Перечисление, используемое для более удобного отображения текущего режима подсветки точки
	*/
	enum coloring_mode
	{
		DEFAULT,
		COLORING_BASED_ON_AMPLITUDE,
		COLORING_BASED_ON_FREQUENCY
	};

public:
	/*!
	* @brief Конструктор этого класса
	* @param init_coordinates: самые первые координаты точки (фактически - координаты клика мышью на окне, куда выводилось исходное видео)
	* @param sampling_rate: частота сэмплирования (фактически - FPS исходного видео)
	* @param point_id: ID точки
	* @param radius_resize_factor: коэффициент масштабирования радиуса отрисовываемой точки
	* @param output_csv_filename: имя (путь) csv-файла для записи метаданных
	* @note Коэффициент масштабирования: FrameHandler::InitTextResizeFactors()
	*/
	ColoredPointHandler(Point2f init_coordinates, double sampling_rate, int point_id, float radius_resize_factor, std::string output_csv_filename);
	/*!
	* @brief Деструктор этого класса
	*
	* Вызывает метод родительского класса OutputToCsv Write() для записи метаданных
	*/
	~ColoredPointHandler();
	/*!
	* @brief Отрисовывает точку и данные, связанные с ней
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	void Draw(Mat& frame) override;
	/*!
	* @brief Устанавливает нужный режим отрисовки цветом
	* @param coloring_mode: режим, который нужно установить 
	* @note ColoredPointHandler::coloring_mode
	*/
	void SetColoringMode(int coloring_mode);

private:
	/*!
	* @brief Отрисовывает гистограмму
	*/
	void DrawHistogram() override;
	/*!
	* @brief Отрисовывает точку на кадре
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	void DrawPoint(Mat& frame);
	/*!
	* @brief Отрисовывает прямоугольник взаимодействия
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	void DrawInteractionRectangle(Mat& frame);

private:
	/*!
	* @brief Обновляет текущий цвет точки
	*/
	void UpdatePointColor() override;

private:
	// Текущий режим работы (цвета на основе амплитуд/частот)
	int coloring_mode_;
	/*!
	* @brief Коэффициент рескейла для радиуса точки
	*/
	float radius_resize_factor_;
	/*!
	* @brief ID точки (нет блин почки)
	*/
	int point_id_;
	/*!
	* @brief Цвет точки
	*/
	Scalar point_color_;

};

#endif