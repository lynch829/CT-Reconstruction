#include "systemmatrix.h"
#include "coding.h"
#include <cassert>

bool SystemMatrix::valid(int i, int j) const{
	return (-1 < i && i < rows) && (-1 < j && j < cols);
}


SystemMatrix::SystemMatrix(SystemInfo& sysinfo){
	Ndet = sysinfo.Ndet;
	Nang = sysinfo.Nang;
	N = sysinfo.N;
	index = new ROW[rows];
}

SystemMatrix::SystemMatrix(size_t ndet, size_t nang, size_t Nimg): Ndet(ndet), Nang(nang), N(Nimg){
	rows = ndet * nang;
	cols = Nimg * Nimg;
	index = new ROW[rows];
}

void SystemMatrix::insertOrChange(int i, int j, double v){
	assert(valid(i, j));
	index[i][j] = v;
}

double SystemMatrix::at(int i, int j) const{
	assert(valid(i, j));
	ROW::iterator it = index[i].find(j);
	if (it != index[i].end()){ // not find
		return it->second;
	}
	else return 0;
}

// matrix operations
void SystemMatrix::transpose(SystemMatrix& At){
	At.cols = rows; At.rows = cols;
	for (int i = 0; i < rows; i++){
		for (ROW::iterator it = index[i].begin(); it != index[i].end(); it++){
			At.insertOrChange(it->first, i, it->second);
		}
	}
}

void SystemMatrix::mulWith(const Mat& f, Mat& P){
	for (int i = 0; i < rows; i++){
		P.at<double>(i, 0) = oneRowMulWith(i, f);
	}
}

double SystemMatrix::oneRowMulWith(int i, const Mat& f){
	double P = 0;	// M(i) * f
	size_t fi, fj;	// index of image f (fi,fj)
	for (ROW::iterator it = index[i].begin(); it != index[i].end(); it++){
		oneD2twoD(it->first, N, N, fi, fj);
		P += it->second * f.at<double>(fi, fj);
	}
	return P;
}

double SystemMatrix::squaredNorm(int i){
	double y = 0;
	for (ROW::iterator it = index[i].begin(); it != index[i].end(); it++){
		y += it->second * it->second;
	}
	return y;
}

ROW* SystemMatrix::getRow(int i){
	return index+i;
}