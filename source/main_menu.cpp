#include "GUI/main_menu.h"

MainMenu::MainMenu(std::string params_file_name) :
	params_file_name_{ params_file_name },
	winname_{ MAIN_MENU_WINDOW_NAME },
	menu_width_{ 500 },
	menu_height_{ 500 },
	button_amount_{ 4 },
	mode_chosen_{ false }
{
	// Загружаем названия директорий
	LoadPaths();
	namedWindow(winname_, WINDOW_AUTOSIZE);
	// Инициализируем отступы
	int x = 20;
	button_spacing_ = x;
	top_offset_ = 3 * x;
	bottom_offset_ = 5 * x;
	width_offset_ = 3 * x;
	button_width_ = menu_width_ - 2 * width_offset_;
	button_height_ = (menu_height_ - (button_amount_ - 1) * button_spacing_ - top_offset_ - bottom_offset_) / button_amount_;
	// Надписи на кнопках
	button_text_[0] = "CALIBRATION";
	button_text_[1] = "UNDISTORTION";
	button_text_[2] = "VIBRATION MONITORING";
	button_text_[3] = "MOVEMENT ENLARGEMENT";
	// Инициализируем контейнер цветов кнопок
	for (int i = 0; i < button_amount_; i++)
	{
		buttons_color_.push_back(Scalar(255, 255, 255));
	}
	// Инициализируем атрибуты текста
	font = FONT_HERSHEY_PLAIN;
	font_scale = 1;
	thickness = 2;
	baseline = 0;
	// Создание меню на Mat-объекте
	CreateMenu();
}

void MainMenu::LoadPaths()
{
	std::ifstream params_file;
	params_file.open(params_file_name_);

	if (!params_file.is_open())
	{
		std::cout << "Wrong filename" << std::endl;
	}

	std::getline(params_file, input_calibration_video_);
	std::getline(params_file, input_distorted_video_);
	std::getline(params_file, output_undistorted_video_);
	std::getline(params_file, input_vibration_detection_);
	std::getline(params_file, output_vibration_detection_);
	std::getline(params_file, chessboards_path_);
	std::getline(params_file, input_movement_enlargment_);
	std::getline(params_file, output_movement_enlargement_);
}

void MainMenu::ExecuteCameraCalibration(std::string input_file_name, std::string chessboards_path)
{
	CameraCalibrator camera_calibrator(input_file_name, chessboards_path);
	camera_calibrator.ExecuteCameraCalibration();
}

void MainMenu::ExecuteVideoUndistortion(std::string input_file_name, std::string output_file_name)
{
	std::string params;

	std::cout << "Please attach the parameters.txt file: ";
	std::cin >> params;
	std::cout << std::endl;

	VideoUndistorter video_undistorter(input_file_name, output_file_name, params);
	video_undistorter.ExecuteVideoUndistortion();
}

void MainMenu::ExecuteVibrationMonitoring(std::string input_file_name, std::string output_file_name)
{
	VibrationDetector vibration_detector(input_file_name, output_file_name, MAIN_WINDOW_NAME);
	vibration_detector.ExecuteVibrationDetection();
}

void MainMenu::ExecuteMotionMagnification(std::string input_file_name, std::string output_file_name)
{
	MovementEnlarger movement_enlarger(input_file_name, output_file_name);
	//movement_enlarger.ExecuteLaplaceEnlargement();
	movement_enlarger.ExecuteRieszEnlargement();
}

void MainMenu::CreateMenu()
{
	// Создадим кнопки в соответствии с их позициями
	for (int i = 0; i < button_amount_; i++)
	{
		buttons_.push_back
		(
			Rect(
				Point2i(
					width_offset_,
					top_offset_ + i * (button_height_ + button_spacing_)
				),
				Point2i(
					width_offset_ + button_width_,
					top_offset_ + button_height_ + i * (button_height_ + button_spacing_)
				)
			)
		);
	}
	
	menu_ = Mat(Size(menu_width_, menu_height_), CV_8UC3, Scalar(0, 0, 0));	

	// Подсказка для пользователя
	std::string tip = "Press 'Q' to quit";
	putText(
		menu_,
		tip,
		Point(
			(menu_width_ - getTextSize(tip, font, font_scale, thickness, &baseline).width) / 2,
			menu_height_ - (bottom_offset_ - getTextSize(tip, font, font_scale, thickness, &baseline).height) / 2),
		font,
		font_scale,
		Scalar(75, 75, 75),
		thickness / 2,
		4
	);

	Idle();
}

void MainMenu::UpdateBtnColorAndText()
{
	for (int i = 0; i < button_amount_; i++)
	{
		// Рисуем очертания кнопки
		rectangle(menu_, buttons_[i], buttons_color_[i], FILLED);
		// Размеры текста в пикселях
		Size current_text_size = getTextSize(button_text_[i], font, font_scale, thickness, &baseline);
		// Координаты текущего текста
		Point text_coordinates = Point(
			buttons_[i].tl().x + button_width_ / 2 - current_text_size.width / 2,
			buttons_[i].tl().y + button_height_ / 2 + (current_text_size.height) / 2);
		// Рисуем текст кнопки
		putText(
			menu_,
			button_text_[i],
			text_coordinates,
			font,
			font_scale,
			Scalar(255, 255, 255),
			thickness,
			4 // тип линии. вроде отличаются сглаживанием. выбрал 4-й просто так :)
		);

	}
}

void MainMenu::OnMouse(int event, int x, int y, int flags, void* userdata)
{
	MainMenu* d = static_cast<MainMenu*>(userdata);
	d->DetectEvent(event, x, y, flags);
}

void MainMenu::DetectEvent(int event, int x, int y, int flags)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
	{
		for (int i = 0; i < buttons_.size(); i++)
		{
			// Если клик пользователя был по кнопке
			if (buttons_[i].contains(Point(x, y)))
			{
				switch (i)
				{
				case CALIBRATION:
				{
					std::cout << "u've chosen camera calibration" << std::endl;
					ExecuteCameraCalibration(input_calibration_video_, chessboards_path_);
					break;
				}
				case UNDISTORTION:
				{
					std::cout << "u've chosen undistortion" << std::endl;
					ExecuteVideoUndistortion(input_distorted_video_, output_undistorted_video_);
					break;
				}
				case VIBRATION_MONITORING:
				{
					std::cout << "u've chosen vibration monitoring" << std::endl;
					ExecuteVibrationMonitoring(input_vibration_detection_, output_vibration_detection_);
					break;
				}
				case MOVEMENT_ENLARGEMENT:
				{
					std::cout << "u've chosen movement enlarging" << std::endl;
					ExecuteMotionMagnification(input_movement_enlargment_, output_movement_enlargement_);
					break;
				}
				}
			}
		}
		break;
	}
	case EVENT_MOUSEMOVE:
	{
		last_mouse_position_.x = x;
		last_mouse_position_.y = y;
		break;
	}
	}
}

void MainMenu::Idle()
{
	// Устанавливаем хэндлер окна
	setMouseCallback(winname_, OnMouse, (void*)this);

	while (!mode_chosen_)
	{
		for (int i = 0; i < buttons_.size(); i++)
		{
			if (buttons_[i].contains(last_mouse_position_))
			{
				buttons_color_[i] = Scalar(0, 0, 255);
			}
			else buttons_color_[i] = Scalar(25, 25, 25);
		}
		// Обновляем текущие цвета кнопок
		UpdateBtnColorAndText();

		imshow(winname_, menu_);
		switch (waitKey(10))
		{
		case 'q':
		{
			mode_chosen_ = true;
			break;
		}
		}
	}
}
