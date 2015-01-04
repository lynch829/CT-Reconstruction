/* implementation of io.h
 * Huayu Zhang, Dec 2014, DEP, THU
 */
#include "io.h"
#include "coding.h"
#include <iostream>
#include <fstream>
#include <vector>

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

void print(string varname, const SystemMatrix& sm, int Nang, int Ndet, int M, int N){
	ofstream os;
	os.open("system_matrix.txt");
	os << varname << endl;
	size_t iang, jdet, fi, fj;
	for (int i = 0; i < sm.rows; i++){
		oneD2twoD(i, Nang, Ndet, iang, jdet);
		os << "Angle: " << iang << "Detector: " << jdet << endl;
		for (ROW::iterator it = sm.index[i].begin(); it != sm.index[i].end(); it++){
			oneD2twoD(it->first, M, N, fi, fj);
			os << "(" << fi << "," << fj << "): " << sm.at(i, it->first) << endl;
		}
	}
	os.close();
}

void print(const SystemMatrix& sm, int Nang, int Ndet, int M, int N){
	size_t iang, jdet, fi, fj;
	for (int i = 0; i < sm.rows; i++){
		oneD2twoD(i, Nang, Ndet, iang, jdet);
		cout << "Angle: " << iang << "Detector: " << jdet << endl;
		for (ROW::iterator it = sm.index[i].begin(); it != sm.index[i].end(); it++){
			oneD2twoD(it->first, M, N, fi, fj);
			cout << "(" << fi << "," << fj << "): " << sm.at(i, it->first) << endl;
		}
	}
}

void print(string varname, const SystemMatrix& sm){
	ofstream os;
	os.open("system_matrix.txt");
	os << varname << endl;
	for (int i = 0; i < sm.rows; i++){
		for (ROW::iterator it = sm.index[i].begin(); it != sm.index[i].end(); it++){
			os << "(" << i << "," << it->first << "): " << it->second << endl;
		}
	}
	os.close();
}

void print(const SystemMatrix& sm){
	for (int i = 0; i < sm.rows; i++){
		for (ROW::iterator it = sm.index[i].begin(); it != sm.index[i].end(); it++){
			cout << "(" << i << "," << it->first << "): " << it->second << endl;
		}
	}
}

void readSPECTImage(string dir, string filename, vector<double*>& data, int rows, int cols, int slices){
	FILE* fp = NULL;
	size_t result;
	int Npixel = rows * cols * slices;
	string path = dir + "\\" + filename;
	float* seq = new float[Npixel];
	printf("Reading data from %s\n", path.c_str());
	fopen_s(&fp, path.c_str(), "rb");
	if (fp == NULL){
		fputs("File error", stderr);
		exit(1);
	}
	result = fread(seq, sizeof(float), Npixel, fp);
	if (result != Npixel){
		fputs("Data reading error", stderr);
		exit(5);
	}
	fclose(fp);
	data.assign(slices, NULL);
	for (int k = 0; k < slices; k++){
		data[k] = new double[rows * cols];
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < cols; j++){
				data[k][i * cols + j] = seq[k * slices * cols + i * cols + j];
			}
		}
	}
	printf("Reading data finished\n");
}

void readSPECTProjection(string dir, string filename, vector<double*>& data, int dets, int slices, int frame){
	FILE* fp = NULL;
	size_t result;
	int Npixel = dets * slices * frame;
	string path = dir + "\\" + filename;
	float* seq = new float[Npixel];	
	printf("Reading data from %s\n", path.c_str());
	fopen_s(&fp, path.c_str(), "rb");
	if (fp == NULL){
		fputs("File error", stderr);
		exit(1);
	}
	result = fread(seq, sizeof(float), Npixel, fp);	// dets -> slices -> frame
	if (result != Npixel){
		fputs("Data reading error", stderr);
		exit(5);
	}
	fclose(fp);
	data.assign(slices, NULL);
	for (int k = 0; k < slices; k++){
		data[k] = new double[dets * frame];
		for (int i = 0; i < frame; i++){
			for (int j = 0; j < dets; j++){
				data[k][i * dets + j] = seq[i * slices * dets + k * dets + j];
				//data[k][i * dets + j] = seq[k * slices * frame + i * dets + j];
			}
		}
	}
	printf("Reading data finished\n");
}