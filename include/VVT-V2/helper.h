#ifndef HELPER_H
#define HELPER_H

// Отключаю варнинги на безопасность, чтобы localtime(&time) не ругалась. Пока тут Си-шная реализация, попозже переделаю
#define _CRT_SECURE_NO_WARNINGS

// standart headers
#include <iostream>
#include <fstream>

// OpenCV headers
#include <opencv2/opencv.hpp>

/*!
* @brief Функции в этом пространстве имен используются для генерирования имени csv-файла для записи и конвертирования отношения в диапазоне [0; 1] в RGB цвет
*/

namespace HelperFunctions {
    /*!
    * \addtogroup HelperFunctions
    * @{
    */

    /*!
    * @brief Генерирует имя CSV файла для сохранения метаданных
    * @return имя файла
    */
	std::string GenerateCsvFilename();
    /*!
    * @brief Конвертирует отношение в диапазоне [0; 1] в RGB цвет
    * @param ratio: отношение
    * @return: RGB цвет
    */
	cv::Scalar RatioToRgb(double ratio);
    /*!
    * @brief Эта функция пригодится. Разберусь с ней чуть позже
    */
    void FindLocalMaxima(cv::Mat& input_array, cv::Mat& local_maxima, double threshold_value);
    /*! @} End of HelperFunctions Groups */
}

#endif