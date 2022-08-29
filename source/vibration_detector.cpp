#include "VVT-V2\vibration_detector.h"

VibrationDetector::VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name) :
	input_file_name_{ input_file_name },
	output_file_name_{ output_file_name },
	window_name_{ window_name },
	number_of_points_{ 0 },
	update_rate_{ 20 },
	warping_figure_selecting_{ false },
	max_amplitude_scalar_{ 0.0 },
	colored_point_mode_{ COLORING_BASED_ON_FREQUENCY },
	roi_selecting_{ false },
	point_id_{ 0 },
	current_mode_{ DEFAULT },
	fullscreen_{ false }
{
	roi_selected_ = false;
	lk_win_size_ = 60;
	level_amount_ = 1;
	point_offset_ = 100;

	frame_handler = new FrameHandler(input_file_name_, output_file_name_, MAIN_WINDOW_NAME);

	res_mp_ = 1.0;//= frame_handler->GetResizingCoefficient();
}

VibrationDetector::~VibrationDetector()
{
	delete frame_handler;
	DeleteColoredPoints();
	while (!vec_lonely_point_handlers_.empty())
	{
		delete(*(std::begin(vec_lonely_point_handlers_)));
		// удаляем первую точку (первую в векторе и по сути первую по номеру)
		vec_lonely_point_handlers_.erase(std::begin(vec_lonely_point_handlers_));
	}
}

void VibrationDetector::ServeTheQueues()
{
	for (int i = 0; i < l_click_queue_.size(); i++)
	{
		LeftClickHandler(l_click_queue_[i]);
	}
	for (int i = 0; i < c_point_queue_.size(); i++)
	{
		CreateNewColoredPoint(c_point_queue_[i]);
	}
	for (int i = 0; i < delete_queue_.size(); i++)
	{
		DeletePoints(delete_queue_[i]);
	}
	l_click_queue_.clear();
	c_point_queue_.clear();
	delete_queue_.clear();
}

void VibrationDetector::CreateNewPoint(Point2f mouse_coordinates)
{
	LonelyPointHandler* point_handler_ = new LonelyPointHandler(mouse_coordinates, update_rate_, fps_, point_id_++, res_mp_);
	vec_lonely_point_handlers_.push_back(point_handler_);
}

void VibrationDetector::LeftClickHandler(Point2f mouse_coordinates)
{
	// если вектор пустой - просто добавляем новую точку
	if (vec_lonely_point_handlers_.empty())
	{
		bool flag_colored_interacted_ = false;
		for (int i = 0; i < vec_colored_point_handlers_.size(); i++)
		{
			if (vec_colored_point_handlers_[i]->VibratingPoint::IsInteracted(mouse_coordinates))
			{
				vec_colored_point_handlers_[i]->SetHistogramFlag(true);
				flag_colored_interacted_ = true;
			}
		}
		if (flag_colored_interacted_ == false)
			CreateNewPoint(mouse_coordinates);
	}
	else
	{
		// проходим по всему вектору точек
		bool flag_lonely_interacted_ = false;
		bool flag_colored_interacted_ = false;
		for (int i = 0; i < vec_lonely_point_handlers_.size(); i++)
		{
			// если произошло пересечение с мышкой, то новую точку не добавляем
			if (vec_lonely_point_handlers_[i]->VibratingPoint::IsInteracted(mouse_coordinates))
			{
				vec_lonely_point_handlers_[i]->SetHistogramFlag(true);
				//vec_lonely_point_handlers_[i]->SetHistogramWindowProperty(WND_PROP_TOPMOST, true);
				flag_lonely_interacted_ = true;
			}
		}
		for (int i = 0; i < vec_colored_point_handlers_.size(); i++)
		{
			if (vec_colored_point_handlers_[i]->VibratingPoint::IsInteracted(mouse_coordinates))
			{
				vec_colored_point_handlers_[i]->SetHistogramFlag(true);
				flag_colored_interacted_ = true;
			}
		}
		if (flag_lonely_interacted_ == false && flag_colored_interacted_ == false)
			CreateNewPoint(mouse_coordinates);
	}
}

void VibrationDetector::CreateNewColoredPoint(Point2f mouse_coordinates)
{
	ColoredPointHandler* point_handler_ = new ColoredPointHandler(mouse_coordinates, update_rate_, fps_, point_id_++, res_mp_);
	vec_colored_point_handlers_.push_back(point_handler_);
}

