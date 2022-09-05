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
    /*!
    * @brief ��������� ���������� ����� � ������������ ���������
    * @param value: �����, ��������� ����������
    * @param n: ����� ����� (�� �������� ����� ��������� �����)
    * @return: ����������� �����
    */
    template <typename T>
    std::string ToStringWithPrecision(const T value, const int n)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << value;
        return out.str();
    }
	/*!
	* @brief ��������� ������ ��� ������ ����� ���������
	* @param input_vector: ������, � ������� ����� ��������� ����������
	*/
    template<class T>
    void DeadzoneFilter(std::vector<T>& input_vector)
    {
        float dead_zone_factor = 0.1f;
        for (int i = 0; i < static_cast<int>(input_vector.size() * dead_zone_factor); i++)
        {
            input_vector.erase(std::begin(input_vector));
        }
    }
	/*!
	* @brief ����������� ������, ������ �������� ����� (�������)
	* @param input_vector: ������, ������� ����� ��������
	* @param n: ���������� ��������� ������ (�����), ������� �� ����� �������
	*/
	template<class T>
	std::vector<T> MakeShorterFromStart(std::vector<T> src, int n)
	{
		auto it = src.end();
		std::vector<T> dst;

		if (n < src.size())
			return src;

		while (it != (src.end() - n))
		{
			dst.push_back(*it);
			it--;
		}

		return dst;
	}
	/*!
	* @brief ��������� ������� �������� �������� ��������� ������� � ���� ((a0 - a1) + (a1 - a2)) / 3
	* @param src - ������� ������ ��� �������
	*/
	template<typename T>
	T CalculateMeanDifferenceInVector(std::vector<T> src)
	{
		std::vector<T> difference_vector;

		auto it = src.begin();
		while (it != (src.end() - 1))
		{
			difference_vector.push_back(*(it + 1) - *it);
			it++;
		}

		T mean_difference;

		it = difference_vector.begin();
		mean_difference = *it;
		it++;

		while (it != difference_vector.end())
		{
			mean_difference += *it;
			it++;
		}
		mean_difference /= difference_vector.size();

		return mean_difference;
	}
	/*!
	* @brief ���������� ������ ������ �������� �������� ������� src
	* @param src - ������� ������ ��� �������
	*/
	template<typename T>
	int FindGlobalMaxIdx(std::vector<T> src)
	{
		auto it = src.begin();
		int idx = 0;
		T last_max = 0;
		while (it != src.end())
		{
			if (*it > last_max)
			{
				idx = std::distance(src.begin(), it);
				last_max = *it;
			}
			it++;
		}

		return idx;
	}
	/*!
	* @brief ���������� ������ ������ ���������� �������� ������� src
	* @param src - ������� ������ ��� �������
	*/
	template<typename T>
	int FindGlobalMinIdx(std::vector<T> src)
	{
		auto it = src.begin();
		int idx = 0;
		T last_max = 0;
		while (it != src.end())
		{
			if (*it < last_max)
			{
				idx = std::distance(src.begin(), it);
				last_max = *it;
			}
			it++;
		}

		return idx;
	}
	/*!
	* @brief ��������� ������������ �������� �������� ��������� �������
	* @param src - ������� ������ ��� �������
	*/
	template<typename T>
	T CalculateMaxDifferenceInVector(std::vector<T> src)
	{
		std::vector<T> difference_vector;

		auto it = src.begin();
		while (it != (src.end() - 1))
		{
			difference_vector.push_back(*(it + 1) - *it);
			it++;
		}

		cv::Scalar cv_scalar_mean_val = cv::mean(src);
		T mean_val = cv_scalar_mean_val[0];

		return difference_vector[FindGlobalMaxIdx(difference_vector)] / mean_val;
	}

	/*! @} End of HelperFunctions Groups */
}

#endif