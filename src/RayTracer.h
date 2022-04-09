#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    vec3f trace( Scene *scene, double x, double y );
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth );


	void getBuffer( unsigned char *&buf, int &w, int &h );
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );

	bool loadScene( char* fn );

	bool sceneLoaded();
	unsigned char* buffer;
	double dist = 2;
	unsigned char* backgroundImage=nullptr;
	int m_width=0;
	int m_height=0;
	double wbackground = 0;
	double hbackground = 0;

private:
	int buffer_width, buffer_height;
	int bufferSize;

	Scene *scene;
	vec3f origin;//background
	bool m_bSceneLoaded;
};

#endif // __RAYTRACER_H__
