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
	double dang = (ae - as) / Nang;
	angs[0] = as;
	for (size_t i = 1; i < Nang; i++){
		angs[i] = angs[i-1] + dang;
	}
}

void SystemInfo::selectAngle(size_t start, size_t end, size_t step, vector<double>& angles){
	step = min(step, Nang);
	for (size_t i = start; i < end; i += step){
		angles.push_back(angs[i]);
	}
}

double* SystemInfo::selectProject(size_t start, size_t end, size_t step, double* P){
	double* Ps;
	size_t n = (end - start) / step;
	Ps = new double[n * Ndet];
	step = min(step, Nang);
	for (size_t i = 0; i < n; i++){
		memcpy(Ps + i * Ndet, P + i * step * Ndet, Ndet * sizeof(double));
	}
	return Ps;
}

void SystemInfo::pinpoint(vector<Vec2d>& srcs, vector<Vec2d>& dets, ProjectionType prjtype){
	double dl = l_det / Ndet, start = -(l_det - dl) / 2;
	for (size_t i = 0; i < Ndet; i++){
		switch (prjtype)
		{
		case FAN:
			srcs.push_back(Vec2d(-dist_os, 0));
			break;
		case PARALLEL:
			srcs.push_back(Vec2d(-dist_os, start + i * dl));		
			break;
		default:
			break;
		}
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


CTSystem::CTSystem(const vector<Vec2d>& s, const vector<Vec2d>& d, const vector<double>& a,
	 unsigned short dim, size_t* sz, double* fov) :
	 src0(s), det0(d), angles(a), C(d.size() * a.size(), sz[0] * sz[1]), CT(sz[0] * sz[1], d.size() * a.size()){
	assert(sz); assert(fov);
	imginfo.dim = dim; 
	imginfo.FOV.assign(fov, fov + dim);
	imginfo.size.assign(sz, sz + dim);
	fullImage = CTPixel(Vec2d(-fov[0]/2, -fov[1]/2), fov[0]);
}

CTSystem::CTSystem(const CTSystem& cts) :
src0(cts.src0), det0(cts.det0), angles(cts.angles), C(cts.C.rows, cts.C.cols), CT(cts.CT.rows, cts.CT.cols) {
	imginfo.dim = cts.imginfo.dim;
	imginfo.FOV = cts.imginfo.FOV;
	imginfo.size = cts.imginfo.size;
	fullImage = CTPixel(Vec2d(-imginfo.FOV[0] / 2, -imginfo.FOV[1] / 2), imginfo.FOV[0]);
	C = cts.C;
	CT = cts.CT;
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

void CTSystem::getImageInfo(ImageInfo& img_info){
	img_info = imginfo;
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
	SYSTEMTIME start, end;
	getPresentTime(start);
	generatePixelMap();
	// angle loop
	for (size_t k = 0; k < angle_s; k++){
		printf("angle:%.1f\n", angles[k]);
		// detector loop
		for (size_t l = 0; l < det_s; l++){
			id_p = k * det_s + l;
			sr = rotate(src0[l], angles[k]);
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

	C.transpose(CT);// transpose
	
	getPresentTime(end);
	return getTimeConsumption(start, end);
}


// reconstruction algorithm, return run time(ms)
int CTSystem::art(const Mat& P, const Mat& f0, Mat& I, unsigned int N, double lambda){
	Mat f = f0.clone(), lastf;
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
			k = (P.at<double>(i) - (C.oneRowMulWith(i, f))) / C.squaredNorm(i);
			for (ROW::iterator it = wi->begin(); it != wi->end(); it++){
				f.at<double>(it->first) += lambda * k * it->second;
			}
		}
		for (int i = 0; i < f.rows * f.cols; i++){
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


int CTSystem::MLEM(const Mat& P, Mat& I, const Mat& f0/*initial*/, unsigned int N/*iteration times*/
	, double tol){
	SYSTEMTIME start, end;
	getPresentTime(start);

	Mat f = f0.clone(), last_f;
	Mat Pc = Mat::zeros(P.rows, P.cols, CV_64F), Pr = Mat::zeros(P.rows, P.cols, CV_64F);	// Projection calculated
	Mat sum_c = Mat::zeros(f.rows , f.cols, CV_64F);	// summation of each column
	Mat corr = Mat::zeros(f.rows , f.cols, CV_64F);	// correct factor
	int itN = 0;
	double* cv = new double[N];
	// MLEM iteration
	while (/*cv[itN - 1] > tol && */itN < N) {
		last_f = f.clone();
		//show2Dimg(last_f, "last_f", WINDOW_NORMAL);
		C.mulWith(last_f, Pc);
		//show2Dimg(Pc, "Projection", WINDOW_NORMAL);
		divide(P, Pc, Pr);
		//show2Dimg(Pr, "Projetion relative", WINDOW_NORMAL);
		CT.sumRows(sum_c);
		//show2Dimg(sum_c, "Sum_column", WINDOW_NORMAL);
		CT.mulWith(Pr, corr);
		//show2Dimg(corr, "Projection correction factor", WINDOW_NORMAL);
		for (int i = 0; i < C.cols; i++){
			f.at<double>(i) = last_f.at<double>(i)
				/ sum_c.at<double>(i) * corr.at<double>(i);
		}
		//show2Dimg(last_f, "Last", WINDOW_NORMAL);
		//show2Dimg(f, "Curr", WINDOW_NORMAL);
		cv[itN++] = pow(norm(last_f - f), 2);
		// for debug
		//print("result:", f);
		//printf("MLEM number:%d, SSE=%f\n", itN, cv[itN++]);
	}
	printf("MLEM SSE=%f\n", cv[itN-1]);
	I = f;
	getPresentTime(end);
	return getTimeConsumption(start, end);
}

int CTSystem::MLEM_gaussian(const Mat& P, Mat& I, const Mat& f0, Gaussian gs,
	unsigned int N, double tol){
	SYSTEMTIME start, end;
	getPresentTime(start);

	Mat f = f0.clone(), last_f;
	Mat Pc = Mat::zeros(P.rows, P.cols, CV_64F), Pr = Mat::zeros(P.rows, P.cols, CV_64F);	// Projection calculated
	Mat sum_c = Mat::zeros(f.rows, f.cols, CV_64F);	// summation of each column
	Mat corr = Mat::zeros(f.rows, f.cols, CV_64F);	// correct factor
	int itN = 0;
	double* cv = new double[N];
	// MLEM iteration
	while (/*cv[itN - 1] > tol && */itN < N) {
		last_f = f.clone();
		//show2Dimg(last_f, "last_f", WINDOW_NORMAL);
		C.mulWith(last_f, Pc);
		//show2Dimg(Pc, "Projection", WINDOW_NORMAL);
		divide(P, Pc, Pr);
		//show2Dimg(Pr, "Projetion relative", WINDOW_NORMAL);
		CT.sumRows(sum_c);
		//show2Dimg(sum_c, "Sum_column", WINDOW_NORMAL);
		CT.mulWith(Pr, corr);
		//show2Dimg(corr, "Projection correction factor", WINDOW_NORMAL);
		for (int i = 0; i < C.cols; i++){
			f.at<double>(i) = last_f.at<double>(i)
				/ sum_c.at<double>(i) * corr.at<double>(i);
		}
		GaussianBlur(f, f, gs.ksize, gs.sigmaX, gs.sigmaY);
		//show2Dimg(last_f, "Last", WINDOW_NORMAL);
		//show2Dimg(f, "Curr", WINDOW_NORMAL);
		cv[itN++] = pow(norm(last_f - f), 2);
		// for debug
		//print("result:", f);
		//printf("MLEM number:%d, SSE=%f\n", itN, cv[itN++]);
	}
	printf("MLEM SSE=%f\n", cv[itN - 1]);
	I = f;
	getPresentTime(end);
	return getTimeConsumption(start, end);
}

int CTSystem::MLEM_median(const Mat& P, Mat& I, const Mat& f0, int md,
	unsigned int N, double tol){
	SYSTEMTIME start, end;
	getPresentTime(start);

	Mat f = f0.clone(), last_f, f_filt;
	Mat Pc = Mat::zeros(P.rows, P.cols, CV_64F), Pr = Mat::zeros(P.rows, P.cols, CV_64F);	// Projection calculated
	Mat sum_c = Mat::zeros(f.rows, f.cols, CV_64F);	// summation of each column
	Mat corr = Mat::zeros(f.rows, f.cols, CV_64F);	// correct factor
	int itN = 0;
	double* cv = new double[N];
	// MLEM iteration
	while (/*cv[itN - 1] > tol && */itN < N) {
		last_f = f.clone();
		//show2Dimg(last_f, "last_f", WINDOW_NORMAL);
		C.mulWith(last_f, Pc);
		//show2Dimg(Pc, "Projection", WINDOW_NORMAL);
		divide(P, Pc, Pr);
		//show2Dimg(Pr, "Projetion relative", WINDOW_NORMAL);
		CT.sumRows(sum_c);
		//show2Dimg(sum_c, "Sum_column", WINDOW_NORMAL);
		CT.mulWith(Pr, corr);
		//show2Dimg(corr, "Projection correction factor", WINDOW_NORMAL);
		for (int i = 0; i < C.cols; i++){
			f.at<double>(i) = last_f.at<double>(i)
				/ sum_c.at<double>(i) * corr.at<double>(i);
		}
		f.convertTo(f_filt, CV_32F);
		medianBlur(f_filt, f, md);
		f.convertTo(f, CV_64F);
		//show2Dimg(last_f, "Last", WINDOW_NORMAL); // for debug
		//show2Dimg(f, "Curr", WINDOW_NORMAL);	// for debug
		cv[itN++] = pow(norm(last_f - f), 2);
		// for debug
		//print("result:", f);
		//printf("MLEM number:%d, SSE=%f\n", itN, cv[itN++]);
	}
	printf("MLEM SSE=%f\n", cv[itN - 1]);
	I = f;
	getPresentTime(end);
	return getTimeConsumption(start, end);
}






bool SystemMatrix::valid(int i, int j) const{
	return (-1 < i && i < rows) && (-1 < j && j < cols);
}


SystemMatrix::~SystemMatrix(){
	delete[] index;
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
		P.at<double>(i) = oneRowMulWith(i, f);
	}
}

double SystemMatrix::oneRowMulWith(int i, const Mat& f){
	double P = 0;	// M(i) * f
//	size_t fi, fj;	// index of image f (fi,fj)
	for (ROW::iterator it = index[i].begin(); it != index[i].end(); it++){
		//oneD2twoD(it->first, M, N, fi, fj);
		P += it->second * f.at<double>(it->first);
	}
	return P;
}


double SystemMatrix::sumRow(int i){
	double y = 0;
	for (ROW::iterator it = index[i].begin(); it != index[i].end(); it++){
		y += it->second;
	}
	return y;
}


void SystemMatrix::sumRows(Mat& S){
	for (int i = 0; i < rows; i++){
		S.at<double>(i) = sumRow(i);
	}
}

SystemMatrix& SystemMatrix::operator=(const SystemMatrix& C){
	rows = C.rows;
	cols = C.cols;
	index = new ROW[rows];
	for (int i = 0; i < rows; i++){
		index[i] = C.index[i];
	}
	return *this;
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