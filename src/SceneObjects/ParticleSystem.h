#pragma once
#include <vector>
#include "../scene/scene.h"
using namespace std;
#define abs(x) ((x)>0?x:-x)
# define M_PI           3.14159265358979323846  /* pi */

typedef struct
{
	vec3f position;
	vec3f velocity;
	vec3f color;
	double decay;
} Particle;

class ParticleSystem : public MaterialSceneObject
{
public:

	ParticleSystem(Scene* scene, Material* mat);
	~ParticleSystem();
	std::vector<Particle> pts;

	Particle createParticle();

	bool intersectLocal(const ray& r, isect& i) const override;
	vec3f pos, dir, accel, colorInit, u, v;
	double vmin, vmax, dmin, dmax;
	int frameNum, count;
	
	bool intersectParticle(const vec3f& q, const vec3f& p,  vec3f& d ,double &diff)const
	{
		vec3f pq = (q - p).normalize();
		 d = d.normalize();
		double cosV = (pq * d);
		double theta = acos(cosV);
		diff = theta;
		return ((-0.007) < (theta) && (theta) < 0.007);
	}


};

