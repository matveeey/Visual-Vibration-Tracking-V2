#include "VVT-V2\vibration_detector.h"

VibrationDetector::VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name) :
	input_file_name_{ input_file_name },
	output_file_name_{ output_file_name },
	window_name_{ window_name },
	number_of_points_{ 0 },
	update_rate_{ 20 },
	warping_figure_selecting_{ false },
	roi_selecting_{ false },
	point_id_{ 0 }
{
	point_selected_ = false;
	lk_win_size_ = 20;
	level_amount_ = 1;
	point_offset_ = 100;

	frame_handler = new FrameHandler(input_file_name_, output_file_name_, MAIN_WINDOW_NAME);

	res_mp_ = frame_handler->GetResizingCoefficient();
}

VibrationDetector::~VibrationDetector()
{
	delete frame_handler;
}

void VibrationDetector::ServeTheQueues()
{
	for (int i = 0; i < l_click_queue_.size(); i++)
	{
		LeftClickHandler(l_click_queue_[i]);
	}
	for (int i = 0; i < delete_queue_.size(); i++)
	{
		DeletePoints(delete_queue_[i]);
	}
	l_click_queue_.clear();
	delete_queue_.clear();
}

void VibrationDetector::CreateNewPoint(Point2f mouse_coordinates)
{

	PointHandler* point_handler_ = new PointHandler(mouse_coordinates, update_rate_, fps_, point_id_++, res_mp_);
	vec_point_handlers_.push_back(point_handler_);

	std::cout << "DEBUG: creating new point" << std::endl;
}

void VibrationDetector::LeftClickHandler(Point2f mouse_coordinates)
{
	// если вектор пустой - просто добавляем новую точку
	if (vec_point_handlers_.empty())
	{
		CreateNewPoint(mouse_coordinates);
	}
	else
	{
		// проходим по всему вектору точек
		bool flag_interacted_ = false;
		for (int i = 0; i < vec_point_handlers_.size(); i++)
		{
			// если произошло пересечение с мышкой, то новую точку не добавляем
			if (vec_point_handlers_[i]->VibratingPoint::IsInteracted(mouse_coordinates))
			{
				vec_point_handlers_[i]->SetHistogramFlag(true);
				flag_interacted_ = true;
			}
		}
		if (!flag_interacted_)
			CreateNewPoint(mouse_coordinates);
	}
}

