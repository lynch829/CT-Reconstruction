/* Geometry 
 * include functions about geometric computation 
 * Huayu Zhang, Dec 2014
 */
#pragma once
#include <opencv2\core\core.hpp>

using namespace cv;

const double eps = 0.0;

enum Axis{ X, Y, NONE };

struct Line{
	Vec2d P, Q;		// end points	Px < Qx
	double a, b, c;	// ax + by + c = 0	a,b has been normalized (a>=0)
	void computeLineEq();
	Line(){}
	Line(const Vec2d& P1, const Vec2d& P2);
	Line& operator=(const Line& other);
};

Axis parallelTo(const Vec2d& P, const Vec2d& Q);
Axis parallelTo(const Line& l);

double distance(const Vec2d& P, const Line& l);		
/* signed, <0 means P is on the left side of l
 * >0 means P is on the right side of l */


Vec2d rotate(const Vec2d& p, double angle);	// anti-clockwise