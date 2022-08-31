#ifndef HELPER_H
#define HELPER_H

// �������� �������� �� ������������, ����� localtime(&time) �� ��������. ���� ��� ��-���� ����������, ������� ���������
#define _CRT_SECURE_NO_WARNINGS

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

/*!
* @brief ������� � ���� ������������ ���� ������������ ��� ������������� ����� csv-����� ��� ������ � ��������������� ��������� � ��������� [0; 1] � RGB ����
*/

namespace HelperFunctions {
    /*!
    * \addtogroup HelperFunctions
    * @{
    */

    /*!
    * @brief ���������� ��� CSV ����� ��� ���������� ����������
    * @return ��� �����
    */
	std::string GenerateCsvFilename();
    /*!
    * @brief ������������ ��������� � ��������� [0; 1] � RGB ����
    * @param ratio: ���������
    * @return: RGB ����
    */
	cv::Scalar RatioToRgb(double ratio);
    /*!
    * @brief ��� ������� ����������. ��������� � ��� ���� �����
    */
    void FindLocalMaxima(cv::Mat& input_array, cv::Mat& local_maxima, double threshold_value);
    /*! @} End of HelperFunctions Groups */
}

#endif