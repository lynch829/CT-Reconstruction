#include "CTsystem.h"
#include "testfunc.h"
#include "io.h"
#include "coding.h"
#include <opencv2\imgproc\imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h> 

using namespace std;

void SystemInfo::calculateAngs(){
	angs = new double[Nang];
	double dang = M_PI * 2 / Nang;
	angs[0] = 0;
	for (size_t i = 1; i < Nang; i++){
		angs[i] = angs[i-1] + dang;
	}
}

void SystemInfo::selectAngle(size_t start, size_t end, size_t step, vector<double>& angles){
	step = min(step, Nang);
	for (UINT32 i = start; i < end; i += step){
		angles.push_back(angs[i]);
	}
}

double* SystemInfo::selectProject(size_t start, size_t end, size_t step, double* P){
	double* Ps;
	UINT32 n = (end - start) / step;
	Ps = new double[n * Ndet];
	step = min(step, Nang);
	for (UINT32 i = 0; i < n; i++){
		memcpy(Ps + i * Ndet, P + i * step * Ndet, Ndet * sizeof(double));
	}
	return Ps;
}

void SystemInfo::pinpoint(Vec2d& src, vector<Vec2d>& dets){
	src = Vec2d(-dist_os, 0);
	double dl = l_det / Ndet, start = -(l_det - dl) / 2;
	for (size_t i = 0; i < Ndet; i++){
		dets.push_back(Vec2d(dist_od, start + i * dl));
	}
}

void SystemInfo::calculate(size_t*& s, double*& fov, unsigned short dim){
	s = new size_t[dim];
	fov = new double[dim];
	for (unsigned short i = 0; i < dim; i++){
		s[i] = N;
		fov[i] = l_model;
	}
}


CTSystem::CTSystem(const Vec2d& s, const vector<Vec2d>& d, const vector<double>& a,
	double* prj, unsigned short dim, unsigned int* sz, double* fov) :
	src0(s), det0(d), angles(a), C(d.size(), a.size(), sz[0]){
	P.create(d.size() * a.size(), 1, CV_64F);
	for (int i = 0; i < P.rows; i++)
		P.at<double>(i, 0) = prj[i];
	assert(sz); assert(fov);
	imginfo.dim = dim; 
	imginfo.FOV.assign(fov, fov + dim);
	imginfo.size.assign(sz, sz + dim);
	fullImage = CTPixel(Vec2d(-fov[0]/2, -fov[1]/2), fov[0]);
}

void CTSystem::generatePixelMap(){
	int Nx, Ny, id;
	Nx = imginfo.size[1];
	Ny = imginfo.size[0];
	double dx = imginfo.FOV[0] / Nx;
	double dy = imginfo.FOV[1] / Ny;
	Vec2d lb;	//left bottom
	pixelInfo = new CTPixel[Nx * Ny];
	for (int i = 0; i < Ny; i++){
		for (int j = 0; j < Nx; j++){
			lb(0) = (j - Nx / 2) * dx;
			lb(1) = (i - Ny / 2) * dy;
			id = twoD2oneD(i, j, Ny, Nx);
			pixelInfo[id] = CTPixel(lb, dx);
		}
	}
}


void CTSystem::getIndex(const Vec2d& pos, const Line& l, size_t& i, size_t& j){
	double dx = imginfo.FOV[0] / imginfo.size[1];
	double dy = imginfo.FOV[1] / imginfo.size[0];
	if (l.b > 0){
		j = static_cast<size_t>((pos(0) + imginfo.FOV[0] / 2) / dx);
		i = static_cast<size_t>(ceil((pos(1) + imginfo.FOV[1] / 2) / dy)) - 1;
	}
	else{
		j = static_cast<size_t>((pos(0) + imginfo.FOV[0] / 2) / dx);
		i = static_cast<size_t>((pos(1) + imginfo.FOV[1] / 2) / dy);
	}
}

