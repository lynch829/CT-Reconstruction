// pixel class
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