void VibrationDetector::DeletePoints(Point2i mouse_coordinates)
{
	// создаем вектор ID (номеров) для точек, которые удалим
	std::vector<int> point_ids_to_be_deleted_;

	// проходим по всему вектору точек
	for (int i = 0; i < vec_lonely_point_handlers_.size(); i++)
	{
		// если произошло пересечение с мышкой, вносим ID (номер) точки в "вектор точек на удаление"
		if (vec_lonely_point_handlers_[i]->VibratingPoint::IsInteracted(mouse_coordinates))
		{
			point_ids_to_be_deleted_.push_back(i);
		}
	}
	std::cout << "DEBUG: deleting "<< point_ids_to_be_deleted_.size() << " point" << std::endl;

	std::vector<LonelyPointHandler*>::iterator it = vec_lonely_point_handlers_.begin();

	// проходимся по вектору точек на удаление
	while (!point_ids_to_be_deleted_.empty())
	{
		delete(*(std::begin(vec_lonely_point_handlers_) + point_ids_to_be_deleted_[0]));
		// удаляем первую точку (первую в векторе и по сути первую по номеру)
		vec_lonely_point_handlers_.erase(std::begin(vec_lonely_point_handlers_) + point_ids_to_be_deleted_[0]);
		// удаляем номер только что удалённой точки из вектора
		point_ids_to_be_deleted_.erase(std::begin(point_ids_to_be_deleted_));
		// меняем номера других точек в списке, если там ещё что-то есть
		for (int i = 0; i < point_ids_to_be_deleted_.size(); i++)
		{
			// уменьшаем номер оставшихся в очереди на удаление точек на один, тк номера в vec_point_handlers сместились на один вниз (влево и тп, как удобнее для понимания :))
			point_ids_to_be_deleted_[i] = point_ids_to_be_deleted_[i] - 1;
		}
	}
}

void VibrationDetector::DeleteColoredPoints()
{
	while (!vec_colored_point_handlers_.empty())
	{
		delete(*(std::begin(vec_colored_point_handlers_)));
		// удаляем первую точку (первую в векторе и по сути первую по номеру)
		vec_colored_point_handlers_.erase(std::begin(vec_colored_point_handlers_));
	}
}

// callback function for determining the event click on mouse
void VibrationDetector::OnMouse(int event, int x, int y, int flags, void* userdata)
{
	VibrationDetector* d = static_cast<VibrationDetector*>(userdata);
	d->DetectEvent(event, x, y, flags);
}

// "helper" function for implementing callback function as a method of C++ class
void VibrationDetector::DetectEvent(int event, int x, int y, int flags)
{
	Point tmp_container = TranslateCoordinates(Point2f(x, y));
	x = tmp_container.x;
	y = tmp_container.y;

	switch (current_mode_)
	{
	case DEFAULT:
	{
		DefaultModeHandler(event, x, y);
		break;
	}
	case SELECTINGROI:
	{
		RoiSelectionModeHandler(event, x, y);
		break;
	}
	case PAUSE:
	{
		break;
	}
	}
}

void VibrationDetector::DefaultModeHandler(int event, int x, int y)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
	{
		// Проверяем флаги выделения ROI и фигуры для варпа
		if (!roi_selecting_)
		{
			l_click_queue_.push_back(Point2i(x, y));
		}
		break;
	}
	case EVENT_RBUTTONDOWN:
	{
		if (!roi_selecting_ && !warping_figure_selecting_)
			delete_queue_.push_back(Point2i(x, y));
		break;
	}
	case EVENT_MOUSEMOVE:
	{
		// Координаты последнего перемещения мыши
		last_mouse_position_.x = x;
		last_mouse_position_.y = y;
	}
	}
}
void VibrationDetector::RoiSelectionModeHandler(int event, int x, int y)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
	{
		// Удаляем старые "цветные" точки
		DeleteColoredPoints();
		// Делаем флаг для выделения ROI активным
		roi_selecting_ = true;
		// Сохраняем координаты клика как одну из границ ROI
		tl_click_coords_.x = x;
		tl_click_coords_.y = y;
		break;
	}
	case EVENT_LBUTTONUP:
	{
		if (roi_selecting_)
		{
			// Отключаем флаг для выделения ROI
			roi_selecting_ = false;
			roi_selected_ = true;
			// Сохраняем последние позиции
			last_mouse_position_.x = x;
			last_mouse_position_.y = y;
		}
		break;
	}
	case EVENT_MOUSEMOVE:
	{
		// Координаты последнего перемещения мыши
		last_mouse_position_.x = x;
		last_mouse_position_.y = y;
		break;
	}
	}
}

