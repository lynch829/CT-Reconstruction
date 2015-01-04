/* CT/SPECT image
 * Include intersection information
 * weight factor in each pixel
 * Huayu Zhang, Dec 2014
 */

#pragma once
#include "genimage.h"
#include "geometry.h"

// record intersection
struct Intersect{
	Vec2d Pt;	/* intersect point */
	PixelSide pside; /* at which side Pt is located */
	Intersect(){}
	Intersect(Vec2d pt, PixelSide ps) :Pt(pt), pside(ps){}
	Intersect& operator=(Intersect& other){	// assign value
		Pt = other.Pt; pside = other.pside;
		return *this;
	}
};

class CTPixel : public Pixel{
protected:
	Intersect* inter_set;      // set of intersect points (LOR,Side)  (x1 < x2)
	double c;					// weight factor
public:
	CTPixel():c(0){}
	CTPixel(Vec2d lb, double sl) :Pixel(lb, sl), inter_set(NULL),c(0){}
	bool intersectWith(const Line& l);	// projection line
	Intersect getIntersect(int i); // i \in [0,2)
	double getWeight(){ return c; }
};