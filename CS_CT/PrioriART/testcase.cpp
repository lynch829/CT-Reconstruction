#include "testcase.h"
#include "io.h"

void demo1(){
	// data
	string prjfile = "data" ,dir = "cube300mm.prj";
	SystemInfo sysinfo;
	Vec2d s;			// source
	vector<Vec2d> d;	// detectors
	vector<double> a;	// angles
	double* P, *Ps;			// projection
	size_t angle_start, angle_end, angle_step;
	unsigned short dim = 2;
	size_t* sz;
	double* fov;
	// set system parameters
	sysinfo.dist_od = 1200.0;
	sysinfo.dist_os = 1200.0;
	sysinfo.l_det = 1600.0;
	sysinfo.Ndet = 10;
	sysinfo.Nang = 360;
	sysinfo.N = 8;
	sysinfo.l_model = 800.0;
	angle_start = 0;
	angle_end = 360;
	angle_step = 45;
	// get detectors and source position, angles
	sysinfo.pinpoint(s, d);
	sysinfo.calculateAngs();
	sysinfo.selectAngle(angle_start, angle_end, angle_step, a);
	sysinfo.calculate(sz, fov, dim);
	// read projection data
	readProjection(prjfile, dir, P, sysinfo.Nang, sysinfo.Ndet);
	Ps = sysinfo.selectProject(angle_start, angle_end, angle_step, P);
	// reconstruction
	CTSystem CTS1(s, d, a, Ps, dim, sz, fov);
	Mat I;
	Mat f0 = Mat::zeros(sz[0], sz[1], CV_64F);
	CTS1.generateSystemMatrix();
	CTS1.art(f0, I, 10);
	//print("result:", I);
	show2Dimg(I, "8*8 square", WINDOW_NORMAL);
}

void demo2(){
	// data
	string prjfile = "data", dir = "cylinder.prj";
	SystemInfo sysinfo;
	Vec2d s;			// source
	vector<Vec2d> d;	// detectors
	vector<double> a;	// angles
	double* P, *Ps;			// projection
	size_t angle_start, angle_end, angle_step;
	unsigned short dim = 2;
	size_t* sz;
	double* fov;
	// set system parameters
	sysinfo.dist_od = 600.0;
	sysinfo.dist_os = 600.0;
	sysinfo.l_det = 1440.0;
	sysinfo.Ndet = 72;
	sysinfo.Nang = 360;
	sysinfo.N = 128;
	sysinfo.l_model = 600.0;
	angle_start = 0;
	angle_end = 360;
	angle_step = 1;
	// get detectors and source position, angles
	sysinfo.pinpoint(s, d);
	sysinfo.calculateAngs();
	sysinfo.selectAngle(angle_start, angle_end, angle_step, a);
	sysinfo.calculate(sz, fov, dim);
	// read projection data
	readProjection(prjfile, dir, P, sysinfo.Nang, sysinfo.Ndet);
	Ps = sysinfo.selectProject(angle_start, angle_end, angle_step, P);
	// reconstruction
	CTSystem CTS1(s, d, a, Ps, dim, sz, fov);
	Mat I;
	Mat f0 = Mat::zeros(sz[0], sz[1], CV_64F);
	CTS1.generateSystemMatrix();
	CTS1.art(f0, I, 30);
	//print("result:", I);
	show2Dimg(I, "64*64 square", WINDOW_NORMAL);
}


void demo3(){
	// data
	string prjfile = "data", dir = "csrbigdata.prj";
	SystemInfo sysinfo;
	Vec2d s;			// source
	vector<Vec2d> d;	// detectors
	vector<double> a;	// angles
	double* P, *Ps;			// projection
	size_t angle_start, angle_end, angle_step;
	unsigned short dim = 2;
	size_t* sz;
	double* fov;
	// set system parameters
	sysinfo.dist_od = 1200.0;
	sysinfo.dist_os = 1200.0;
	sysinfo.l_det = 2400.0;
	sysinfo.Ndet = 400;
	sysinfo.Nang = 360;
	sysinfo.N = 256;
	sysinfo.l_model = 800.0;
	angle_start = 0;
	angle_end = 216;
	angle_step = 1;
	// get detectors and source position, angles
	sysinfo.pinpoint(s, d);
	sysinfo.calculateAngs();
	sysinfo.selectAngle(angle_start, angle_end, angle_step, a);
	sysinfo.calculate(sz, fov, dim);
	// read projection data
	readProjection(prjfile, dir, P, sysinfo.Nang, sysinfo.Ndet);
	Ps = sysinfo.selectProject(angle_start, angle_end, angle_step, P);
	// reconstruction
	CTSystem CTS1(s, d, a, Ps, dim, sz, fov);
	Mat I;
	Mat f0 = Mat::zeros(sz[0], sz[1], CV_64F);
	CTS1.generateSystemMatrix();
	CTS1.art(f0, I, 30);
	show2Dimg(I, "256*256 square", WINDOW_NORMAL);
}