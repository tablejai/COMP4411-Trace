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
	return intersectBody(r, i);
}



bool Box::intersectBody(const ray& r, isect& i) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();
	double tmax;
	double tmin;
	b.intersect(r,tmax,tmin);
	vec3f q;
	float tolerance = 0.0000001;
	
	if (tmin > 0.5)
	{
		q = { p[0] + tmin * d[0],p[1] + tmin * d[1] ,p[1] + tmin * d[1] };
		i.t = tmin;
	}
	else if (tmax > 0.5)
	{
		 q = { p[0] + tmax * d[0],p[1] + tmax * d[1] ,p[1] + tmax * d[1] };
		 i.t = tmax;
	}
	else {
		return false;
	}
	if (q[2] < b.min[2]-tolerance || q[1] < b.min[1] - tolerance || q[0] < b.min[0] - tolerance)
		return false;
	if (q[2]>b.max[2] + tolerance || q[1] > b.max[1]+ tolerance || q[0] > b.max[0]+ tolerance)
		return false;
	
	return true;
}

