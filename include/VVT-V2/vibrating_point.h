#ifndef VIBRATING_POINT_H
#define VIBRATING_POINT_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// other headers
#include "VVT-V2/peak_finder.h"

using namespace cv;

/*!
* @brief Абстрактный класс для описания вибрирующей точки
* 
* Объект класса-наследника будет иметь возможность добавления новых пространственных и временных позиций, нахождения частоты вибрации точки на кадре на основе этих данных (ExecuteFFT())
* 
* А также проверить пересечение курсором области взаимодействия с точкой (метод IsInteracted())
*/
class VibratingPoint
{
public:
	/*!
	* @brief Выполняет БПФ для нахождения частоты вибрации.
	* 
	После выполнения записывает в контейнер frequencies_ найденные пики, а также добавляет в контейнер x_ найденный спектр частот вибрации (для отображения гистограммы)
	*/
	void ExecuteFFT();
	/*!
	* @brief Частотный фильтр для нижней части диапазона
	* @param magnitudes: массив, в котором будет проведена фильтрация
	*/
	template<class T>
	void DeadzoneFilter(std::vector<T>& input_vector);
	/*!
	* @brief Добавляет новую пространственную координату в конец контейнера VibratingPoint::point_coordinates_;
	* @param position: координаты точки
	*/
	void AddNewPointPosition(Point2f position);
	/*!
	* @brief Добавляет новую временную координату (время кадра, на котором новая позиция точки была трекнута) в конец контейнера VibratingPoint::point_time_coordinates_;
	* @param time: время в миллисекундах
	*/
	void AddNewPointTime(double time);
	/*!
	* @brief Проверяет точку на наличие взаимодействия с ней (если курсор попал в область взаимодействия точки VibratingPoint::interaction_box_)
	* @param coordinates: координаты курсора
	*/
	bool IsInteracted(Point2i coordinates);
	/*!
	* @brief Устанавливает максимальное найденное значение амплитуды точки в кадре
	* @note Необходимо для вычисления цвета "цветной" точки, а также для вычисления относительной амплитуды движения "одинокой" (lonely) точки
	* @param amplitude: найденная максимальная амплитуда, которая будет установлена
	*/
	void UpdateMaxAmplitudeOverall(double amplitude);
	/*!
	* @brief Отрисовывает точку на кадре
	* @param frame: изображение, на котором будет происходить отрисовка
	*/
	virtual void Draw(Mat& frame) = 0;

protected:
	/*!
	* @brief Отрисовывает гистограмму спектра частот колебаний точки (полученную после БПФ в методе ExecuteFFT())
	*/
	virtual void DrawHistogram() = 0;
	/*!
	* @brief Обновляем текущий цвет точки
	*/
	virtual void UpdatePointColor() = 0;

private:
	/*!
	* @brief Вычисляет среднее значение разности элементов массива
	* @param src - входной вектор для анализа
	*/
	template<typename T>
	T CalculateMeanDifferenceInVector(std::vector<T> src);
	/*!
	* @brief Вычисляет максимальное значение разности элементов массива
	* @param src - входной вектор для анализа
	*/
	template<typename T>
	T CalculateMaxDifferenceInVector(std::vector<T> src);
	/*!
	* @brief Вычисляет основную частоту
	* @param src - входной вектор для анализа
	*/
	template<typename T>
	int FindGlobalMaxIdx(std::vector<T> src);

private:
	std::vector<float> max_differences_;

// Сеттеры
public:
	void SetSensivity(double sensivity);

// Геттеры
public:
	/*!
	* @brief Возвращает последний элемент вектора координат
	*/
	Point2f GetLastFoundCoordinates();
	/*!
	* @brief Возвращает вектор текущих частот точки
	*/
	std::vector<double> GetCurrentVibrationFrequency();
	/*!
	* @brief Возвращает последнюю найденную амплитуду
	*/
	Point3f GetCurrentAmplitude();
	/*!
	* @brief Возвращает последний найденный уровень достоверности
	*/
	double GetCurrentConfidenceLevel();

protected:
	/*!
	* @brief Прямоугольник (область) взаимодействия с точкой
	*/
	Rect interaction_box_;
	/*!
	* @brief Половина длины прямоугольника взаимодействия
	*/
	int interaction_offset_;
	/*!
	* @brief Флаг взаимодействия с точкой
	* 
	* True - в случае взаимодействия, false - в противном
	*/
	bool interacted_;
	/*!
	* @brief Амплитуды вибрации по x, y (дополнительно и по z, но пока z не активно)
	*/
	Point3f amplitude_;
	/*!
	* @brief Контейнер для координат точки
	*/
	std::vector<Point2f> point_coordinates_;

	// Если объявить double confidence_level_ в этом месте (буквально после этого комментария) (visual studio 2022, номер сборки лень смотреть), программа будет падать :/ 
	// Почему так?

	/*!
	* @brief Контейнер для частот точки
	*/
	std::vector<double> frequencies_;
	/*!
	* @brief Главная частота (основная)
	*/
	double main_frequency_;
	/*!
	* @brief Контейнер для временных координат точки
	*/
	std::vector<double> point_time_coordinates_;
	/*!
	* @brief Частота сэмплирования (фактически - FPS исходного видео)
	*/
	double sampling_rate_;
	/*!
	* @brief Радиус точки, который нужен для её отрисовки
	*/
	int current_point_radius_;
	/*!
	* @brief Дефолтный радиус точки
	*/
	int default_point_radius_;
	/*!
	* @brief Текущая максимальная амплитуда
	*/
	double max_amplitude_;

	/*!
	* @brief Контейнер для хранения координат гистограммы по оси X (фактически - найденный спектр частот вибрации точки после выполнения БПФ)
	*/
	std::vector<double> x_;
	/*!
	* @brief Контейнер для хранения координат гистограммы по оси Y (фактически - найденные магнитуды точки после выполнения БПФ)
	*/
	std::vector<float> y_;
	/*!
	* @brief Чувствительность определения вибрации
	*/
	double sensivity_;
	/*!
	* @brief Рейтинг достоверности вибрации
	*/
	double confidence_level_;
};

#endif