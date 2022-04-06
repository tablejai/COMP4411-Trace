#include <cmath>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "Box.h"
using namespace std;
bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	
	i.obj = this;
    return intersectBody(r,i);
}



bool Box::intersectBody(const ray& r, isect& i) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();
	double tmax;
	double tmin;
	b.intersect(r,tmin,tmax);
	vec3f q;
	float tolerance = 0.001;
	vec3f isectPoint;
	if (tmin > 0.3)
	{
		q = { p[0] + tmin * d[0],p[1] + tmin * d[1] ,p[2] + tmin * d[2] };

	}
	else {
		return false;
	}
	i.t = tmin;
	isectPoint = r.at(i.t);
	if (fabs(isectPoint[0] - 0.5) <= tolerance) {
		i.N = { 1,0,0 };
	}
	else if (fabs(isectPoint[0] + 0.5) <= tolerance) {
		i.N = { -1,0,0 };
	}
	else if (fabs(isectPoint[1] - 0.5) <= tolerance) {
		i.N = { 0,1,0 };
	}
	else if (fabs(isectPoint[1] + 0.5) <= tolerance) {
		i.N = { 0,-1,0 };
	}
	else if (fabs(isectPoint[2] - 0.5) <= tolerance) {
		i.N = { 0,0,1 };
	}
	else if (fabs(isectPoint[2] + 0.5) <= tolerance) {
		i.N = { 0,0,-1 };
	}

	if (q[2] < b.min[2]-tolerance || q[1] < b.min[1] - tolerance || q[0] < b.min[0] - tolerance)
		return false;
	if (q[2]>b.max[2] + tolerance || q[1] > b.max[1]+ tolerance || q[0] > b.max[0]+ tolerance)
		return false;
	
	return true;
}

