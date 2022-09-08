#ifndef VIBRATING_POINT_H
#define VIBRATING_POINT_H

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// other headers
#include "VVT-V2/peak_finder.h"
#include "VVT-V2/helper.h"

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
	void UpdateExtremumAmplitude(double extremum_amplitude);
	/*!
	* @brief Выполняет БПФ для нахождения частоты вибрации.
	* 
	После выполнения записывает в контейнер frequencies_ найденные пики, а также добавляет в контейнер x_ найденный спектр частот вибрации (для отображения гистограммы)
	*/
	void ExecuteFFT();
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
	* @brief Устанавливает чувствительность, влияющую на confidence
	*/
	void SetSensivity(double sensivity);
	/*!
	* @brief Проводит необходимые для вычисления амплитуды вычисления
	*/
	void CalculateAmplitude();
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
	* @brief Возвращает текущую "основную" частоту
	*/
	double GetCurrentMainFrequency();
	/*!
	* @brief Возвращает последнюю найденную относительную амплитуду
	*/
	Point3f GetRelativeAmplitude();
	/*!
	* @brief Возвращает последнюю найденную относительную амплитуду
	*/
	Point3f GetCurrentAmplitude();
	/*!
	* @brief Возвращает последний найденный уровень достоверности
	*/
	double GetCurrentConfidenceLevel();
	
public:
	/*!
	* @brief Максимум и минимум амплитуды среди всех VibratingPoint объектов-наследников
	* 
	* inline - для совмещенного объявления с определением переменной внутри класса
	*/
	static inline std::pair<double, double> extremum_amplitude_;

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
	* @brief Относительные амплитуды вибрации по x, y. Выражается отношением амплитуды
	*/
	Point3f relative_amplitude_;
	/*!
	* @brief Фактические амплитуды вибрации по x, y. Выражаюытся в пикселях
	*/
	Point3f current_amplitude_;
	/*!
	* @brief Контейнер для координат точки
	*/
	std::vector<Point2f> point_coordinates_;
	/*!
	* @brief Контейнер для координат точки по оси X
	*/
	std::vector<double> x_coordinates_;
	/*!
	* @brief Контейнер для координат точки по оси Y
	*/
	std::vector<double> y_coordinates_;
	/*!
	* @brief Контейнер для координат точки относительно нулевого уровня
	*/
	std::vector<cv::Point2f> vec_meaned_coordinates_of_point_;
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

private:
	/*!
	* @brief Максимальная разнциа в магнитудах в найденном спектре после БПФ
	* @see ExecuteFFT() и confidence_level_ - используется для определения "уровня достоверности" результатов у конкретной точки
	*/
	std::vector<float> mag_max_differences_;
};

#endif