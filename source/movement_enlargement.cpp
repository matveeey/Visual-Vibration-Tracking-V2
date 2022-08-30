#include "MovEn/movement_enlargement.h"

MovementEnlarger::MovementEnlarger(std::string input_file_name, std::string output_file_name) :
	input_file_name_{ input_file_name },
	output_file_name_{ output_file_name },
	win_name_{ "Movement Enlarger" }
	
{
	std::cout << "Movement Enlarger constructor" << std::endl;
}

MovementEnlarger::~MovementEnlarger()
{
	std::cout << "Movement Enlarger destructor" << std::endl;
}

void MovementEnlarger::ExecuteLaplaceEnlargement()
{
	FrameHandler frame_handler(input_file_name_, output_file_name_, win_name_);
	int current_num_of_frame = 0;
	int amount_of_frames = frame_handler.GetAmountOfFrames();

	std::vector<Mat> input_pyramid;
	std::vector<Mat> motion_pyramid;
	while (current_num_of_frame < amount_of_frames)
	{
		frame_handler.ReadNextFrame();
		// обновляем номер кадра после чтения
		current_num_of_frame = frame_handler.GetCurrentPosOfFrame();
		frame_to_be_enlarged_ = frame_handler.GetCurrentFrame();

		// Конвертим цвет
		frame_to_be_enlarged_.convertTo(frame_to_be_enlarged_, CV_32FC3, 1.0 / 255.0f);
		cvtColor(frame_to_be_enlarged_, frame_to_be_enlarged_, cv::COLOR_BGR2YCrCb);

		// Строим пирамиду
		BuildLaplacePyramid(frame_to_be_enlarged_, input_pyramid, levels_amount_);

		// Если кадр первый - "инициализируем" его "нетронутой" пирамидой (до фильтрации)
		if (current_num_of_frame == 1)
		{
			low_pass_high_pyramid_ = input_pyramid;
			low_pass_low_pyramid_ = input_pyramid;
			motion_pyramid = input_pyramid;
			std::cout << "Current num of frame is 1" << std::endl;
			std::cout << frame_to_be_enlarged_.size() << std::endl;
		}
		else
		{
			for (int current_level = 0; current_level < levels_amount_; ++current_level)
			{
				IirFilter(input_pyramid[current_level], motion_pyramid[current_level], low_pass_high_pyramid_[current_level], low_pass_low_pyramid_[current_level], cutoff_low_, cutoff_high_);
			}

			int width = frame_handler.GetFrameWidth();
			int height = frame_handler.GetFrameHeight();

			delta_ = lambda_cutoff_ / (8.0 * (1.0 + alpha_));

			// 
			lambda_ = sqrt(width * width + height * height) / 3.0;

			// Увеличиваем движение на каждом уровне пирамиды
			for (int current_level = 0; current_level >= 0; --current_level)
			{
				LaplaceEnlarge(motion_pyramid[current_level], current_level);
				lambda_ /= 2.0;
				std::cout << "current lambda_ is " << lambda_ << " on level " << current_level << std::endl;
			}
		}

		// Теперь соберем изображения обратно из пирамиды
		std::cout << "is gonna to be build" << std::endl;
		BuildFromLaplacePyramid(motion_pyramid, frame_with_motion_info_, levels_amount_);
		std::cout << "built" << std::endl;

		// Приглушаем побежавшие цвета
		Attenuate(frame_with_motion_info_);
		std::cout << "attenuated" << std::endl;

		// Добавляем движение на изначальное изображение, исключая первый кадр
		if (current_num_of_frame > 1)
			frame_enlarged_ = frame_to_be_enlarged_ + frame_with_motion_info_;
		else
			frame_enlarged_ = frame_to_be_enlarged_;

		std::cout << "frame constructed" << std::endl;
		// Конвертим обратно
		cvtColor(frame_enlarged_, frame_enlarged_, COLOR_YCrCb2BGR);
		frame_enlarged_.convertTo(frame_enlarged_, CV_8UC3, 255.0, 1.0 / 255.0);

		// DEBUG
		std::cout << "frame " << current_num_of_frame << " is processed. The are also " << amount_of_frames - current_num_of_frame << " frames left. " << std::endl;

		std::cout << "CUM" << std::endl;
		frame_handler.ShowFrame(frame_enlarged_, false);
		frame_handler.WriteFrame(frame_enlarged_);

		int code = waitKey(20);
		switch (code)
		{
		// пауза (пробел)
		case 32:
		{
			waitKey(0);
			break;
		}
		case 'q':
		{
			current_num_of_frame = amount_of_frames;
			std::cout << frame_handler.GetInputCapStatus() << std::endl;
			break;
		}
		}
	}

}