void VibrationDetector::DeletePoints(Point2i mouse_coordinates)
{
	// создаем вектор ID (номеров) для точек, которые удалим
	std::vector<int> point_ids_to_be_deleted_;

	// проходим по всему вектору точек
	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		// если произошло пересечение с мышкой, вносим ID (номер) точки в "вектор точек на удаление"
		if (vec_point_handlers_[i]->VibratingPoint::IsInteracted(mouse_coordinates))
		{
			point_ids_to_be_deleted_.push_back(i);
		}
	}
	std::cout << "DEBUG: deleting "<< point_ids_to_be_deleted_.size() << " point" << std::endl;

	std::vector<PointHandler*>::iterator it = vec_point_handlers_.begin();

	// проходимся по вектору точек на удаление
	while (!point_ids_to_be_deleted_.empty())
	{
		delete(*(std::begin(vec_point_handlers_) + point_ids_to_be_deleted_[0]));
		// удаляем первую точку (первую в векторе и по сути первую по номеру)
		vec_point_handlers_.erase(std::begin(vec_point_handlers_) + point_ids_to_be_deleted_[0]);
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

// callback function for determining the event click on mouse
void VibrationDetector::OnMouse(int event, int x, int y, int flags, void* userdata)
{
	VibrationDetector* d = static_cast<VibrationDetector*>(userdata);
	d->DetectEvent(event, x, y, flags);
}

// "helper" function for implementing callback function as a method of C++ class
void VibrationDetector::DetectEvent(int event, int x, int y, int flags)
{
	Point tmp = TranslateCoordinates(Point2f(x, y));
	x = tmp.x;
	y = tmp.y;

	switch (event) {
		// ЛКМ была прожата вниз
	case EVENT_LBUTTONDOWN:
	{
		if (!roi_selecting_ && !warping_figure_selecting_)
		{
			l_click_queue_.push_back(Point2i(x, y));
		}
		if (warping_figure_selecting_)
		{
			warping_figure_.push_back(Point2i(x, y));
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
		last_mouse_position_.x = x;
		last_mouse_position_.y = y;
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
//
//void VibrationDetector::SelectRoi(int event, int x, int y, int flags, void* userdata)
//{
//	VibrationDetector* b = static_cast<VibrationDetector*>(userdata);
//	b->DragDetect(event, x, y, flags);
//}
//
//void VibrationDetector::DragDetect(int event, int x, int y, int flags)
//{
//	switch (event)
//	{
//	case EVENT_RBUTTONDOWN:
//		// we need to initialize coordinates so if we select roi again, the default bottim right coordinates would be reset
//		this->right_button_down_ = true;
//		this->tl_click_coords_.x = x;
//		this->tl_click_coords_.y = y;
//		this->br_click_coords_.x = x;
//		this->br_click_coords_.y = y;
//		mouse_move_coords_.x = x;
//		mouse_move_coords_.y = y;
//		roi_ = Rect(tl_click_coords_, br_click_coords_);
//		std::cout << "rbuttondown" << std::endl;
//		break;
//	case EVENT_RBUTTONUP:
//		this->right_button_down_ = false;
//		this->rectangle_selected_ = true;
//		this->br_click_coords_.x = x;
//		this->br_click_coords_.y = y;
//		roi_ = Rect(tl_click_coords_, br_click_coords_);
//		break;
//	case EVENT_MOUSEMOVE:
//		if (right_button_down_)
//		{
//			mouse_move_coords_.x = x;
//			mouse_move_coords_.y = y;
//			roi_ = Rect(tl_click_coords_, mouse_move_coords_);
//		}
//		break;
//	}
//}
//
//void VibrationDetector::SelectFigure(int event, int x, int y, int flags, void* userdata)
//{
//	VibrationDetector* b = static_cast<VibrationDetector*>(userdata);
//	b->FigureMountDetect(event, x, y, flags);
//}
//
//void VibrationDetector::FigureMountDetect(int event, int x, int y, int flags)
//{
//	switch (event) {
//		// ЛКМ была прожата вниз
//	case EVENT_LBUTTONDOWN:
//	{
//		warping_figure_.push_back(Point(x, y));
//		break;
//	}
//	}
//}

void VibrationDetector::TrackAndCalc()
{
	std::vector<Point2f> PrevPts;
	std::vector<Point2f> NextPts;
	std::vector<uchar> status;
	std::vector<float> error;

	// "Достаем" из point handler'ов последние найденные точки, чтобы использовать их в качестве "начальных" значений для calcOpticalFlowPyrLK()
	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		PrevPts.push_back(vec_point_handlers_[i]->GetLastFoundCoordinates());
	}

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
	if (NextPts.size() != vec_point_handlers_.size())
	{
		std::cout << "DEBUG: ERROR - NextPts size doesnt match vec_point_handlers_ size" << std::endl;
		return;
	}
	
	// Сохраняем наши "предыдущие" точки для вывода на экран
	previous_points_coordinates_ = PrevPts;

	// Закидываем найденные значения обратно в point handler
	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		vec_point_handlers_[i]->AddNewPointPosition(NextPts[i]);
		vec_point_handlers_[i]->AddNewPointTime(frame_time_);
		// Вызов БПФ (FFT)
		vec_point_handlers_[i]->ExecuteFFT();
	}
}

Point2f VibrationDetector::TranslateCoordinates(Point2f point)
{
	return Point2f(point.x / res_mp_, point.y / res_mp_);
}

void VibrationDetector::DrawDebugLkWinRectangle(Mat& frame)
{

	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		// DEBUG
		Rect debug_lk_win_size = Rect(
			Point2i(vec_point_handlers_[i]->GetLastFoundCoordinates().x - lk_win_size_, vec_point_handlers_[i]->GetLastFoundCoordinates().y - lk_win_size_),
			Point2i(vec_point_handlers_[i]->GetLastFoundCoordinates().x + lk_win_size_, vec_point_handlers_[i]->GetLastFoundCoordinates().y + lk_win_size_)
		);
		rectangle(frame, debug_lk_win_size, Scalar(0, 0, 255), 1);
	}
}

void VibrationDetector::ExecuteVibrationDetection()
{
	// reading the first frame of sequence so we can convert it to gray color space
	frame_handler->ReadNextFrame();
	current_tracking_frame_ = frame_handler->GetCurrentFrame();

	/*if (warping_figure_selected_)
	{
		current_tracking_frame_ = GetWarpedFrame(current_tracking_frame_, warping_figure_, frame_processor.GetFrameWidth(), frame_processor.GetFrameHeight());
	}*/

	prev_img_gray_ = frame_handler->GetGrayFrame(current_tracking_frame_);
	fps_ = frame_handler->GetInputFps();

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

		if (!vec_point_handlers_.empty())
		{
			// Трекинг и вычисление частоты вибрации
			frame_time_ = frame_handler->GetCurrentTimeOfFrame();
			TrackAndCalc();

			for (int i = 0; i < vec_point_handlers_.size(); i++)
			{
				// Рисование точек, треков и данных (вибрации, амплитуды и т.п.)
				vec_point_handlers_[i]->VibratingPoint::IsInteracted(last_mouse_position_);
				vec_point_handlers_[i]->Draw(current_tracking_frame_);
			}
		}

		// обслуживаем и очищаем очереди на создание и удаление точек
		ServeTheQueues();

		// Выводим на экран ресайзнутый фрейм и записываем изначальный (не ресайзнутый)
		frame_handler->WriteFrame(current_tracking_frame_);

		current_tracking_frame_.copyTo(current_tracking_frame_resized_);
		current_tracking_frame_resized_ = frame_handler->ResizeFrame(current_tracking_frame_resized_);

		frame_handler->ShowFrame(current_tracking_frame_resized_);

		prev_img_gray_ = next_img_gray_;

		// 20 - delay in ms
		int code = waitKey(20);
		switch (code)
		{
		// пауза (пробел)
		case 32:
		{
			waitKey(0);
			/*vibration_displayer.SetMode(0);

			Mat tmp = copy_of_current_tracking_frame_;
			vibration_displayer.ProcessFrame(tmp);
			copy_of_current_tracking_frame_ = vibration_displayer.GetFrame();
			frame_processor.ShowFrame(copy_of_current_tracking_frame_);
			frame_processor.WriteFrame(copy_of_current_tracking_frame_);

			waitKey(0);
			vibration_displayer.SetMode(-1);*/
			break;
		}
		case 'c':
		{
			Mat unchanged_frame = current_tracking_frame_;
			frame_handler->ShowFrame(current_tracking_frame_);
			
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
		
				frame_handler->ShowFrame(unchanged_frame);
				waitKey(20);
			}
			break;
		}
		
		// Выделение региона интереса (ROI)
		case 'r':
		{

			/*Mat tmp = copy_of_current_tracking_frame_;
			vibration_displayer.ProcessFrame(tmp);
			copy_of_current_tracking_frame_ = vibration_displayer.GetFrame();
			frame_processor.ShowFrame(copy_of_current_tracking_frame_);

			rectangle_selected_ = false;
			while (!rectangle_selected_)
			{
				copy_of_current_tracking_frame_.copyTo(unchanged_frame_);
				setMouseCallback(frame_processor.GetWindowName(), SelectRoi, (void*)this);
				rectangle(unchanged_frame_, roi_.tl(), roi_.br(), Scalar(0, 255, 0), 1);
				frame_processor.ShowFrame(unchanged_frame_);
				waitKey(20);
			}
			std::cout << "ROI selected..." << std::endl;
			contour_handler_ = new ContourHandler(current_tracking_frame_, roi_);
			vibration_displayer.SetMode(-1);*/
			break;
		}
		case 'q':
		{

			/*Mat tmp = copy_of_current_tracking_frame_;
			vibration_displayer.ProcessFrame(tmp);
			copy_of_current_tracking_frame_ = vibration_displayer.GetFrame();
			frame_processor.ShowFrame(copy_of_current_tracking_frame_);*/

			
			running_ = false;
			std::cout << frame_handler->GetInputCapStatus() << std::endl;
			break;
		}
		}
	}
	
}