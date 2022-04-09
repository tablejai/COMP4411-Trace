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
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	isect I = isect();
	vec3f result = { 1, 1, 1 };
	ray ry(P, getDirection(P));
	vec3f Q = ry.at(I.t);

	double distance = P.distance(P);

	while (scene->intersect(ry, I)) {
		if ((distance - I.t) < RAY_EPSILON)
		{
			return vec3f{ 0,0,0 };
		}
		Q = ry.at(I.t);
		ry = ray(Q, ry.getDirection());
		result[0] *= (I.getMaterial().kt)[0];
		result[1] *= (I.getMaterial().kt)[1];
		result[2] *= (I.getMaterial().kt)[2];
	}
	return result;
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
	//cout << c << "," << l << "," << q << endl;
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
	ray ry(position,(position-P).normalize());
	isect I;
	double distance = P.distance(position);
	vec3f Q = ry.at(I.t);
	vec3f d = ry.getDirection();
	vec3f result = { 1, 1, 1 };
	int i = 0;
	while (scene->intersect(ry, I)) {
		i++;
		cout << i << endl;
		/*if ((distance -I.t) < RAY_EPSILON) 
		{
			cout << "xxx" << endl;
			return vec3f{0,0,0};
		}*/
		Q = ry.at(I.t);
		ry = ray(Q, d);
		result[0]*=(I.getMaterial().kt)[0];
		result[1] *= (I.getMaterial().kt)[1];
		result[2] *= (I.getMaterial().kt)[2];
		cout << (I.getMaterial().kt)[1] << endl;
	}

	/*if (I.getMaterial() != nullptr)
	else
		cout << "null" << endl;*/
    return result;
}

double SpotLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE
	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	double dist = (P-position).length();
	//cout << c << "," << l << "," << q << endl;
	return min(1, 1 / (c + l * dist + q* dist * dist));
}

vec3f SpotLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}
vec3f SpotLight::getColor() const
{
	return color;
}

vec3f SpotLight::getDirection( const vec3f& P) const
{
	return (position - P).normalize();
}
vec3f SpotLight::getDirection() const
{
	return vec3f(0,0,0);
}


vec3f SpotLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	ray ry(position,(position-P).normalize());
	isect I;
	double distance = P.distance(position);
	vec3f Q = ry.at(I.t);
	vec3f d = ry.getDirection();
	vec3f result = { 1, 1, 1 };
	int i = 0;
	while (scene->intersect(ry, I)) {
		i++;
		cout << i << endl;
		/*if ((distance -I.t) < RAY_EPSILON) 
		{
			cout << "xxx" << endl;
			return vec3f{0,0,0};
		}*/
		Q = ry.at(I.t);
		ry = ray(Q, d);
		result[0]*=(I.getMaterial().kt)[0];
		result[1] *= (I.getMaterial().kt)[1];
		result[2] *= (I.getMaterial().kt)[2];
		cout << (I.getMaterial().kt)[1] << endl;
	}

	/*if (I.getMaterial() != nullptr)
	else
		cout << "null" << endl;*/
    return result;
}
