#ifndef CV_CALLBACK_HANDLER_H
#define CV_CALLBACK_HANDLER_H

// standart headers
#include <iostream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

class CvCallbackHandler
{
public:
	CvCallbackHandler(int update_rate);
	
	void SetMouseHandler(std::string window_name, MouseCallback onMouse, void* userdata = (void*)0);

	Point2f GetEventCoordinates();

private:
	static void SelectPoint(int event, int x, int y, int flags, void* userdata);
	void MouseCallbackFunc(int event, int x, int y, int flags);

private:

};

#endif