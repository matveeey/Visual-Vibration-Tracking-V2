#ifndef HELPER_H
#define HELPER_H
#define _CRT_SECURE_NO_WARNINGS

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

// Генерирует имя CSV файла для сохранения метаданных
std::string GenerateCsvFilename();
// Конвертирует отношение в диапазоне [0; 1] в цвет
cv::Scalar RatioToRgb(double ratio);

#endif