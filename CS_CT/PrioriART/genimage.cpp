#include "genimage.h"
#include<iostream>

void Pixel::updateCoordinate(){
	Prb = Plb + Vec2d(slength, 0);
	Plu = Plb + Vec2d(0, slength);
	Pru = Plb + Vec2d(slength, slength);
}

Pixel::Pixel(Vec2d lb, double sl) :Plb(lb), slength(sl){
	updateCoordinate();
}
void Pixel::setLeftBottom(Vec2d newLb) {
	Plb = newLb;
	updateCoordinate();
}

void Pixel::setSideLength(double newSl){
	slength = newSl;
	updateCoordinate();
}

Vec2d Pixel::getVertex(PixelVertex pv){
	Vec2d v;
	switch (pv)
	{
	case LEFTBOTTOM:
		v = Plb;
		break;
	case RIGHTBOTTOM:
		v = Prb;
		break;
	case LEFTUPPER:
		v = Plu;
		break;
	case RIGHTUPPER:
		v = Pru;
		break;
	default:
		break;
	}
	return v;
}