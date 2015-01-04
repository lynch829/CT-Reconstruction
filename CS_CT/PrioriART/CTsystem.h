/* CT system
 * include image, system matrix and projection
 * source, detectors, angles 
 * Huayu Zhang, Dec 2014 */

#pragma once

#include<unordered_map>
#include <map>
#include <cassert>
#include "ctimage.h"

using namespace std;

// Machine
struct SystemInfo{
	double dist_os;		//distance between source and object
	double dist_od;		//distance between the detector array and object
	double l_det;		//the length of the detector array
	double l_model;		//length of model
	unsigned int Ndet;	//number of detectors
	unsigned int Nang;		//number of angles
	unsigned int N;		//image size
	double* angs;
	~SystemInfo(){ delete[] angs; }
	void calculateAngs();
	void selectAngle(size_t start, size_t end, size_t step, vector<double>& angles);
	double* selectProject(size_t start, size_t end, size_t step, double* P);
	void pinpoint(Vec2d& src, vector<Vec2d>& dets);// calculate coordinate of source and detectors
	void calculate(size_t*& s, double*& fov, unsigned short dim);
};

// FOV, size, dimension
struct ImageInfo{
	unsigned short dim;	// dim
	vector<double> FOV;   // field of view:mm
	vector<unsigned int> size;	    // matrix size
	ImageInfo& operator=(const ImageInfo& imginfo){
		dim = imginfo.dim;
		FOV = imginfo.FOV;
		size = imginfo.size;
		return *this;
	}
};



typedef map<int, double> ROW;

class SystemMatrix {
protected:
	ROW* index;	// [i].rbtree<int(row),DT(data)>	// (ang, det) -> row, im(row, col) -> col
	bool valid(int i, int j) const;
public:
	int rows, cols;	// size rols*cols
	size_t Ndet;	//number of detectors
	size_t Nang;	//number of angles
	size_t N;		//image size(square)
public:
	SystemMatrix(const SystemInfo& sysinfo);
	SystemMatrix(size_t ndet, size_t nang, size_t N);
	// get and set
	void insertOrChange(int i, int j, double v);
	double at(int i, int j) const;
	// matrix operations
	void transpose(SystemMatrix& At);
	void mulWith(const Mat& f, Mat& P);
	double oneRowMulWith(int i/*row id*/, const Mat& f);
	double squaredNorm(int i);
	ROW* getRow(int i);
	friend void print(string varname, const SystemMatrix& sm);	// for debug
	friend void print(const SystemMatrix& sm);
};

class CTSystem{
protected:
	//CTImage image;		// image
	Vec2d src0;			// initial position of the source 
	vector<Vec2d> det0;	// initial position of detectors
	vector<double> angles;	// rotation angles
	Mat P;					// projection
	SystemMatrix C;			// system matrix	Cf = P
	CTPixel* pixelInfo;	// N * N CTPixels
	CTPixel fullImage;	// FOV
	ImageInfo imginfo;	

	void generatePixelMap();	// fill out pixelInfo
	void getIndex(const Vec2d& pos, const Line& l, size_t& i, size_t& j);
public:
	CTSystem(const Vec2d& s, const vector<Vec2d>& d, const vector<double>& a, 
		double* prj, unsigned short dim = 2, unsigned int* sz = NULL, double* fov = NULL);
	int generateSystemMatrix();

	// reconstruction algorithm, return run time(ms)
	int art(const Mat& f0 /*initial*/, Mat& I, unsigned int N = 10/*iteration times*/
		, double lambda = 1.0/*relaxation factor*/);
	int conjugategradient();
	int arttv();
	int arttv(const Mat& Ipriori);	// ART-TV with priori information
};


