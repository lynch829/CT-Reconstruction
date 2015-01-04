/* containing functions for I/O operations
 * Huayu Zhang, Dec 2014, DEP, THU
 */
#pragma once
#include <string>
#include <ostream>
#include "genimage.h"
#include "CTsystem.h"

using namespace std;

// image io
template<typename DT>
void readMatrix(string dir, string filename, DT*& arr, size_t* size, double* fov, unsigned short dim);

template<typename DT>
void writeMatrix(string dir, string filename, DT* arr, size_t* size, double* fov, unsigned short dim);



// projection io
template<typename DT>
void readProjection(string dir, string filename, DT*& p, size_t numAng, size_t numDet);
#include "io_implementation.h"

// print the system matrix to the screen or a file for debug
void print(string varname, const SystemMatrix& sm, int Nang, int Ndet, int M, int N);
void print(const SystemMatrix& sm, int Nang, int Ndet, int M, int N);
void print(string varname, const SystemMatrix& sm);
void print(const SystemMatrix& sm);

// SPECT data 
void readSPECTImage(string dir, string filename, vector<double*>& data, int rows, int cols, int slices);
void readSPECTProjection(string dir, string filename, vector<double*>& data, int dets, int slices, int frame);