Mat VibrationDetector::MakeWarpedFrame(Mat frame, std::vector<Point2i> warping_figure)
{
	int coeff = 1;

	Point2f src[4] = { warping_figure[0], warping_figure[1], warping_figure[2], warping_figure[3] };

	for (int i = 0; i < 4; i++)
	{
		src[i].x = src[i].x * coeff;
		src[i].y = src[i].y * coeff;
		warping_figure[i].x = warping_figure[i].x * coeff;
		warping_figure[i].y = warping_figure[i].y * coeff;
	}

	/*float w =
		sqrt((warping_figure[2].x - warping_figure[3].x) * (warping_figure[2].x - warping_figure[3].x)
		+
		(warping_figure[2].y - warping_figure[3].y) * (warping_figure[2].y - warping_figure[3].y)) * 2 / 3;*/
	float h =
		sqrt((warping_figure[1].x - warping_figure[3].x) * (warping_figure[1].x - warping_figure[3].x)
		+
		(warping_figure[1].y - warping_figure[3].y) * (warping_figure[1].y - warping_figure[3].y));
	float w = h * 2 / 3;

	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(frame, frame, matrix, Point(w, h));

	return frame;
}

void VibrationDetector::TrackAndCalc()
{
	std::vector<Point2f> PrevPts;
	std::vector<Point2f> NextPts;
	std::vector<uchar> status;
	std::vector<float> error;
	double amplitude_scalar = 0;

	// "Достаем" из lonely point handler'ов последние найденные точки, чтобы использовать их в качестве "начальных" значений для calcOpticalFlowPyrLK()
	for (int i = 0; i < vec_lonely_point_handlers_.size(); i++)
	{
		PrevPts.push_back(vec_lonely_point_handlers_[i]->GetLastFoundCoordinates());
	}
	// "Достаем" из colored point handler'ов последние найденные точки, чтобы использовать их в качестве "начальных" значений для calcOpticalFlowPyrLK()
	for (int i = 0; i < vec_colored_point_handlers_.size(); i++)
	{
		// Определяем самую большую амплитуду на экране в данный момент
		amplitude_scalar = sqrt(current_amplitude_.x * current_amplitude_.x + current_amplitude_.y * current_amplitude_.y);
		current_amplitude_ = vec_colored_point_handlers_[i]->GetCurrentAmplitude();
		if (amplitude_scalar < sqrt(current_amplitude_.x * current_amplitude_.x + current_amplitude_.y * current_amplitude_.y))
			max_amplitude_scalar_ = sqrt(current_amplitude_.x * current_amplitude_.x + current_amplitude_.y * current_amplitude_.y);

		PrevPts.push_back(vec_colored_point_handlers_[i]->GetLastFoundCoordinates());
	}

	if ((!vec_lonely_point_handlers_.empty()) || (!vec_colored_point_handlers_.empty()))
		// вызов Lucas-Kanade алгоритма
		calcOpticalFlowPyrLK(
			prev_img_gray_,
			next_img_gray_,
			PrevPts,
			NextPts,
			status,
			error,
			Size(lk_win_size_, lk_win_size_),
			level_amount_,
			TermCriteria(
				TermCriteria::MAX_ITER | TermCriteria::EPS,
				500,
				0.001
			),
			OPTFLOW_LK_GET_MIN_EIGENVALS
		);

	// проверяем равны ли размеры векторов хэндлеров точек и найденных точек на картинке
	if (NextPts.size() != (vec_lonely_point_handlers_.size() + vec_colored_point_handlers_.size()))
	{
		std::cout << "DEBUG: ERROR - NextPts size doesnt match vec_point_handlers_ size" << std::endl;
		return;
	}

	// Закидываем найденные значения обратно в point handler'ы
	// Сначала идёт lonely
	for (int i = 0; i < vec_lonely_point_handlers_.size(); i++)
	{
		vec_lonely_point_handlers_[i]->AddNewPointPosition(NextPts[i]);
		vec_lonely_point_handlers_[i]->AddNewPointTime(frame_time_);
		// Вызов БПФ (FFT)
		vec_lonely_point_handlers_[i]->ExecuteFFT();
	}
	// После colored
	for (int i = vec_lonely_point_handlers_.size(); i < vec_colored_point_handlers_.size(); i++)
	{
		vec_colored_point_handlers_[i]->AddNewPointPosition(NextPts[i]);
		vec_colored_point_handlers_[i]->AddNewPointTime(frame_time_);
		// Вызов БПФ (FFT)
		vec_colored_point_handlers_[i]->ExecuteFFT();
	}
}

