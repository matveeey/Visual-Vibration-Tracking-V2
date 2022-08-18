#include "VVT-V2\vibration_detector.h"

VibrationDetector::VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name) :
	input_file_name_{ input_file_name },
	output_file_name_{ output_file_name },
	window_name_{ window_name },
	number_of_points_{ 0 },
	update_rate_{ 20 },
	warping_figure_selecting_{ false },
	roi_selecting_{ false }
{
	point_selected_ = false;
	lk_win_size_ = 30;
	point_offset_ = 100;
	res_mp_ = 1;


}

VibrationDetector::~VibrationDetector()
{
}

void VibrationDetector::ServeTheQueues()
{
	for (int i = 0; i < create_queue_.size(); i++)
	{
		CreateNewPoint(create_queue_[i]);
	}
	for (int i = 0; i < delete_queue_.size(); i++)
	{
		DeletePoints(delete_queue_[i]);
	}
	create_queue_.clear();
	delete_queue_.clear();
}

void VibrationDetector::CreateNewPoint(Point2f mouse_coordinates)
{
	PointHandler point_handler_ = PointHandler(mouse_coordinates, update_rate_, fps_);
	vec_point_handlers_.push_back(point_handler_);

	Histogram histogram = Histogram(600, 300, static_cast<int>(fps_ / 2));
	vec_histograms_.push_back(histogram);

	std::cout << "DEBUG: creating new point" << std::endl;
}

