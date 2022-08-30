#ifndef PEAK_FINDER_H
#define PEAK_FINDER_H

#include <vector>

/*!
* @brief Позволяет найти пик (исключая небольшие колебания - шум) в массиве (контейнер vector)
* @author https://github.com/claydergc/find-peaks
*/
namespace PeakFinder {
    const float EPS = 2.2204e-16f;

    /*!
    * \addtogroup PeakFinder
    * @{
    */

    /*!
    * @param x0: входной массив
    * @param extrema: 1 - если нужен максимум, -1 - если нужен минимум
    * @param includeEndpoints: true - если границы интервала входного массива x0 должны быть включены в поиск пика, false - в противном случае
    * @return peakInds: индексы пиков, найденные в  x0
    */
    void findPeaks(std::vector<float> x0, std::vector<int>& peakInds, bool includeEndpoints=true, float extrema=1);
    /*! @} End of PeakFinder Groups */
}

#endif
