#include "MovEn/eulerian_enlargement.h"

EulerEnlarger::EulerEnlarger() :
	cutoff_low_{ 0.05 },
	cutoff_high_{ 0.95 },
	exaggeraiton_coefficient_{ 0.5 },
	lambda_{ 0.0 }, // будем изменено в цикле
	lambda_cutoff_{ 0.0 },
	color_attenuation_coefficient_{ 0.0 },
	delta_{ 0.0 },
	alpha_{ 0.8 },
	levels_amount_{ 3 }
{
	std::cout << "Euler Enlarger constructor" << std::endl;
}

EulerEnlarger::~EulerEnlarger()
{
	std::cout << "Euler Enlarger destructor" << std::endl;
}

void EulerEnlarger::LaplaceEnlarge(Mat& motion_pyramid_level, int current_level)
{
	float current_alpha = (lambda_ / (delta_ / 8.0) - 1.0) * exaggeraiton_coefficient_;

	if (current_level == levels_amount_ || current_level == 0)
	{
		// Для первого и последнего уровней пирамиды устанавливаем усиление на 0
		motion_pyramid_level = motion_pyramid_level * 0;
	}
	else
	{
		// Выполняем для остальных уровней
		motion_pyramid_level = motion_pyramid_level * std::min(current_alpha, alpha_);
	}
}

void EulerEnlarger::Attenuate(Mat& motion_frame)
{
	// "Приглушение" будет происходить только для цветных кадров
	if (motion_frame.channels() > 2)
	{
		//std::vector<Mat> planes;
		Mat planes[3];
		split(motion_frame, planes);
		// Применяем только к 2-му и 3-му каналу, тк первый отвечает за ч-б составляющую картинки в нашем формате
		planes[1] = planes[1] * color_attenuation_coefficient_;
		planes[2] = planes[2] * color_attenuation_coefficient_;
		merge(planes, 3, motion_frame);
	}
}

void EulerEnlarger::BuildLaplacePyramid(Mat& input_frame, std::vector<Mat>& input_pyramid, int levels)
{
	std::vector<Mat> pyramid;
	Mat current_level = input_frame.clone();

	for (int level = 0; level < levels; ++level)
	{
		Mat down, up;
		// Блюрим кадр и уменьшаем его размер по ширине и высоте в два раза
		pyrDown(current_level, down);
		// Увеличиваем изображение по ширине и высоте в два раза, затем блюрим его
		pyrUp(down, up, current_level.size());
		Mat laplace = current_level - up;
		pyramid.push_back(laplace);
		current_level = down;
	}
	pyramid.push_back(current_level);
	std::cout << "BuildLaplacePyramid::pyramid.size() is " << pyramid.size() << std::endl;
	input_pyramid = pyramid;
}

void EulerEnlarger::IirFilter(Mat& input_pyramid_level, Mat& motion_pyramid_level, Mat& low_pass_high_level, Mat& low_pass_low_level, double cutoff_low, double cutoff_high)
{
	low_pass_high_level = (1 - cutoff_high) * low_pass_high_level + cutoff_high * input_pyramid_level;
	low_pass_low_level = (1 - cutoff_low) * low_pass_low_level + cutoff_low * input_pyramid_level;

	motion_pyramid_level = low_pass_high_level - low_pass_low_level;
}

void EulerEnlarger::BuildFromLaplacePyramid(std::vector<Mat>& motion_pyramid, Mat& motion_frame, const int levels)
{
	// Сначала рассматриваем последний элемент вектора - он же первый уровень пирамиды
	std::cout << "motion_pyramid size " << motion_pyramid.size() << " and levels is " << levels << std::endl;
	Mat current_level = motion_pyramid[levels];

	for (int level = levels - 1; level >= 0; --level)
	{
		std::cout << "current level is " << level << std::endl;
		Mat up;
		std::cout << "PyrUP IS GOING" << std::endl;
		pyrUp(current_level, up, motion_pyramid[level].size());
		std::cout << "PyrUP IS DONE" << std::endl;
		// Добавляем только что заапсемпленный уровень к "текущему" (по сути предыдущему) уровню пирамиды
		current_level = up + motion_pyramid[level];
	}
	std::cout << "Built" << std::endl;
	motion_frame = current_level.clone();
}
