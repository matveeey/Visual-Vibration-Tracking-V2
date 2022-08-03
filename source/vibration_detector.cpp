#include "VVT-V2\vibration_detector.h"

VibrationDetector::VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name) :
	input_file_name_{ input_file_name },
	output_file_name_{ output_file_name },
	window_name_{ window_name },
	fft_performer_{nullptr},
	number_of_points_{ 0 },
	vec_of_frequencies_{ 0 },
	frequency_update_rate_{ 20 }
{
	point_selected_ = false;
	lk_win_size_ = 91;
	point_offset_ = 100;
	res_mp_ = 1;
}

VibrationDetector::~VibrationDetector()
{
	// переделай
	delete this->data_displayer_;
	delete this->fft_performer_;
	delete this->point_handler_;
}

// callback function for determining the event click on mouse
void VibrationDetector::SelectPoint(int event, int x, int y, int flags, void* userdata)
{
	VibrationDetector* d = static_cast<VibrationDetector*>(userdata);
	d->OnMouse(event, x, y, flags);
}

// "helper" function for implementing callback function as a method of C++ class
void VibrationDetector::OnMouse(int event, int x, int y, int flags)
{
	switch (event) {
	// ЛКМ была прожата вниз
	case EVENT_LBUTTONDOWN:
	{
		this->point_selected_ = true;
		this->click_coords_.x = x;
		this->click_coords_.y = y;
		this->text_coords_.push_back(click_coords_);
		this->prev_pts_.push_back(click_coords_);

		this->number_of_points_++;

		this->point_handler_ = new PointHandler(frequency_update_rate_);
		this->vec_of_point_handlers_.push_back(*point_handler_);

		this->fft_performer_ = new FftPerformer();
		this->vec_of_fft_performers_.push_back(*fft_performer_);

		this->data_displayer_ = new DataDisplayer();
		this->vec_of_data_displayer_.push_back(*data_displayer_);
		break;
	}
	case EVENT_RBUTTONDOWN:
	{
		this->point_to_be_deleted_.x = x;
		point_to_be_deleted_.y = y;

		for (int i = 0; i < prev_pts_.size(); i++)
		{
			intersection_ = IntersectionCheck(i);
			point_id_ = i;
			if (intersection_)
			{
				std::cout << "point number " << point_id_ << " is gonna be deleted" << std::endl;
				prev_pts_.erase(std::begin(prev_pts_) + point_id_);
				next_pts_.erase(std::begin(next_pts_) + point_id_);
				text_coords_.erase(std::begin(text_coords_) + point_id_);
				vec_of_data_displayer_.erase(std::begin(vec_of_data_displayer_) + point_id_);
				vec_of_fft_performers_.erase(std::begin(vec_of_fft_performers_) + point_id_);
				if (prev_pts_.size() == 0)
				{
					point_selected_ = false;
				}
			}
		}
		break;
	}
	}

	if (!prev_pts_.empty())
	{
		last_mouse_position_.x = x;
		last_mouse_position_.y = y;
	}
}

bool VibrationDetector::IntersectionCheck(int point_num)
{
	Rect offset_box(Point(prev_pts_[point_num].x - point_offset_, prev_pts_[point_num].y - point_offset_), Point(prev_pts_[point_num].x + point_offset_, prev_pts_[point_num].y + point_offset_));
	if (offset_box.contains(last_mouse_position_))
	{
		std::cout << "You are in neighbourhood of point " << point_num << std::endl;
		return true;
	}
	return false;

}