std::vector<Point2f> VibrationDetector::FindGoodFeatures(Mat frame, Rect roi)
{
	std::vector<Point2f> good_features;
	// Если изображение не черно-белое
	if (frame.channels() > 2)
		cvtColor(frame, frame, COLOR_BGR2GRAY);
	// Устанавливаем ROI на нашем изображении
	Mat frame_with_roi = frame(roi);
	goodFeaturesToTrack(frame_with_roi, good_features, 50, 0.01, 3, noArray());

	// Перевод в координаты изначального изображения
	for (int i = 0; i < good_features.size(); i++)
	{
		good_features[i].x = good_features[i].x + roi.tl().x;
		good_features[i].y = good_features[i].y + roi.tl().y;
	}

	return good_features;
}

Point2f VibrationDetector::TranslateCoordinates(Point2f point)
{
	return Point2f(point.x / res_mp_, point.y / res_mp_);
}

void VibrationDetector::DrawAndOutput(Mat& frame)
{
	if (roi_selected_)
	{
		frame = frame_handler->ConcatenateFramesVertically(frame, grad_scale_);
	}
	// Масштабируем кадр для вывода на экран и добавляем на него подсказки для пользователя
	frame.copyTo(current_tracking_frame_resized_);
	//current_tracking_frame_resized_ = frame_handler->ResizeFrame(current_tracking_frame_resized_);
	current_tracking_frame_resized_ = frame_handler->AddTips(current_tracking_frame_resized_, current_mode_);
	// Выводим кадр на экран
	frame_handler->ShowFrame(current_tracking_frame_resized_, fullscreen_);
}

// DEBUG
void VibrationDetector::DrawDebugLkWinRectangle(Mat& frame)
{

	for (int i = 0; i < vec_lonely_point_handlers_.size(); i++)
	{
		// DEBUG
		Rect debug_lk_win_size = Rect(
			Point2i(vec_lonely_point_handlers_[i]->GetLastFoundCoordinates().x - lk_win_size_, vec_lonely_point_handlers_[i]->GetLastFoundCoordinates().y - lk_win_size_),
			Point2i(vec_lonely_point_handlers_[i]->GetLastFoundCoordinates().x + lk_win_size_, vec_lonely_point_handlers_[i]->GetLastFoundCoordinates().y + lk_win_size_)
		);
		rectangle(frame, debug_lk_win_size, Scalar(0, 0, 255), 1);
	}
}

