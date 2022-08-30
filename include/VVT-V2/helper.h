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
    * ���������� ��� CSV ����� ��� ���������� ����������
    * @return ��� �����
    */
	std::string GenerateCsvFilename();
    /*!
    * ������������ ��������� � ��������� [0; 1] � RGB ����
    * @param ratio: ���������
    * @return: RGB ����
    */
	cv::Scalar RatioToRgb(double ratio);
    /*! @} End of HelperFunctions Groups */
}

#endif