#ifndef EULERIAN_ENLARGEMENT_H
#define EULERIAN_ENLARGEMENT_H

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

/*!
* @brief Класс для визуального увеличения вибрации на видео
*/
class EulerEnlarger
{
protected:
	EulerEnlarger();
	~EulerEnlarger();

protected:
	/*!
	* @brief Осуществляет увеличение движения на конкретном уровне пирамиды
	*/
	void LaplaceEnlarge(Mat& motion_pyramid_level, int current_level);
	/*!
	* @brief "Приглушает" усиленные цвета на кадре
	*/
	void Attenuate(Mat& motion_frame);

	// Вход и выход из пирамид. Фильтры
	/*!
	* @brief Создает пирамиду изображений
	*/
	void BuildLaplacePyramid(const Mat& input_frame, std::vector<Mat>& input_pyramid, const int levels);
	/*!
	* @brief Фильтр для отсекания низких и высоких "пространственных" частот ("фильтр с бесконечной импульсной характеристикой" - infinite impulse response)
	* 
	* Чем больше cutoff (пороговое значение), тем быстрее исчезает "низкочастотные" области изображения из "низкочастотной" пирамиды
	* => длительные (соответственно большие) движения быстрее исчезают
	*/
	void IirFilter(Mat& input_pyramid_level, Mat& motion_pyramid_level, Mat& low_pass_high_level, Mat& low_pass_low_level, double cutoff_low, double cutoff_high);
	/*!
	* @brief Извлекаем выходное изображение из пирамиды Лапласа
	*/
	void BuildFromLaplacePyramid(std::vector<Mat>& motion_pyramid, Mat& motion_frame, int levels);

protected:
	// lowpass 
	std::vector<Mat> low_pass_high_pyramid_;
	std::vector<Mat> low_pass_low_pyramid_;
	// 
	double cutoff_low_;
	double cutoff_high_;

	/*!
	* @brief Количество уровней (слоев) пирамиды
	*/
	int levels_amount_;
	/*!
	* @brief Коэффициент увеличения. Чем выше - тем больше шума, тем сильнее увеличиваются небольшие движения.
	*/
	double exaggeraiton_coefficient_;
	/*!
	* @brief Показательная длина волны
	*/
	double lambda_;
	double lambda_cutoff_;
	/*!
	* @brief Коэффициент приглушеняи цвета
	*/
	double color_attenuation_coefficient_;
	/*!
	* @brief Коэффициент, использующийся при увеличении
	*/
	double delta_;

	double alpha_;
};

#endif