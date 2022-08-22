#ifndef LAPLACE_ENLARGEMENT_H
#define LAPLACE_ENLARGEMENT_H

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

class LaplaceEnlarger
{
protected:
	LaplaceEnlarger();
	~LaplaceEnlarger();

protected:
	// Основные методы
	//
	// Осуществляет увеличение движения на конкретном уровне пирамиды
	void LaplaceEnlarge(Mat& motion_pyramid_level, int current_level);
	// "Приглушает" цвета на кадре
	void Attenuate(Mat& motion_frame);

	// Вход и выход из пирамид. Фильтры
	// 
	// Создает пирамиду изображений Лапласа
	void BuildLaplacePyramid(Mat& input_frame, std::vector<Mat>& input_pyramid, int levels);
	// Фильтр для отсекания низких и высоких "пространственных" частот ("фильтр с бесконечной импульсной характеристикой" - infinite impulse response)
	// Чем больше cutoff (пороговое значение), тем быстрее исчезает "низкочастотные" области изображения из "низкочастотной" пирамиды
	// => длительные (соответственно большие) движения быстрее исчезают
	void IirFilter(Mat& input_pyramid_level, Mat& motion_pyramid_level, Mat& low_pass_high_level, Mat& low_pass_low_level, double cutoff_low, double cutoff_high);
	// Извлекаем выходное изображение из пирамиды Лапласа
	void BuildFromLaplacePyramid(std::vector<Mat>& motion_pyramid, Mat& motion_frame, int levels);

protected:
	// lowpass 
	std::vector<Mat> low_pass_high_pyramid_;
	std::vector<Mat> low_pass_low_pyramid_;
	// 
	double cutoff_low_;
	double cutoff_high_;

	// Количество уровней (слоев) пирамиды
	int levels_amount_;
	// Коэффициент увеличения. Чем выше - тем больше шума, тем сильнее увеличиваются небольшие движения.
	double exaggeraiton_coefficient_;
	// Показательная длина волны
	double lambda_;
	double lambda_cutoff_;
	// Коэффициент приглушеняи цвета
	double color_attenuation_coefficient_;
	// Коэффициент, использующийся при увеличении
	double delta_;

	float alpha_;
};

#endif