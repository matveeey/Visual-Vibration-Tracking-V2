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
* @brief  ласс дл€ создани€ объектов-обработчиков контуров.
* 
* —пособен найти контура на входном изображении (принимаетс€ в конструкторе), отрисовать их, и произвести определенные операции над ними (например, уменьшить размер
* или плотность точек в контуре
*/

class ContourHandler
{
public:
	/*!
	* @brief  онструктор этого класса
	* @param frame: Mat-instance изображени€, на котором будет выполнен поиск контуров. »зображение может быть в RGB/gray-scale (в первом случае будет сконвертировано внутри)
	* @param ROI: регион интереса, на котором будут найдены контуры. 
	*/
	ContourHandler(Mat frame, Rect* ROI = nullptr);
	/*!
	* @brief ќтрисовывает контура на изображении
	* @param frame: адрес изображени€, на котором будет отрисованы найденные контура
	*/
	void DrawContours(Mat& frame);
	/*!
	* @brief ¬озвращает найденные контура в виде { { contour_1 }, { contour_2 }, ... , { contour_n } }
	* @return  онтуры
	*/
	std::vector<std::vector<Point2i>> GetContours();
	/*!
	* @brief ¬озвращает найденные контура в виде { contour_1, contour_2, ... , contour_n }.
	* 
	* ‘актически выполн€ет "склеивание" найденных контуров в один вектор
	* @return »зображение с нарисованными контурами
	*/
	std::vector<Point2f> GetContinousContours();

private:
	/*!
	* @brief ”дал€ет каждый второй элемент вектора (уменьшает размер в два раза)
	* @param src: вектор, который будет "прор€жЄн"
	*/
	template<typename T>
	std::vector<T> MakeTwiceThinner(std::vector<T> src);
	/*!
	* @brief ¬ычисл€ет рассто€ние между двум€ точками
	* @param point1: перва€ точка
	* @param point2: втора€ точка
	* @return –ассто€ние между точками
	*/
	template<typename T>
	float CalculateDistance(T point1, T point2);

private:
	/*!
	* @brief  онтуры в виде { { contour_1 }, { contour_2 }, ... , { contour_n } }
	*/
	std::vector<std::vector<Point>>contour_shapes_;
	/*!
	* @brief ћинимальное рассто€ние между точками контура, при котором "лева€" ("перва€") точка будет сохранена
	* @see »спользуетс€ в GetContinousContours()
	*/
	float minimal_distance_;

};

#endif
