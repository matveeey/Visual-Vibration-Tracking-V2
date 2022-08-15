#include "VVT-V2/cv_callback_handler.h"

CvCallbackHandler::CvCallbackHandler(int update_rate)
{
}

void CvCallbackHandler::SetMouseHandler(std::string window_name, MouseCallback onMouse, void* userdata)
{
	setMouseCallback(window_name, onMouse, userdata);
}

Point2f CvCallbackHandler::GetEventCoordinates()
{
	return Point2f();
}

void CvCallbackHandler::SelectPoint(int event, int x, int y, int flags, void* userdata)
{
}

void CvCallbackHandler::MouseCallbackFunc(int event, int x, int y, int flags)
{
	switch (event)
	{

	}
}
