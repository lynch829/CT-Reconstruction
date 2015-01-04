#include "coding.h"
#include <cassert>

size_t twoD2oneD(size_t i, size_t j, size_t rows, size_t cols){
	assert(i < rows && j < cols);
	return i * cols + j;
}

void oneD2twoD(size_t id, size_t rows, size_t cols, size_t& i, size_t& j){
	assert(id < rows * cols);
	i = id / cols;
	j = id % cols;
}


void ptr2Mat(vector<double*>& src, vector<Mat>& dst, int m, int n){
	for (int i = 0; i < src.size(); i++){
		dst.push_back(Mat(m, n, CV_64F, src[i]));
	}
}