void VibrationDetector::LucasKanadeTracking(Mat prev_img_gray, Mat next_img_gray, std::vector<Point2f>& prev_pts, std::vector<Point2f>& next_pts, std::vector<uchar>& status)
{
	// calling Lucas-Kanade algorithm
	calcOpticalFlowPyrLK(
		prev_img_gray,
		next_img_gray,
		prev_pts,
		next_pts,
		status,
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
}

void VibrationDetector::LucasKanadeDoubleSideTracking(Mat prev_img_gray, Mat next_img_gray, std::vector<Point2f>& prev_pts, std::vector<Point2f>& next_pts, std::vector<uchar> status)
{
	std::vector<Point2f> tmp_prev_pts = prev_pts;
	//std::vector<Point2f> tmp_next_pts;

	// первый "прямой" вызов
	LucasKanadeTracking(prev_img_gray, next_img_gray, tmp_prev_pts, next_pts, status);
	// второй "обратный" вызов 
	LucasKanadeTracking(next_img_gray, prev_img_gray, next_pts, tmp_prev_pts, status);

	for (int i = 0; i < next_pts.size(); i++)
	{
		next_pts[i] = next_pts[i] - (tmp_prev_pts[i] - prev_pts[i]);
		std::cout << tmp_prev_pts[i] - prev_pts[i] << std::endl;
	}
}


void VibrationDetector::DrawPoints(std::vector<Point2f> prev_pts, std::vector<Point2f> next_pts, Mat& frame)
{
	for (int current_tracking_point = 0; current_tracking_point < static_cast<int>(prev_pts.size()); current_tracking_point++) {
		// line between two dots (next and previous)
		line(frame,
			prev_pts[current_tracking_point],
			next_pts[current_tracking_point],
			Scalar(0, 255, 0),
			1,
			LINE_AA
		);

		rectangle(frame, Rect(Point2f(next_pts[current_tracking_point].x - lk_win_size_, next_pts[current_tracking_point].y - lk_win_size_),
			Point2f(next_pts[current_tracking_point].x + lk_win_size_, next_pts[current_tracking_point].y + lk_win_size_)), Scalar(255, 0, 0), 1);

		// circle
		if (vec_of_interacts_[current_tracking_point])
		{
			circle(frame, next_pts[current_tracking_point], 10, Scalar(255, 207, 64), 2);
		}
		else
		{
			circle(frame, next_pts[current_tracking_point], 10, Scalar(0, 0, 255), 2);
		}

	}
}

void VibrationDetector::ExecuteVibrationDetection()
{
	FrameHandler frame_processor(input_file_name_, output_file_name_, MAIN_WINDOW_NAME);
	if (frame_processor.GetFrameHeight() > 1000)
	{
		res_mp_ = 2;
	}

	vibration_inited_ = false;
	colors_inited_ = false;

	// reading the first frame of sequence so we can convert it to gray color space
	frame_processor.ReadNextFrame();
	current_tracking_frame_ = frame_processor.GetCurrentFrame();
	prev_img_gray_ = frame_processor.GetGrayFrame(current_tracking_frame_);
	sampling_frequency_ = frame_processor.GetInputFps();

	// conditions of exit
	running_ = true;
	int current_num_of_frame = 0;
	int amount_of_frames = frame_processor.GetAmountOfFrames();

	while ((current_num_of_frame < amount_of_frames) && running_ == true)
	{
		current_num_of_frame = frame_processor.GetCurrentPosOfFrame();
		// reading next frame and converting it to gray color space
		frame_processor.ReadNextFrame();
		current_tracking_frame_ = frame_processor.GetCurrentFrame();
		next_img_gray_ = frame_processor.GetGrayFrame(current_tracking_frame_);

		// callback function for detecting the click - these coords are our starting point
		setMouseCallback(frame_processor.GetWindowName(), SelectPoint, (void*)this);

		// Lucas-Kanade tracking
		if (this->point_selected_)
		{
			vec_of_interacts_.clear();
			// getting next_pts_ updated from Lucas-Kanade tracking
			LucasKanadeTracking(prev_img_gray_, next_img_gray_, prev_pts_, next_pts_, status_);

			// collecting just tracked points
			std::cout << "doin " << std::endl;
			for (int i = 0; i < next_pts_.size(); i++)
			{
				interaction_ = IntersectionCheck(i);
				if (interaction_)
					vec_of_interacts_.push_back(true);
				else
					vec_of_interacts_.push_back(false);

				vec_of_fft_performers_[i].CollectTrackedPoints(frame_processor.GetCurrentPosOfFrame(), next_pts_[i], frame_processor.GetCurrentTimeOfFrame(), i);

				if (((vec_of_fft_performers_[i].GetLengthOfPointData()) % 3 == 0) && (vec_of_fft_performers_[i].GetLengthOfPointData() != 0))
				{

					vec_of_frequencies_.clear();
					vec_of_frequencies_ = vec_of_fft_performers_[i].ExecuteFft(sampling_frequency_, false, i); // for a certain point

					vec_of_data_displayer_[i].SetVectorOfFrequencies(vec_of_frequencies_);
				}
				
				vec_of_data_displayer_[i].OutputVibrationParameters(current_tracking_frame_, text_coords_[i], res_mp_);
			}

			// drawing lines
			DrawPoints(prev_pts_, next_pts_, current_tracking_frame_);

			// update points and frames
			this->prev_pts_ = next_pts_;
			this->prev_vibrating_pts_ = next_vibrating_pts_;
			next_img_gray_.copyTo(this->prev_img_gray_);
		}		

		// display and write frame
		frame_processor.ShowFrame(current_tracking_frame_);
		frame_processor.WriteFrame(current_tracking_frame_);

		// 20 - delay in ms
		int code = waitKey(20);
		switch (code)
		{
		case 'q':
		{
			frame_processor.~FrameHandler();
			running_ = false;
			std::cout << frame_processor.GetInputCapStatus() << std::endl;
			break;
		}
		case 'p':
		{
			waitKey(0);
			break;
		}
		}
	}
}