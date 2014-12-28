#include "io.h"
#include "systemmatrix.h"
#include "coding.h"
#include <iostream>
#include <fstream>

using namespace std;

void print(string varname, const Mat& I){
	cout << varname << endl;
	for (int y = 0; y < I.rows; ++y) {
		for (int x = 0; x < I.cols; ++x) {
			printf("%.2f ", I.at<double>(y, x));
		}
		printf("\n");
	}
}

void print(string varname, const SystemMatrix& sm){
	ofstream os;
	os.open("system_matrix.txt");
	os << varname << endl;
	size_t iang, jdet, fi, fj;
	for (int i = 0; i < sm.rows; i++){
		oneD2twoD(i, sm.Nang, sm.Ndet, iang, jdet);
		os << "Angle: " << iang << "Detector: " << jdet << endl;
		for (ROW::iterator it = sm.index[i].begin(); it != sm.index[i].end(); it++){
			oneD2twoD(it->first, sm.N, sm.N, fi, fj);
			os << "(" << fi << "," << fj << "): " << sm.at(i, it->first) << endl;
		}
	}
	os.close();
}

void print(const SystemMatrix& sm){
	size_t iang, jdet, fi, fj;
	for (int i = 0; i < sm.rows; i++){
		oneD2twoD(i, sm.Nang, sm.Ndet, iang, jdet);
		cout << "Angle: " << iang << "Detector: " << jdet << endl;
		for (ROW::iterator it = sm.index[i].begin(); it != sm.index[i].end(); it++){
			oneD2twoD(it->first, sm.N, sm.N, fi, fj);
			cout << "(" << fi << "," << fj << "): " << sm.at(i, it->first) << endl;
		}
	}
}