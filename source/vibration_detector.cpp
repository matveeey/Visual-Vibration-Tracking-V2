#include "VVT-V2\vibration_detector.h"

VibrationDetector::VibrationDetector(std::string input_file_name, std::string output_file_name, std::string window_name) :
	input_file_name_{ input_file_name },
	output_file_name_{ output_file_name },
	window_name_{ window_name },
	fft_performer_{nullptr},
	number_of_points_{ 0 },
	vec_of_frequencies_{ 0 },
	amplitude_coeff_{ 1.0 }
{
	point_selected_ = false;
	lk_win_size_ = 45;
}

VibrationDetector::~VibrationDetector()
{
	delete[] this->fft_performer_;
	delete[] this->data_displayer_;
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
		this->prev_pts_.push_back(click_coords_);

		this->number_of_points_++;

		this->fft_performer_ = new FftPerformer();
		this->vec_of_fft_performers_.push_back(*fft_performer_);

		this->data_displayer_ = new DataDisplayer();
		this->vec_of_data_displayer_.push_back(*data_displayer_);
		break;
	}
	}
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
		Size(lk_win_size_ * 2 + 1, lk_win_size_ * 2 + 1),
		5,
		TermCriteria(
			TermCriteria::MAX_ITER | TermCriteria::EPS,
			50,
			0.03
		)
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


void VibrationDetector::DrawLines(std::vector<Point2f> prev_pts, std::vector<Point2f> next_pts, Mat& frame)
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

		// circle
		circle(frame, next_pts[current_tracking_point], 10, (0, 0, 255), 2);
	}
}

void VibrationDetector::ExecuteVibrationDetection()
{
	FrameHandler frame_processor(input_file_name_, output_file_name_, MAIN_WINDOW_NAME);

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

	while ((current_num_of_frame <= amount_of_frames) && running_ == true)
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
			// getting next_pts_ updated from Lucas-Kanade tracking
			LucasKanadeTracking(prev_img_gray_, next_img_gray_, prev_pts_, next_pts_, status_);

			// collecting just tracked points
			std::cout << "doin " << std::endl;
			for (int i = 0; i < next_pts_.size(); i++)
			{
				vec_of_fft_performers_[i].CollectTrackedPoints(frame_processor.GetCurrentPosOfFrame(), next_pts_[i], frame_processor.GetCurrentTimeOfFrame(), i);

				if (((vec_of_fft_performers_[i].GetLengthOfPointData()) % 3 == 0) && (vec_of_fft_performers_[i].GetLengthOfPointData() != 0))
				{

					vec_of_frequencies_.clear();
					vec_of_frequencies_ = vec_of_fft_performers_[i].ExecuteFft(sampling_frequency_, false, i); // for a certain point

					vec_of_data_displayer_[i].SetVectorOfFrequencies(vec_of_frequencies_);
					
					current_amplitude_ = vec_of_fft_performers_[i].GetAmplitude();
				}
				
				vec_of_data_displayer_[i].OutputVibrationParameters(current_tracking_frame_, next_pts_[i], amplitude_coeff_, current_amplitude_);
			}

			// drawing lines
			DrawLines(prev_pts_, next_pts_, current_tracking_frame_);

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
		}
	}
}