void VibrationDetector::ExecuteVibrationDetection()
{
	// reading the first frame of sequence so we can convert it to gray color space
	frame_handler->ReadNextFrame();
	current_tracking_frame_ = frame_handler->GetCurrentFrame();

	prev_img_gray_ = frame_handler->GetGrayFrame(current_tracking_frame_);
	fps_ = frame_handler->GetInputFps();

	// Генерируем шкалу
	grad_scale_ = frame_handler->GenerateGradScale(0, fps_ / 2, colored_point_mode_);

	// conditions of exit
	running_ = true;
	int current_num_of_frame = 0;
	int amount_of_frames = frame_handler->GetAmountOfFrames();

	// устанавливаем callback handler на наше окно
	setMouseCallback(frame_handler->GetWindowName(), OnMouse, (void*)this);

	while ((current_num_of_frame < amount_of_frames - 1) && running_ == true)
	{
		current_num_of_frame = frame_handler->GetCurrentPosOfFrame();

		// reading next frame and converting it to gray color space
		frame_handler->ReadNextFrame();

		current_tracking_frame_ = frame_handler->GetCurrentFrame();
		if (!warping_figure_.empty())
		{
			current_tracking_frame_ = MakeWarpedFrame(current_tracking_frame_, warping_figure_);
		}

		next_img_gray_ = frame_handler->GetGrayFrame(current_tracking_frame_);

		// Трекинг и вычисление частоты вибрации
		frame_time_ = frame_handler->GetCurrentTimeOfFrame();
		TrackAndCalc();

		// Рисование точек, треков и данных (вибрации, амплитуды и т.п.)
		for (int i = 0; i < vec_lonely_point_handlers_.size(); i++)
		{
			vec_lonely_point_handlers_[i]->VibratingPoint::IsInteracted(last_mouse_position_);
			vec_lonely_point_handlers_[i]->Draw(current_tracking_frame_);
		}
		for (int i = 0; i < vec_colored_point_handlers_.size(); i++)
		{
			// Обновляем максимальную амплитуду
			vec_colored_point_handlers_[i]->SetMaxAmplitude(max_amplitude_scalar_);
			vec_colored_point_handlers_[i]->VibratingPoint::IsInteracted(last_mouse_position_);
			vec_colored_point_handlers_[i]->SetMode(colored_point_mode_);
			vec_colored_point_handlers_[i]->Draw(current_tracking_frame_);
		}

		// обслуживаем и очищаем очереди на создание и удаление точек
		ServeTheQueues();

		// Записываем изначальный (не ресайзнутый)
		frame_handler->WriteFrame(current_tracking_frame_);
		
		DrawAndOutput(current_tracking_frame_);

		// Обновляем предыдущий кадр
		prev_img_gray_ = next_img_gray_;

		// 20 - задержка в мс
		int code = waitKey(20);
		switch (code)
		{
		// Пауза
		// Клавиши - пробел (ASCII code)
		case 32:
		{
			current_mode_ = PAUSE;
			
			DrawAndOutput(current_tracking_frame_);

			waitKey(0);
			current_mode_ = DEFAULT;
			break;
		}
		case 'c':
		{
			Mat unchanged_frame = current_tracking_frame_;
			frame_handler->ShowFrame(current_tracking_frame_, fullscreen_);
			
			warping_figure_.clear();
			warping_figure_selecting_ = true;

			while (warping_figure_selecting_)
			{		
				for (int i = 0; i < warping_figure_.size(); i++)
				{
					circle(unchanged_frame, warping_figure_[i], 10, Scalar(255, 207, 64), 2);
				}
		
				if (warping_figure_.size() == 4)
					warping_figure_selecting_ = false;
		
				frame_handler->ShowFrame(unchanged_frame, fullscreen_);
				waitKey(20);
			}
			break;
		}
		// Фуллскрин. 
		// Клавиши - "F" или "f" (ASCII code)
		case 102:
		case 70:
		{
			// Проверка на флаг полноэкранного режима. Если он изначально был false, меняем на true, и наоборот в противном случае
			if (!fullscreen_)
			{
				fullscreen_ = true;
			}
			else
			{
				fullscreen_ = false;
			}
			break;
		}
		// Изменение режима подсветки точек при активном регионе интереса (ROI)
		// Клавиши - "M" или "m" (ASCII code)
		case 77:
		case 109:
		{
			// Перебираем режимы по порядку
			if (colored_point_mode_ == 2)
				colored_point_mode_ = 0;
			else
				colored_point_mode_++;

			grad_scale_ = frame_handler->GenerateGradScale(0, fps_ / 2, colored_point_mode_);
			break;
		}
		// Выделение региона интереса (ROI)
		// Клавиши - "R" или "r" (ASCII code)
		case 82:
		case 114:
		{
			// Обновляем текущий режим
			current_mode_ = SELECTINGROI;
			// Регион интереса
			Rect roi;
			// Сбрасываеи флаг выделенного региона интереса
			roi_selected_ = false;
			// Флаг выхода из режима
			bool wanna_quit = false;

			while (!roi_selected_ && !wanna_quit)
			{
				// создаю копию current_tracking_frame_
				Mat frame = current_tracking_frame_.clone();
				
				// Пока происходит выделение региона интереса (в нашем случае пока не была отпущена ЛКМ), отрисовываем прямоугольник
				if (roi_selecting_)
				{
					roi = Rect(tl_click_coords_, last_mouse_position_);
					rectangle(frame, roi, Scalar(0, 255, 0), 1 * (1 / res_mp_));
				}

				DrawAndOutput(frame);

				switch (waitKey(20))
				{
				// R for reset
				case 'r':
				{
					wanna_quit = true;
					// Удаляем старые "цветные" точки
					DeleteColoredPoints();
					break;
				}
				// Esc
				case 27:
				{
					wanna_quit = true;
					break;
				}
				}
			}
			// ROI может быть пустым после выхода из цикла выше кейсом в свитче
			if (!roi.empty())
				c_point_queue_ = FindGoodFeatures(current_tracking_frame_, roi);

			current_mode_ = DEFAULT;
			break;
		}
		case 'q':
		{
			running_ = false;
			std::cout << frame_handler->GetInputCapStatus() << std::endl;
			break;
		}
		}
	}
}