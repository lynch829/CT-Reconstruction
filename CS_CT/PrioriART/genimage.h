// general medical image class, pixel class
/* Huayu Zhang, Dec 2014 */
// need OpenCV

#pragma once

#include<cstdlib>
#include<string>
#include<cstring>
#include<vector>
#include<opencv2\core\core.hpp>
#include "gui.h"

using namespace std;
using namespace cv;

// general medical image
/*class GeneralMedicalImage: public Mat{
protected:
	unsigned short dim;	// dim
	vector<double> FOV;   // field of view:mm
	vector<unsigned int> size;	    // matrix size
	void convert2Byte(Mat& byteMat);	// for show
public:
	// constructor and destructor
	GeneralMedicalImage(unsigned short d = 2, unsigned int* s = NULL, double* fov = NULL);
	~GeneralMedicalImage(){}
	// get and set
	unsigned short getDim(){ return dim; }
	vector<double> getFOV(){ return FOV; }
	double getFOV(unsigned short i){ assert(i < dim); return FOV[i]; }
	vector<unsigned int> getSize(){ return size; }
	size_t getSize(unsigned short i){ assert(i < dim); return size[i]; }
	// display
	virtual void show2D(string window, int flags = WINDOW_AUTOSIZE);
	//virtual void show3D();
	//
	virtual GeneralMedicalImage& operator=(GeneralMedicalImage& other);
};*/

enum PixelSide{ BOTTOM, UPPER, LEFT, RIGHT, LEFT_BOTTOM, RIGHT_BOTTOM, LEFT_UPPER, RIGHT_UPPER };
enum PixelVertex{ LEFTBOTTOM, RIGHTBOTTOM, LEFTUPPER, RIGHTUPPER };

// pixel 
class Pixel
{
protected:
	double slength;				// side length
	Vec2d Plb, Prb, Plu, Pru; // left bottom, right bottom, left upper, right upper
	void updateCoordinate();	// update coordinate 
public:
	// constructor
	Pixel(){};
	Pixel(Vec2d lb, double sl);
	~Pixel(){};
	// get and set
	void setLeftBottom(Vec2d newLb);
	void setSideLength(double newSl);
	double getSideLength(){ return slength; }
	Vec2d getVertex(PixelVertex pv);
};