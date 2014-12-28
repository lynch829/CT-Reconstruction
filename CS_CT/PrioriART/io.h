#pragma once
#include <string>
#include <ostream>
#include "genimage.h"
#include "CTsystem.h"

using namespace std;

// image io
template<typename DT>
void readMatrix(string dir, string filename, DT*& arr, size_t* size, double* fov, unsigned short& dim);

template<typename DT>
void writeMatrix(string dir, string filename, DT*& arr, size_t* size, double* fov, unsigned short& dim);



// projection io
template<typename DT>
void readProjection(string dir, string filename, DT*& p, size_t numAng, size_t numDet);
#include "io_implementation.h"

void print(string varname, const Mat& I);

void print(string varname, const SystemMatrix& sm);