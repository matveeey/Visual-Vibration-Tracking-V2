#ifndef VIBRATION_DETECTOR_H
#define VIBRATION_DETECTOR_H

// VREMENNO
#define MAIN_WINDOW_NAME "Video"
#define V_MONITOR_WINDOW_NAME "Vibration Monitor"

// standart headers
#include <iostream>
#include <vector>

// OpenCV headers
#include <opencv2/video/tracking.hpp>

// my headers
#include "VVT-V2/lonely_point_handler.h"
#include "VVT-V2/colored_point_handler.h"
#include "VVT-V2/frame_handler.h"
#include "VVT-V2/helper.h"
#include "VVT-V2/contour_handler.h"

/*!
* @brief Класс для создания объектов-детекторов вибрации
*
* Объект класса является основной единицей в программе. Внутри себя создает объекты различных классов, описанных в заголовочных файлах проекта. См.
*/
class VibrationDetector 
{
	enum main_mode
	{
		DEFAULT,
		SELECTINGROI,
		PAUSE
	};

	enum colored_point_mode
	{
		COLORING_DEFAULT,
		COLORING_BASED_ON_FREQUENCY,
		COLORING_BASED_ON_AMPLITUDE
	};

public:
	VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name);
	~VibrationDetector();
	// Выполняет детекцию вибрации
	void ExecuteVibrationDetection();

private:
	// Обслуживает очереди на удаление и создание. Необходимо для того, чтобы не нарушить нумерацию точек во время выполнения основного цикла
	void ServeTheQueues();
	// Создаем "обычную" точку
	void CreateNewPoint(Point2f mouse_coordinates);
	// Обработчик левых кликов
	void LeftClickHandler(Point2f mouse_coordinates);
	// Обработчик "цветных" точек
	void CreateNewColoredPoint(Point2f mouse_coordinates);
	// Удаляем объект "обычную" точку
	void DeletePoints(Point2i mouse_coordinates);
	// Удаляем объекты "цветная" точка
	void DeleteColoredPoints();
	// Удаляет "цветные" и "обычные" точки, если уровень доверия к нми слишком низкий
	void FindAndDeleteUncofidentPoints();
	// callback functions for detecting the click
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void DetectEvent(int event, int x, int y, int flags);
	// Обычный режим
	void DefaultModeHandler(int event, int x, int y);
	// Режим выделения региона интереса
	void RoiSelectionModeHandler(int event, int x, int y);
	// Удаляет перспективные искажения
	Mat MakeWarpedFrame(Mat frame, std::vector<Point2i> warping_figure);
	// Трекает точки алгоритмом Лукаса-Канаде и вызывает БПФ
	void TrackAndCalc();
	// Находит подходящие для трекинга точки на изображении
	std::vector<Point2f> FindGoodFeatures(Mat frame, Rect roi);
	// Рисует точки и выводит данные
	void DrawDebugLkWinRectangle(Mat& frame);
	// Масштабирует, рисует и выводит на экран обработанный кадр с точками
	void DrawAndOutput(Mat& frame);

