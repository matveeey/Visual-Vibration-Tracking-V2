#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// standart headers
#include <iostream>
//#include <sstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

using namespace cv;

/*!
* @brief ����� ��� �������� ��������-����������, �� ������� ����� �������� ��������� ����������
*/
class Histogram
{
public:
	/*!
	* @brief ����������� ����� ������
	* @param width: ������ ���� ����������� (������� ������)
	* @param height: ������ ���� ����������� (������� ������)
	* @param x_limit: ������������ (����������) �������� �� ��� X
	* @param id: ID �����
	* @param x_values: ����� ���������� �� ���������� ��� ��� X
	* @param y_values: ����� ���������� �� ��������� ��� ��� Y
	*/
	Histogram(int width, int height, int x_limit, int id, std::vector<double>& x_values, std::vector<float>& y_values);
	/*!
	* @brief ���������� ����� ������
	* 
	* ���� ����������� ����������� ������ �����������
	*/
	~Histogram();

	/*!
	* @brief callback ������� ��� ����������� ������� (������������ ��� ��������� ����� �����)
	*/
	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	/*!
	* @brief callback ������� ��� ����������� ������� (������������ ��� ��������� ����� �����)
	*/
	void DetectEvent(int event, int x, int y, int flags);
	/*!
	* @brief ������� ����������� � ���� �����������
	*/
	void ShowHistogram();
	/*!
	* @brief ������������� X-�
	*/
	void SetXValues(std::vector<double> x_values);
	/*!
	* @brief ������������� Y-�
	*/
	void SetYValues(std::vector<float> y_values);
	/*!
	* @brief ������������� ������ ����� ��������� �����������
	*/
	void SetHistogramFlag(bool flag);
	/*!
	* @brief ��������� ������ ���� ����������� (��������, ��� ������ �� �������� ����)
	* 
	* �� ������ ������ ������� �� ������������ :/
	*/
	void SetHistogramWindowProperty(int prop_id, int prop_value);

private:
	/*!
	* @brief �������������� Histogram::histogram_background_ ��� ����������� �������������
	*/
	void InitHistogramBackground();
	/*!
	* @brief ��������� �������� ��� �����������
	*/
	Mat CalcHistogram();
	/*!
	* @brief ���������, ������������� �� ����� ���������� ����
	*/
	bool IsInteracted(int x, int interval);
	/*!
	* @brief ������� �������� ������� ����� � ��������
	*/
	void PlotMouseValue(Mat& frame, int value_idx);
	/*!
	* @brief ��� ����������� ���-�� ������
	*/
	std::string to_string_with_precision(const float value, const int n = 6);

protected:
	/*!
	* @brief ���� ��� ��������� �����������
	*/
	bool is_histogram_plotted_;

private:
	/*!
	* @brief �������� ����
	*/
	std::string winname_;
	/*!
	* @brief ������ ����� �����������
	*/
	float histogram_offset_;
	/*!
	* @brief ������ ���� ����������� (������� ������)
	*/
	int histogram_frame_width_;
	/*!
	* @brief ������ ���� ����������� (������� ������)
	*/
	int histogram_frame_height_;
	/*!
	* @brief cv::Mat ����������� �����������
	*/
	Mat histogram_;
	/*!
	* @brief cv::Mat ��� ��� �����������
	*/
	Mat histogram_background_;
	/*!
	* @brief �������� ����������� �� ��� X
	*/
	std::vector<double>* x_values_;
	/*!
	* @brief �������� ����������� �� ��� Y
	*/
	std::vector<float>* y_values_;
	/*!
	* @brief ������ ��� �������� ���
	*/
	float axis_signature_offset_;
	/*!
	* @brief �������� ��� ������� ��� � ���������� ���� �������
	*/
	float signature_interval_;
	/*!
	* @brief ���������� �������� �� ���
	*/
	int signature_amount_;
	/*!
	* @brief ������������ (����������) �������� �� ��� X
	*/
	int x_limit_;
	/*!
	* @brief ���������� ���� (�������)
	*/
	Point2i last_mouse_coordinates_;
	/*!
	* @brief ������� ���������, ������� ����� ������ ������� �����: 1.0 - 100%
	*/
	float dead_zone_coeff_;

};

#endif