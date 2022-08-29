#ifndef HELPER_H
#define HELPER_H
#define _CRT_SECURE_NO_WARNINGS

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// ���������� ��� CSV ����� ��� ���������� ����������
std::string GenerateCsvFilename();
// ������������ ��������� � ��������� [0; 1] � ����
cv::Scalar RatioToRgb(double ratio);

#endif