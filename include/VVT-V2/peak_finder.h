#ifndef PEAK_FINDER_H
#define PEAK_FINDER_H

#include <vector>

/*!
* @brief ��������� ����� ��� (�������� ��������� ��������� - ���) � ������� (��������� vector)
* @author https://github.com/claydergc/find-peaks
*/
namespace PeakFinder {
    const float EPS = 2.2204e-16f;

    /*!
    * \addtogroup PeakFinder
    * @{
    */

    /*!
    * @param x0: ������� ������
    * @param extrema: 1 - ���� ����� ��������, -1 - ���� ����� �������
    * @param includeEndpoints: true - ���� ������� ��������� �������� ������� x0 ������ ���� �������� � ����� ����, false - � ��������� ������
    * @return peakInds: ������� �����, ��������� �  x0
    */
    void findPeaks(std::vector<float> x0, std::vector<int>& peakInds, bool includeEndpoints=true, float extrema=1);
    /*! @} End of PeakFinder Groups */
}

#endif
