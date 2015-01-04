/* coding regulars of the data in 1D, 2D and 3D
 * conversion of subindices
 * Huayu Zhang, Dec 2014
 */
#pragma once
#include <opencv2\core\core.hpp>
#include <vector>

using namespace std;
using namespace cv;

size_t twoD2oneD(size_t i, size_t j, size_t rows, size_t cols);
void oneD2twoD(size_t id, size_t rows, size_t cols, size_t& i, size_t& j);


void ptr2Mat(vector<double*>& src, vector<Mat>& dst, int m, int n);
