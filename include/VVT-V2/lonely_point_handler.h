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

/*!
* @brief Дочерний (от VibratingPoint) класс. Предназначен для помещения точки на кадр и отслеживания её вибрации впоследствии
* 
* Объект класса - "одинокая" точка на видео, положение которой отслеживается с течением времени для вычисления частоты вибрации этой точки
* 
* Точку можно отрисовать методом Draw() на кадре
* 
* Также при взаимодействии с точкой (VibratingPoint::IsInteracted()) будет выводиться гистограмма частот в отдельное окно (DrawHistogram())
*/
class LonelyPointHandler : public Histogram, public VibratingPoint, public OutputToCsv
{
public:
	/*!
	* @brief Конструктор этого класса
	* @param init_coordinates: самые первые координаты точки (фактически - координаты клика мышью на окне, куда выводилось исходное видео)
	* @param sampling_rate: частота сэмплирования (фактически - FPS исходного видео)
	* @param point_id: ID точки
	* @param text_resize_factor: коэффициент масштабирования текста, выводимого на изображение при отрисовке частоты и относительной амплитуды вибрации
	* @param output_csv_filename: имя (путь) csv-файла для записи метаданных
	* @note Коэффициент масштабирования: FrameHandler::InitTextResizeFactors()
	*/
	LonelyPointHandler(Point2f init_coordinates, double sampling_rate, int point_id, float text_resize_factor, std::string output_csv_filename);
	/*!
	* @brief Деструктор этого класса
	* 
	* Вызывает метод родительского класса OutputToCsv Write() для записи метаданных
	*/
	~LonelyPointHandler();
	/*!
	* @brief Частотный фильтр для нижней части диапазона
	* @param magnitudes: массив, в котором будет проведена фильтрация
	*/
	void Filter(std::vector<float>& magnitudes);
	/*!
	* @brief Отрисовывает точку и данные, связанные с ней
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	void Draw(Mat& frame) override;

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
	* @brief Отрисовывает линию перемещения (трек) точки
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	void DrawPointTrack(Mat& frame);
	/*!
	* @brief Отрисовывает прямоугольник взаимодействия
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	void DrawInteractionRectangle(Mat& frame);
	/*!
	* @brief Отрисовывает текстовые данные рядом с точкой
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	void DrawTextData(Mat& frame);


// Геттеры
public:
	/*!
	* @brief Возвращает точку, необходимую для отображения координат
	* @return Координаты текста для отображения
	*/
	Point2f GetTextCoordinates();
	/*!
	* @brief Возвращает координаты прямоугольника взаимодействия
	* @return Прямоугольник (область) взаимодействия
	*/
	Rect GetInteractionRect();

private:
	/*!
	* @brief Обновляем текущий цвет точки
	*/
	void UpdatePointColor() override;
private:
	/*!
	* @brief Коэффициент рескейла для отрисовки данных
	*/
	float text_resize_factor_;
	/*!
	* @brief ID точки (нет блин почки)
	*/
	int point_id_;
	/*!
	* @brief Частота обновления частоты, цвета и тп
	*/
	int update_rate_;
	/*!
	* @brief Цвет точки
	*/
	Scalar point_color_;

};

#endif