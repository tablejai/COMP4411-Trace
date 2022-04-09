// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include  "ui/TraceUI.h"

# define M_PI           3.14159265358979323846  /* pi */
#define Map(A,i,j,W)  *((char*)A+(i+ 3 * (W)*j))

extern TraceUI* traceUI;
// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f intersectPlane(Scene* scene, ray r,vec3f n, vec3f p0) {
	vec3f dir = r.getDirection();
	float denom = n.dot(dir);
	float t = 0;
	vec3f target(0,0,0);
	if (denom > 1e-6) {
		vec3f p = p0 - r.getPosition();
		t = p.dot(n) / denom;
		target = t * r.getDirection() + r.getPosition();
	}
	return target;
}
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
	vec3f n = scene->getCamera()->look.normalize();
	vec3f p0 = n * dist;
	ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
	scene->getCamera()->rayThrough(x, y, r);
	if(x==0&&y==0)
		 origin = intersectPlane(scene,r,n,p0);
	if (x >= 0.98 && y>=0.98) {
		vec3f ldir = scene->getCamera()->u.normalize();//idont konw
		vec3f udir = scene->getCamera()->u.cross(scene->getCamera()->look).normalize(); //it wierd but is fact
		ray topline = ray(origin, -ldir);
		ray rightline = ray(origin, -udir);
		vec3f target  = intersectPlane(scene, r, n, p0);
		hbackground = topline.getDistance(target);
		wbackground = rightline.getDistance(target);
		cout << wbackground << "," << hbackground << endl;
	}
	return traceRay( scene, r, vec3f(1.0,1.0,1.0), traceUI->getDepth()).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
bool TIR(ray r, isect i, double n_i, double n_t) {
	return (
		pow(i.N.normalize().dot(r.getDirection().normalize()), 2) <=1 - pow(n_t / n_i, 2));
}
vec3f retractDirection(ray r, isect i, double n_i, double n_t, bool flipNormal) {
	vec3f ret(0, 0, 0);
	vec3f n = i.N;
	if (flipNormal) { n *= -1; }
	vec3f v = r.getDirection().normalize();

	for (int i = 0; i < 3; i++) {
		ret[i] = n_i / n_t * (
			(sqrt(pow(n.dot(v), 2) + pow(n_t / n_i, 2) - 1) - n.dot(v)) * n[i] + v[i]
			);
	}
	return ret;
}
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth )
{


	isect i;
	if (depth == -1)
	{
		return vec3f(0.0, 0.0, 0.0);
	};
	double n_i;
	double n_t;
	if( scene->intersect( r, i ) ) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.
		const Material& m = i.getMaterial();
		vec3f I = m.shade(scene, r, i);
		vec3f N = i.N;
		vec3f Li = -r.getDirection();
		vec3f R = (2 * N.dot(Li) * N - Li);
		R = R.normalize();
		ray r2 = ray(r.at(i.t), R);
	  I += (m.kr.multEach(traceRay(scene, r2, thresh, depth - 1)));
	   double angle;
	   bool normal_flip;
	   if (r.getDirection().dot(i.N)<0) { //into
		   n_i =1;
		   n_t = m.index;
		   normal_flip = false;
	   }
	   else {
		   n_i = m.index;
		   n_t = 1;
		   normal_flip = true;

	   }
	  if (m.kt[0]>0 && m.kt[1] > 0 && m.kt[2] > 0 &&!TIR(r,i,n_i,n_t)) {
		vec3f col;
		if (1.0f - scene->threshold < I.length())
		{
			const ray reflectRay(r.at(i.t), r.getDirection() - 2 * r.getDirection().dot(i.N) * i.N);
			col = traceRay(scene, reflectRay, thresh, depth + 1);
			col = prod(col, m.kr);
			vec3f N = i.N;
			vec3f Li = -r.getDirection();
			vec3f R = (2 * N.dot(Li) * N - Li);
			R = R.normalize();
			ray r2 = ray(r.at(i.t), R);
			I += (m.kr.multEach(traceRay(scene, r2, thresh, depth - 1)));
			double angle;
			bool normal_flip;
			if (r.getDirection().dot(i.N)<0) { //into
				n_i =1;
				n_t = m.index;
				normal_flip = false;
			}
			else {
				n_i = m.index;
				n_t = 1;
				normal_flip = true;
			}
		}



	  /* if (m.kt[0]>0 && m.kt[1] > 0 && m.kt[2] > 0 &&!TIR(r,i,n_i,n_t)) {
		   vec3f normN = N.normalize();
		   vec3f dir = r.getDirection().normalize();
		   if (normal_flip)
			   normN = { -normN[0],-normN[1],-normN[2] };
		   vec3f T = (n_i / n_t) * (normN.cross(-normN.cross(dir))) -  sqrt(1 - pow(n_i / n_t, 2) * (normN.cross(dir).dot(normN.cross(dir))))*normN;
		   ray r3 = ray(r.at(i.t), T.normalize());
		   
		   I += (m.kt.multEach(traceRay(scene, r3, thresh, depth-1)));
	   }
	  return I.clamp();

*/
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.
		vec3f n = scene->getCamera()->look.normalize();
		/*bool intersectPlane(const Vec3f & n, const Vec3f & p0, const Vec3f & l0, const Vec3f & l, float& t)
		{*/
		// assuming vectors are all 
		bool intersect = true;
		vec3f dir = r.getDirection();
		float denom = n.dot(dir);
		float t = 0;
		vec3f target;
		vec3f p0 = n * dist;
		if (denom > 1e-6) {
			vec3f p = p0 - r.getPosition();
			t = p.dot(n) / denom;
			target = t * r.getDirection() + r.getPosition();
		}
		vec3f ldir = scene->getCamera()->u.normalize();//idont konw
		vec3f udir = scene->getCamera()->u.cross(scene->getCamera()->look).normalize(); //it wierd but is fact
		ray rightline = ray(origin, -udir);
		ray topline = ray(origin, -ldir);
		double getcol = rightline.getDistance(target) / wbackground;
		double getrow = topline.getDistance(target) / hbackground;
		if (backgroundImage != nullptr) {
			//cout << 3 * (int)(getcol * m_width) << "| " << (int)getrow * m_height << endl;
			if (getcol < 1.0 && getrow < 1.0) {
				int col = (int)(getcol * m_width);
				int row = (int)(getrow * m_height);
				double r = backgroundImage[3 * (col + row * m_width)] / 255.0;
				double g = backgroundImage[3 * (col + row * m_width) + 1] / 255.0;
				double b = backgroundImage[3 * (col + row * m_width) + 2] / 255.0;
				return {r,g,b};
			}

		}
		return vec3f(0, 0, 0);

	
	}


	
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}


void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[bufferSize];
	}
 
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;
	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}