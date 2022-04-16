#include <cmath>

#include "light.h"
#define min(a,b)   (a<b)? a:b;
double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
	
	
	const vec3f& d = getDirection(P);
	vec3f r(1.0, 1.0, 1.0);
	// push the point outwards a bit so that the ray won't hit itself
	vec3f p = P + d * RAY_EPSILON;
	isect i;
	while (!r.iszero())
	{
		ray s(p, d);
		i = isect();
		if (!scene->intersect(s, i))
		{
			return r;
		}
		Material m = i.getMaterial();
		r = r.multEach(m.kt);
		p = s.at(i.t) + d * RAY_EPSILON;
	}
	return r;
}

vec3f DirectionalLight::getColor() const
{
	// Color doesn't depend on P 
	return color;
}
vec3f DirectionalLight::getColor(const vec3f& P) const {
	return color;
}

vec3f DirectionalLight::getDirection( ) const
{
	return -orientation;
}
vec3f DirectionalLight::getDirection(const vec3f& P) const
{
	return getDirection();
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE
	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	double dist = (P-position).length();
	cout << c << "," << l << "," << q << endl;
	return min(1, 1 / (c + l * dist + q* dist * dist));
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}
vec3f PointLight::getColor() const
{
	return color;
}

vec3f PointLight::getDirection( const vec3f& P) const
{
	return (position - P).normalize();
}
vec3f PointLight::getDirection() const
{
	return vec3f(0,0,0);
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
//	ray ry(position,(position-P).normalize());
//	isect I;
//	double distance = P.distance(position);
//	vec3f Q = ry.at(I.t);
//	vec3f d = ry.getDirection();
//	vec3f result = { 1, 1, 1 };
//	while (scene->intersect(ry, I)) {
//		/*if ((distance -I.t) < RAY_EPSILON) 
//		{
//			cout << "xxx" << endl;
//			return vec3f{0,0,0};
//		}*/
//		Q = ry.at(I.t);
//		ry = ray(Q, d);
//		result[0]*=(I.getMaterial().kt)[0];
//		result[1] *= (I.getMaterial().kt)[1];
//		result[2] *= (I.getMaterial().kt)[2];
////		cout << (I.getMaterial().kt)[1] << endl;
//	}
//
//	/*if (I.getMaterial() != nullptr)
//	else
//		cout << "null" << endl;*/
//    return result;

	double distance = P.distance(position);
	vec3f d = (position - P).normalize();
	ray r(P, d);
	vec3f atten = { 1, 1, 1 };
	vec3f tempP = P;
	isect isec;
	ray tempr(r);
	// recursively find intersection
	while (scene->intersect(tempr, isec)) {
		//printf("intersection\n");
		// intersection is not before light
		if ((distance -= isec.t) < RAY_EPSILON) { return atten; }
		// a totally un-transparent object
		/*cout << isec.getMaterial().kt[0] << ',' <<
			isec.getMaterial().kt[1] << ',' <<
			isec.getMaterial().kt[2] << endl;*/
		if (isec.getMaterial().kt.iszero()) return { 0,0,0 };
		tempP = tempr.at(isec.t);
		tempr = ray(tempP, d);
		atten = atten.multEach(isec.getMaterial().kt);
	}
	// std::cout << atten << std::endl;
	return atten;
}

double SpotLight::effect(double dir)const {
	if (dir < theta && dir >= phi) {
		double out_in_diff = theta - phi;
		double effect_output =  -2 * pow((dir - phi) / (out_in_diff), 3);
		effect_output += 3 * pow((dir - theta) / (out_in_diff), 2);
		return effect_output;
	}
	else if (dir >= theta) return 1.0f;
	else if (dir < phi) return 0.0f;
	return 0.0f;
}

vec3f SpotLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	vec3f dd = this->getDirection(P);
	vec3f atten = color * effect(dd.dot(-dir));;

	isect intersection;
	ray ray(P, dd);
	if (scene->intersect(ray, intersection))
	{
		atten = prod(intersection.getMaterial().kt, atten);
	}
	return atten;
}
