/* Implementation of CTImage class
* Huayu Zhang, Dec 2014
*/

#include "ctimage.h"

bool CTPixel::intersectWith(const Line& l){
	bool inters = false;
	Vec2d Pi[2];
	switch (parallelTo(l))
	{
	case X:			// l // x
		if (Plb(1) <= l.P(1) && l.P(1) < Plu(1)){
			inter_set = new Intersect[2];
			Pi[0] = Vec2d(Plb(0), l.P(1));
			Pi[1] = Vec2d(Prb(0), l.P(1));
			inter_set[0] = Intersect(Pi[0], LEFT);
			inter_set[1] = Intersect(Pi[1], RIGHT);
			c = slength; 
			inters = true;
		}
		break;
	case Y:			// l // y
		if (Plb(0) <= l.P(0) && l.P(0) < Prb(0)){
			inter_set = new Intersect[2];
			Pi[0] = Vec2d(l.P(0), Plb(1));
			Pi[1] = Vec2d(l.P(0), Plu(1));
			inter_set[0] = Intersect(Pi[0], BOTTOM);
			inter_set[1] = Intersect(Pi[1], UPPER);
			c = slength;
			inters = true;
		}
		break;
	case NONE:
		// slope > 0
		if (l.b < 0){			
			if (distance(Plu, l) < -eps && distance(Prb, l) > eps){
				inter_set = new Intersect[2];
				// f(LU) < 0 , f(RB) > 0
				// BOTTOM-LEFT
				if (distance(Plb, l) < 0){	// f(LB) < 0
					Pi[0](1) = Plb(1);
					Pi[0](0) = -(l.b * Pi[0](1) + l.c) / l.a;
					inter_set[0] = Intersect(Pi[0], BOTTOM);
				}
				else if (distance(Plb, l) == 0){
					Pi[0] = Plb;
					inter_set[0] = Intersect(Plb, LEFT_BOTTOM);
				}
				else{
					Pi[0](0) = Plb(0);
					Pi[0](1) = -(l.a * Pi[0](0) + l.c) / l.b;
					inter_set[0] = Intersect(Pi[0], LEFT);
				}

				// UPPER-RIGHT
				if (distance(Pru, l) < 0){	// f(RU) < 0
					Pi[1](0) = Pru(0);
					Pi[1](1) = -(l.a * Pi[1](0) + l.c) / l.b;
					inter_set[1] = Intersect(Pi[1], RIGHT);
				}
				else if(distance(Pru, l) == 0){
					Pi[1] = Pru;
					inter_set[1] = Intersect(Pru, RIGHT_UPPER);
				}
				else{
					Pi[1](1) = Pru(1);
					Pi[1](0) = -(l.b * Pi[1](1) + l.c) / l.a;
					inter_set[1] = Intersect(Pi[1], UPPER);
				}
				c = norm(Pi[0], Pi[1], NORM_L2);
				inters = true;
			}
		}
		// slope < 0
		else{
			if (distance(Plb, l) < -eps && distance(Pru, l) > eps){
				inter_set = new Intersect[2];
				// f(LB) < 0 , f(RU) > 0
				// UPPER-LEFT
				if (distance(Plu, l) < 0){	// f(LU) < 0
					Pi[0](1) = Plu(1);
					Pi[0](0) = -(l.b * Pi[0](1) + l.c) / l.a;
					inter_set[0] = Intersect(Pi[0], UPPER);
				}
				else if (distance(Plu, l) == 0){
					Pi[0] = Plu;
					inter_set[0] = Intersect(Plu, LEFT_UPPER);
				}
				else{
					Pi[0](0) = Plu(0);
					Pi[0](1) = -(l.a * Pi[0](0) + l.c) / l.b;
					inter_set[0] = Intersect(Pi[0], LEFT);
				}

				// BOTTOM_RIGHT
				if (distance(Prb, l) < 0){	// f(RB) < 0
					Pi[1](0) = Prb(0);
					Pi[1](1) = -(l.a * Pi[1](0) + l.c) / l.b;
					inter_set[1] = Intersect(Pi[1], RIGHT);
				}
				else if (distance(Prb, l) == 0){
					Pi[1] = Prb;
					inter_set[1] = Intersect(Prb, RIGHT_BOTTOM);
				}
				else{
					Pi[1](1) = Prb(1);
					Pi[1](0) = -(l.b * Pi[1](1) + l.c) / l.a;
					inter_set[1] = Intersect(Pi[1], BOTTOM);
				}
				c = norm(Pi[0], Pi[1], NORM_L2);
				inters = true;
			}
		}
		break;
	default:
		break;
	}
	return inters;
}

Intersect CTPixel::getIntersect(int i){
	assert(-1 < i && i < 2);
	return inter_set[i];
}
