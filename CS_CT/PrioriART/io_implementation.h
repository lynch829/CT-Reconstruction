/* implementation of io.h
 * Huayu Zhang, Dec 2014, DEP, THU
 */
#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

template<typename DT>
void readMatrix(string dir, string filename, DT*& arr, size_t* size, double* fov, unsigned short dim){
	FILE* fp = NULL;
	size_t result;
	int Npixel = 1;
	string path = dir + "\\" + filename;
	printf("Reading data from %s\n", path.c_str());
	fopen_s(&fp, path.c_str(), "rb");
	if (fp == NULL){
		fputs("File error", stderr);
		exit(1);
	}

	result = fread(&dim, sizeof(dim), 1, fp);
	if (result != 1){
		fputs("Dimension reading error", stderr);
		exit(2);
	}
	result = fread(size, sizeof(unsigned int), dim, fp);
	if (result != dim){
		fputs("Size reading error", stderr);
		exit(3);
	}
	result = fread(fov, sizeof(double), dim, fp);
	if (result != dim){
		fputs("FOV reading error", stderr);
		exit(4);
	}
	for (size_t i = 0; i < result; i++)
		Npixel *= size[i];
	arr = new DT[Npixel];
	result = fread(arr, sizeof(DT), Npixel, fp);
	if (result != Npixel){
		fputs("Data reading error", stderr);
		exit(5);
	}
	fclose(fp);
	printf("Reading data finished\n");
}

template<typename DT>
void writeMatrix(string dir, string filename, DT* arr, size_t* size, double* fov, unsigned short dim){
	FILE* fp = NULL;
	size_t result;
	int Npixel = 1;
	string path = dir + "\\" + filename;
	printf("Saving data to %s\n", path.c_str());
	fopen_s(&fp, path.c_str(), "wb");
	if (fp == NULL){
		fputs("File error", stderr);
		exit(1);
	}

	result = fwrite(&dim, sizeof(unsigned short), 1, fp);
	if (result != 1){
		fputs("Dimension writing error", stderr);
		exit(2);
	}

	result = fwrite(size, sizeof(unsigned int), dim, fp);
	if (result != dim){
		fputs("Size writing error", stderr);
		exit(3);
	}
	result = fwrite(fov, sizeof(double), dim, fp);
	if (result != dim){
		fputs("FOV writing error", stderr);
		exit(4);
	}
	for (size_t i = 0; i < result; i++)
		Npixel *= size[i];
	result = fwrite(arr, sizeof(DT), Npixel, fp);
	if (result != Npixel){
		fputs("Data writing error", stderr);
		exit(5);
	}
	fclose(fp);
	printf("Saving data finished\n");
}




template<typename DT>
void readProjection(string dir, string filename, DT*& p, size_t numAng, size_t numDet){
	FILE* fp = NULL;
	size_t result;
	string path = dir + "\\" + filename;
	float* p_float = new float[numAng * numDet];
	printf("Saving data to %s\n", path.c_str());
	fopen_s(&fp, path.c_str(), "rb");
	p = new DT[numAng * numDet];
	result = fread(p_float, sizeof(float), numAng * numDet, fp);
	if (result != numAng * numDet){
		fputs("Projection reading error", stderr);
		exit(1);
	}
	for (size_t i = 0; i < numAng * numDet; i++){
		p[i] = static_cast<DT>(p_float[i]);
	}
	fclose(fp);
	printf("Reading projection data finished\n");
}