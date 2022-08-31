#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// standart headers
#include <iostream>
//#include <sstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

/*!
* @brief Класс для создания объектов-гистограмм, на которые можно выводить различную информацию
*/
class Histogram
{
public:
	/*!
	* @brief Конструктор этого класса
	* @param width: ширина окна гистограммы (включая оффсет)
	* @param height: высота окна гистограммы (включая оффсет)
	* @param x_limit: максимальное (предельное) значение по оси X
	* @param id: ID точки
	* @param x_values: адрес контейнера со значениями для оси X
	* @param y_values: адрес контейнера со значеними для оси Y
	*/
	Histogram(int width, int height, int x_limit, int id, std::vector<double>& x_values, std::vector<float>& y_values);
	/*!
	* @brief Деструктор этого класса
	* 
	* Окно гистограммы разрушается внутри деструктора
	*/
	~Histogram();

	/*!
	* @brief callback функция для определения события (используется для хэндлинга клика мышью)
	*/
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	/*!
	* @brief callback функция для определения события (используется для хэндлинга клика мышью)
	*/
	void DetectEvent(int event, int x, int y, int flags);
	/*!
	* @brief Выводит гистограмму в окно гистограммы
	*/
	void ShowHistogram();
	/*!
	* @brief Устанавливает X-ы
	*/
	void SetXValues(std::vector<double> x_values);
	/*!
	* @brief Устанавливает Y-ы
	*/
	void SetYValues(std::vector<float> y_values);
	/*!
	* @brief Устанавливает статус флага отрисовки гистограммы
	*/
	void SetHistogramFlag(bool flag);
	/*!
	* @brief Обновляет статус окна гистограммы (например, для вывода на передний план)
	* 
	* На данный момент функция не используется :/
	*/
	void SetHistogramWindowProperty(int prop_id, int prop_value);

private:
	/*!
	* @brief Подгатавливаем Histogram::histogram_background_ для дальнейшего использования
	*/
	void InitHistogramBackground();
	/*!
	* @brief Вычисляет значения для гистограммы
	*/
	Mat CalcHistogram();
	/*!
	* @brief Проверяет, соответствует ли точка координате мыши
	*/
	bool IsInteracted(int x, int interval);
	/*!
	* @brief Выводит значение частоты рядом с курсором
	*/
	void PlotMouseValue(Mat& frame, int value_idx);
	/*!
	* @brief Для ограничения кол-ва знаков
	*/
	std::string to_string_with_precision(const float value, const int n = 6);

protected:
	/*!
	* @brief Флаг для отрисовки гистограммы
	*/
	bool is_histogram_plotted_;

private:
	/*!
	* @brief Название окна
	*/
	std::string winname_;
	/*!
	* @brief Оффсет рамки гистограммы
	*/
	float histogram_offset_;
	/*!
	* @brief Ширина окна гистограммы (включая оффсет)
	*/
	int histogram_frame_width_;
	/*!
	* @brief Высота окна гистограммы (включая оффсет)
	*/
	int histogram_frame_height_;
	/*!
	* @brief cv::Mat изображение гистограммы
	*/
	Mat histogram_;
	/*!
	* @brief cv::Mat фон для гистограммы
	*/
	Mat histogram_background_;
	/*!
	* @brief Значения гистограммы по оси X
	*/
	std::vector<double>* x_values_;
	/*!
	* @brief Значения гистограммы по оси Y
	*/
	std::vector<float>* y_values_;
	/*!
	* @brief Отступ для подписей оси
	*/
	float axis_signature_offset_;
	/*!
	* @brief Интервал для делений оси и количество этих делений
	*/
	float signature_interval_;
	/*!
	* @brief Количество подписей на оси
	*/
	int signature_amount_;
	/*!
	* @brief Максимальное (предельное) значение по оси X
	*/
	int x_limit_;
	/*!
	* @brief Координаты мыши (курсора)
	*/
	Point2i last_mouse_coordinates_;
	/*!
	* @brief Процент диапазона, который будет нижней мертвой зоной: 1.0 - 100%
	*/
	float dead_zone_coeff_;

};

#endif