int CTSystem::generateSystemMatrix(){
	Vec2d sr, dr;	// source and detector rotated
	Intersect its;
	Line pl;	// project line
	size_t i, j, id_f;
	size_t angle_s = angles.size(), det_s = det0.size(), id_p;
	long nnzeros = 0;	// number of non-zeros
	bool intersectWithImage = false;
	cout << "Computing system matrix...\n";
	generatePixelMap();
	// angle loop
	for (size_t k = 0; k < angle_s; k++){
		printf("angle:%.1f\n", angles[k]);
		sr = rotate(src0, angles[k]);
		// detector loop
		for (size_t l = 0; l < det_s; l++){
			id_p = k * det_s + l;
			dr = rotate(det0[l], angles[k]);
			pl = Line(sr, dr);
			intersectWithImage = fullImage.intersectWith(pl);		// intersection between image and pl
			if (intersectWithImage){
				its = fullImage.getIntersect(0);	// get left intersection point
				getIndex(its.Pt, pl, i, j);			// get the index of the pixel where Pt is located
				// find all contributing pixels and store them in set pls
				while (i < imginfo.size[0] && j < imginfo.size[1]){
					id_f = twoD2oneD(i, j, imginfo.size[0], imginfo.size[1]);
					assert(pixelInfo[id_f].intersectWith(pl));
					if (pixelInfo[id_f].getWeight() > 1e-5){
						C.insertOrChange(id_p, id_f, pixelInfo[id_f].getWeight());	// add weight to sparse matrix
					}

					nnzeros++;
					if (pl.b < 0){
						switch (pixelInfo[id_f].getIntersect(1).pside)
						{
						case RIGHT:j++; break;
						case UPPER:i++; break;
						case RIGHT_UPPER:i++; j++; break;
						default: break;
						}
					}
					else{
						switch (pixelInfo[id_f].getIntersect(1).pside)
						{
						case RIGHT:j++; break;
						case BOTTOM:i--; break;
						case RIGHT_BOTTOM:i--; j++; break;
						default: break;
						}
					}
				} // end while
			}

		}	// end for l: detector loop
	}	// end for k: angle loop
	//print("System_Matrix.txt", C);	// for debug
	printf("Number of nonzeros: %d, size: %d * %d, Rate of Sparsity: %f\n", 
		nnzeros, C.rows, C.cols, static_cast<double>(nnzeros) / C.rows / C.cols);
	return nnzeros;
}


// reconstruction algorithm, return run time(ms)
int CTSystem::art(const Mat& f0, Mat& I, unsigned int N, double lambda){
	Mat f = f0.clone(), lastf;
	int m = imginfo.size[0], n = imginfo.size[1];
	size_t fi, fj;
	ROW* wi;
	SYSTEMTIME start, end;
	double* cv = new double[N];
	double k;
	cout << "ART start...\n";
	getPresentTime(start);
	for (size_t iart = 0; iart < N; iart++){
		lastf = f.clone();
		// ART kernel
		for (int i = 0; i < C.rows; i++){
			wi = C.getRow(i);
			k = (P.at<double>(i, 0) - (C.oneRowMulWith(i, f))) / C.squaredNorm(i);
			for (ROW::iterator it = wi->begin(); it != wi->end(); it++){
				oneD2twoD(it->first, m, n, fi, fj);
				f.at<double>(fi,fj) += lambda * k * it->second;
			}
		}
		for (int i = 0; i < m*n; i++){
			if (f.at<double>(i) < 0)
				f.at<double>(i) = 0;
		}
		cv[iart] = pow(norm(lastf - f),2);
		// for debug
		//print("result:", f);
		printf("ART number:%d, MSE=%f\n", iart, cv[iart]);
	}
	I = f;
	getPresentTime(end);
	return getTimeConsumption(start, end);
}

int CTSystem::conjugategradient(){
	SYSTEMTIME start, end;
	getPresentTime(start);
	
	getPresentTime(end);
	return getTimeConsumption(start, end);
}

int CTSystem::arttv(){
	SYSTEMTIME start, end;
	getPresentTime(start);

	getPresentTime(end);
	return getTimeConsumption(start, end);
}

int CTSystem::arttv(const Mat& Ipriori){	// ART-TV with priori information
	SYSTEMTIME start, end;
	getPresentTime(start);

	getPresentTime(end);
	return getTimeConsumption(start, end);
}


bool SystemMatrix::valid(int i, int j) const{
	return (-1 < i && i < rows) && (-1 < j && j < cols);
}


SystemMatrix::SystemMatrix(const SystemInfo& sysinfo){
	Ndet = sysinfo.Ndet;
	Nang = sysinfo.Nang;
	N = sysinfo.N;
	rows = Ndet * Nang;
	cols = N * N;
	index = new ROW[rows];
}

SystemMatrix::SystemMatrix(size_t ndet, size_t nang, size_t Nimg) : Ndet(ndet), Nang(nang), N(Nimg){
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
		P += it->second * f.at<double>(it->first);
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
	return index + i;
}