void VibrationDetector::DeletePoints(Point2i mouse_coordinates)
{
	// создаем вектор ID (номеров) для точек, которые удалим
	std::vector<int> point_ids_to_be_deleted_;

	// проходим по всему вектору точек
	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		// если произошло пересечение с мышкой, вносим ID (номер) точки в "вектор точек на удаление"
		if (vec_point_handlers_[i].IsInteracted(mouse_coordinates))
		{
			point_ids_to_be_deleted_.push_back(i);
		}
	}
	std::cout << "DEBUG: deleting "<< point_ids_to_be_deleted_.size() << " point" << std::endl;
	// проходимся по вектору точек на удаление
	while (!point_ids_to_be_deleted_.empty())
	{
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
	switch (event) {
		// ЛКМ была прожата вниз
	case EVENT_LBUTTONDOWN:
	{
		if (!roi_selecting_ && !warping_figure_selecting_)
		{
			create_queue_.push_back(Point2i(x, y));
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
	std::vector<uchar> tmp_status;
	
	//
	// попробуй два варианта: два цикла / один цикл и трек одной точки
	//

	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		PrevPts.push_back(vec_point_handlers_[i].GetLastFoundCoordinates());
	}
	
	Mat tmp;

	// вызов Lucas-Kanade алгоритма
	calcOpticalFlowPyrLK(
		prev_img_gray_,
		next_img_gray_,
		PrevPts,
		NextPts,
		tmp_status,
		noArray(),
		Size(lk_win_size_, lk_win_size_),
		7,
		TermCriteria(
			TermCriteria::MAX_ITER | TermCriteria::EPS,
			50,
			0.03
		),
		OPTFLOW_LK_GET_MIN_EIGENVALS
	);
	
	// проверяем равны ли размеры векторов хэндлеров точек и найденных точек на картинке
	if (NextPts.size() != vec_point_handlers_.size())
	{
		std::cout << "DEBUG: ERROR - NextPts size doesnt match vec_point_handlers_ size" << std::endl;
		return;
	}
	
	previous_points_coordinates_ = PrevPts;

	// закидываем найденные значения обратно в хэндлер точек
	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		vec_point_handlers_[i].AddNewCoordinate(NextPts[i]);
		// Вызов БПФ (FFT)
		vec_point_handlers_[i].ExecuteFft();
	}
}

void VibrationDetector::DrawData(Mat& frame)
{
	if (previous_points_coordinates_.size() != vec_point_handlers_.size())
	{
		std::cout << "DEBUG: ERROR - previous_points_coordinates_ size doesnt match vec_point_handlers_ size" << std::endl;
		return;
	}

	for (int i = 0; i < vec_point_handlers_.size(); i++)
	{
		Scalar circle_color;
		Point2f new_point = vec_point_handlers_[i].GetLastFoundCoordinates();
		Point2f text_coordinates = vec_point_handlers_[i].GetTextCoordinates();
		Point3f amplitude = vec_point_handlers_[i].GetCurrentAmplitude();
		if (vec_point_handlers_[i].IsInteracted(last_mouse_position_))
		{
			circle_color = Scalar(255, 150, 50);
		}
		else circle_color = Scalar(255, 75, 25);

		// отрисовываем линии точек
		line(frame, previous_points_coordinates_[i], new_point, Scalar(0, 255, 0), 1, LINE_AA);

		// отрисовываем круг вокруг точки
		circle(frame, new_point, 10, circle_color, 2);

		std::vector<double> frequency = vec_point_handlers_[i].GetCurrentVibrationFrequency();

		// обновляем данные гистограммы и отрисовываем их
		vec_histograms_[i].set_y_values(vec_point_handlers_[i].GetY());
		vec_histograms_[i].set_x_values(vec_point_handlers_[i].GetX());
		vec_histograms_[i].plot_histogram();

		// отрисовываем частоту вибрации
		for (int j = 0; j < frequency.size(); j++)
		{
			putText(
				frame,
				"hz: " + std::to_string(frequency[j]),
				Point(text_coordinates.x + 15, text_coordinates.y + res_mp_ * j * 20),
				FONT_HERSHEY_PLAIN,
				res_mp_ * 1.25,
				Scalar(0, 69, 255),
				2
			);
		}

		// отрисовываем амплитуды вибрации
		putText(
			frame,
			"x: " + std::to_string(amplitude.x),
			Point(text_coordinates.x, text_coordinates.y - res_mp_ * 20),
			FONT_HERSHEY_PLAIN,
			res_mp_ * 1,
			Scalar(0, 255, 255),
			2
		);
		putText(
			frame,
			"y: " + std::to_string(amplitude.y),
			Point(text_coordinates.x, text_coordinates.y - res_mp_ * 2 * 20),
			FONT_HERSHEY_PLAIN,
			res_mp_ * 1,
			Scalar(0, 255, 255),
			2
		);
		// uncomment когда появится 3-я координата для амплитуды :)
		/*putText(
			frame,
			"z: " + std::to_string(amplitude.z),
			Point(text_coordinates.x, text_coordinates.y - res_mp_ * 3 * 20),
			FONT_HERSHEY_PLAIN,
			res_mp_ * 1,
			Scalar(0, 255, 255),
			2
		);*/
	}
}

void VibrationDetector::ExecuteVibrationDetection()
{
	FrameHandler frame_processor(input_file_name_, output_file_name_, MAIN_WINDOW_NAME);
	if (frame_processor.GetFrameHeight() > 1000)
	{
		res_mp_ = 2;
	}

	// reading the first frame of sequence so we can convert it to gray color space
	frame_processor.ReadNextFrame();
	current_tracking_frame_ = frame_processor.GetCurrentFrame();

	/*if (warping_figure_selected_)
	{
		current_tracking_frame_ = GetWarpedFrame(current_tracking_frame_, warping_figure_, frame_processor.GetFrameWidth(), frame_processor.GetFrameHeight());
	}*/

	prev_img_gray_ = frame_processor.GetGrayFrame(current_tracking_frame_);
	fps_ = frame_processor.GetInputFps();

	// conditions of exit
	running_ = true;
	int current_num_of_frame = 0;
	int amount_of_frames = frame_processor.GetAmountOfFrames();

	// устанавливаем callback handler на наше окно
	setMouseCallback(frame_processor.GetWindowName(), OnMouse, (void*)this);

	while ((current_num_of_frame < amount_of_frames - 1) && running_ == true)
	{
		current_num_of_frame = frame_processor.GetCurrentPosOfFrame();

		// reading next frame and converting it to gray color space
		frame_processor.ReadNextFrame();
		current_tracking_frame_ = frame_processor.GetCurrentFrame();

		if (!warping_figure_.empty())
		{
			current_tracking_frame_ = MakeWarpedFrame(current_tracking_frame_, warping_figure_);
		}

		next_img_gray_ = frame_processor.GetGrayFrame(current_tracking_frame_);

		if (!vec_point_handlers_.empty())
		{
			// Трекинг и вычисление частоты вибрации
			TrackAndCalc();

			// Рисование точек, треков и данных (вибрации, амплитуды и т.п.)
			DrawData(current_tracking_frame_);
		}

		// обслуживаем и очищаем очереди на создание и удаление точек
		ServeTheQueues();

		// display and write frame
		frame_processor.ShowFrame(current_tracking_frame_);
		frame_processor.WriteFrame(current_tracking_frame_);

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
			frame_processor.ShowFrame(current_tracking_frame_);
			
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
		
				frame_processor.ShowFrame(unchanged_frame);
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
			std::cout << frame_processor.GetInputCapStatus() << std::endl;
			break;
		}
		}
	}
	
}