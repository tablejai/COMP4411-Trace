#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"
#include <iostream>
using namespace std;
class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getColor() const = 0;
	virtual vec3f getDirection(  ) const = 0;
	virtual vec3f getDirection(const vec3f& P)const =0;
protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor() const;
	virtual vec3f getColor(const vec3f& p) const;
	virtual vec3f getDirection() const;
	virtual vec3f getDirection(const vec3f& P)const ;

protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
	

public:
	double c=0;
	double l=0;
	double q=0;
	PointLight( Scene *scene, const vec3f& pos, const vec3f& color )
		: Light( scene, color ), position( pos ){
		cout << c << endl;
		cout << l << endl;
		cout << q << endl;
	}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor() const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection(const vec3f& P)const;
	virtual vec3f getDirection()const;

protected:
	vec3f position;
};

class SpotLight
	: public PointLight
{
public:
	double size;
	double blend;
	double theta;
	double phi;
	vec3f dir;

	SpotLight( Scene *scene, const vec3f& pos, const vec3f& color, const vec3f& dir, double size, double blend)
		: PointLight(scene, pos, color), dir(dir), size(size), blend(blend){
		theta = cos(size / 2);
		phi = cos(size * (1 - blend) / 2);
	}

	vec3f shadowAttenuation(const vec3f& P) const;
	double effect(double dir)const ;

protected:
	vec3f position;

};
#endif // __LIGHT_H__
