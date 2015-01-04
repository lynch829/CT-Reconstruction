/* CT/SPECT system
 * include image, system matrix and projection
 * source, detectors, angles 
 * Huayu Zhang, Dec 2014 
 */

#pragma once

#include<unordered_map>
#include <map>
#include <cassert>
#include "ctimage.h"

using namespace std;

enum ProjectionType{FAN, PARALLEL};	

// parameters of a Gaussian kernel
struct Gaussian{
	Size ksize;	// kernel size
	double sigmaX, sigmaY;	
};

// Machine's geometric informations
struct SystemInfo{
	double dist_os;		//distance between source and object
	double dist_od;		//distance between the detector array and object
	double l_det;		//the length of the detector array
	double l_model;		//length of model
	unsigned int Ndet;	//number of detectors
	unsigned int Nang;		//number of angles
	double as, ae;		// (as,ae)
	unsigned int N;		//image size
	double* angs;		// rotation ang arrays
	~SystemInfo(){ delete[] angs; }
	void calculateAngs();
	void selectAngle(size_t start, size_t end, size_t step, vector<double>& angles);
	double* selectProject(size_t start, size_t end, size_t step, double* P);
	void pinpoint(vector<Vec2d>& srcs, vector<Vec2d>& dets, ProjectionType prjtype);// calculate coordinate of source and detectors
	void calculate(size_t*& s, double*& fov, unsigned short dim);
};


// FOV, size, dimension
struct ImageInfo{
	unsigned short dim;	// dim
	vector<double> FOV;   // field of view:mm
	vector<size_t> size;	    // matrix size
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
public:
	SystemMatrix(int m, int n) :rows(m), cols(n){ index = new ROW[rows]; }
	~SystemMatrix();
	// get and set
	void insertOrChange(int i, int j, double v);
	double at(int i, int j) const;
	// matrix operations
	void transpose(SystemMatrix& At);
	void mulWith(const Mat& f, Mat& P);
	double oneRowMulWith(int i/*row id*/, const Mat& f);
	double squaredNorm(int i);
	double sumRow(int i);
	void sumRows(Mat& S);
	ROW* getRow(int i);
	//operator
	SystemMatrix& operator=(const SystemMatrix& C);
	// print
	friend void print(string varname, const SystemMatrix& sm, int Nang, int Ndet, int M, int N);	// for debug
	friend void print(const SystemMatrix& sm, int Nang, int Ndet, int M, int N);
	friend void print(string varname, const SystemMatrix& sm);
	friend void print(const SystemMatrix& sm);
};

class CTSystem{
protected:
	//CTImage image;		// image
	vector<Vec2d> src0;			// initial position of the source 
	vector<Vec2d> det0;	// initial position of detectors
	vector<double> angles;	// rotation angles
	SystemMatrix C;			// system matrix	Cf = P
	SystemMatrix CT;
	CTPixel* pixelInfo;	// N * N CTPixels
	CTPixel fullImage;	// FOV
	ImageInfo imginfo;	

	void generatePixelMap();	// fill out pixelInfo
	void getIndex(const Vec2d& pos, const Line& l, size_t& i, size_t& j);
public:
	CTSystem(const vector<Vec2d>& s, const vector<Vec2d>& d, const vector<double>& a
		, unsigned short dim = 2, size_t* sz = NULL, double* fov = NULL);
	//~CTSystem(){ if(pixelInfo) delete[] pixelInfo; }
	CTSystem(const CTSystem& cts);
	int generateSystemMatrix();
	void getImageInfo(ImageInfo& img_info);

	// reconstruction algorithm, return run time(ms)
	int art(const Mat& P, const Mat& f0 /*initial*/, Mat& I, unsigned int N = 10/*iteration times*/
		, double lambda = 1.0/*relaxation factor*/);
	int MLEM(const Mat& P, Mat& I, const Mat& f0/*initial*/, unsigned int N = 10/*iteration times*/
		, double tol = 1e-6);
	int MLEM_gaussian(const Mat& P, Mat& I, const Mat& f0/*initial*/, Gaussian gs,
		unsigned int N = 10/*iteration times*/, double tol = 1e-6); // MLEM with Gaussian filter
	int MLEM_median(const Mat& P, Mat& I, const Mat& f0/*initial*/, int md = 3,
		unsigned int N = 10/*iteration times*/, double tol = 1e-6); // MLEM with Gaussian filter
};