private:
	/*!
	* @brief Фреймпроцессор
	*/
	FrameHandler* frame_handler;
	/*!
	* @brief Флаг запущенного основного алгоритма
	* @see ExecuteVibrationDetection()
	*/
	bool running_;
	/*!
	* @brief Коэффициент автоскейла размеров окна в зависимости от разрешения исходного видео
	*/
	float res_mp_;
	/*!
	* @brief Псевдо-id (порядковывй номер точки)
	*/
	int point_id_;
	/*!
	* @brief Последняя позиция курсора в главном окне
	*/
	Point last_mouse_position_;
	/*!
	* @brief Основной режим работы (enum main_mode)
	*/
	int current_mode_;
	/*!
	* @brief Флаг для полноэкранного режима
	*/
	bool fullscreen_;

	///
	// NORMAL MODE
	///
	/*!
	* @brief Очередь на создание "обычных" точек
	*/
	std::vector<Point2i> l_click_queue_;
	/*!
	* @brief Очередь на создание "цветных" точек
	*/
	std::vector<Point2f> c_point_queue_;
	/*!
	* @brief Очередь на удаление точек
	*/
	std::vector<Point2i> delete_queue_;
	/*!
	* @brief Очередь на отрисовку гистограм
	*/
	std::vector<Point2i> histograms_queue_;
	/*!
	* @brief Вектор обработчиков "одиноких" точек
	*/
	std::vector<LonelyPointHandler*> vec_lonely_point_handlers_;
	/*!
	* @brief Вектор обработчиков "цветных" точек
	*/
	std::vector<ColoredPointHandler*> vec_colored_point_handlers_;

	///
	// ADJUST SENSIVITY MODE
	///
	/*!
	* @brief Чувствительность при трекинге вибрации 
	* 
	* Окно для изменения чувствительности вызывается кнопкой "A" при работающей проге
	* 
	* Чем выше значение - тем больше шумов будет поймано, но и тем меньше по амплитуде вибрации можно будет задетектить
	*/
	int sensivity_in_percents_;

	/*!
	* @brief Текущий Mat-instance только что найденного кадра из видео
	*/
	Mat current_tracking_frame_;
	/*!
	* @brief Mat-instance, который будет показан в окне
	* @see DrawAndOutput()
	*/
	Mat frame_to_be_shown_;
	/*!
	* @brief Изображение цветной шкалы внизу экрана
	* @see FrameHandler::GenerateGradScale() и DrawAndOutput()
	*/
	Mat grad_scale_;
	/*!
	* @brief Текущий режим подсветки точек при выбранном регионе интереса (по амплитуде/по частоте)
	*/
	int colored_point_mode_;

	///
	// ROI SELECTING MODE
	///
	// 
	/*!
	* @brief Флаг, показывающий происходит ли в данный момент выделение ROI
	*/
	bool roi_selecting_;
	/*!
	* @brief Флаг, показывающий был ли выделен ROI или нет
	*/
	bool roi_selected_;
	/*!
	* @brief Координаты первой точки прямоугольник (top left, bottom left, top right, bottom right - смотря "в какую сторону" выделять ROI с помощью мыши (курсора))
	*/
	Point2i tl_click_coords_;
	
	/*!
	* @brief Имя (путь) входного видео
	*/
	std::string input_file_name_;
	/*!
	* @brief Имя (путь) видео, в котором будет результат детектинга
	*/
	std::string output_file_name_;
	/*!
	* @brief Имя (путь) файла для хранения метаданных
	* @see OutputToCsv
	*/
	std::string output_csv_file_name_;
	/*!
	* @brief Имя главного окна
	*/
	std::string window_name_;

	/*!
	* @brief Флаг, показывающий, происходит ли в данный момент выделение фигуры (эта возможность отключена временно)
	*/
	bool warping_figure_selecting_;
	/*!
	* @brief Координаты фигуры для исправления перспективы
	*/
	std::vector<Point> warping_figure_;

	/*!
	* @brief Предыдущий кадр видео в грейскейл и с небольшим блюром (FrameHandler::MakeGrayFrame())
	*/
	Mat prev_img_gray_;
	/*!
	* @brief Следующий (или текущий после "предыдущего" - смотря как удобнее для понимания) кадр видео в грейскейл и с небольшим блюром (FrameHandler::MakeGrayFrame())
	*/
	Mat next_img_gray_;
	/*!
	* @brief Размер окна для алгоритма Лукаса-Канаде
	*/
	int lk_win_size_;
	/*!
	* @brief Количество слоев пирамиды изображений в алгоритме Л-К
	*/
	int level_amount_;


	// Для быстрого преобразования Фурье
	/*!
	* @brief ФПС входного видео (две ширины полосы частот вибрации, которые можно отследить)
	*/
	int fps_;
	/*!
	* @brief Текущее время кадра в мс (таймкод)
	*/
	double frame_time_;
};

#endif