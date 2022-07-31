#include "VVT-V2/window_output.h"

WindowOutput::WindowOutput(std::string window_name) :
	window_name_{ window_name }
{
	namedWindow(window_name_);
}

void WindowOutput::DisplayFrame(Mat frame)
{
	imshow(window_name_, frame);
}
