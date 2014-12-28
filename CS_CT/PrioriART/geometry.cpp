#include "geometry.h"

// struct Line
void Line::computeLineEq(){
	if (parallelTo(*this) == X){
		a = 0;
		b = 1;
		c = -b * this->P(1);
	}
	else if (parallelTo(*this) == Y){
		a = 1;
		b = 0;
		c = -a * this->P(0);
	}
	else{
		Vec2d tau = normalize(Q - P);
		a = -tau(1); b = tau(0);
		c = -(a*Q(0) + b*Q(1));
		if (a < 0){ a = -a; b = -b; c = -c; }
	}
}

Line::Line(const Vec2d& P1, const Vec2d& P2){
	if (P1(0) < P2(0)){
		P = P1; Q = P2;
	}
	else{
		P = P2; Q = P1;
	}
	computeLineEq();
}

Line& Line::operator=(const Line& other){
	P = other.P;
	Q = other.Q;
	a = other.a;
	b = other.b;
	c = other.c;
	return *this;
}


// axis relationship
Axis parallelTo(const Vec2d& P, const Vec2d& Q){
	Axis axis;
	if (abs(P(0) - Q(0)) < eps) // Px = Qx
		axis = Y;
	else if (abs(P(1) - Q(1)) < eps)	// Py = Qy
		axis = X;
	else
		axis = NONE;
	return axis;
}

Axis parallelTo(const Line& l){
	return parallelTo(l.P, l.Q);
}


double distance(const Vec2d& P, const Line& l){
	double d = l.a * P(0) + l.b * P(1) + l.c;
	return d;
}


/* P' = R * P
* R = [cos(a) -sin(a); sin(a) cos(a)] */
Vec2d rotate(const Vec2d& p, double angle){
	double c = cos(angle), s = sin(angle);
	Vec2d pp;
	pp(0) = c * p(0) - s * p(1);
	pp(1) = s * p(0) + c * p(1);
	return pp;
}