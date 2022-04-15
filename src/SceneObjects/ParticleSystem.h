#pragma once
#include <vector>
#include "../scene/scene.h"
using namespace std;

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
	
	bool intersectParticle(const vec3f& q, const vec3f& p,  vec3f& d)const
	{
		vec3f pq = (q - p).normalize();
		 d = d.normalize();
		double cosV = (pq * d);
		double theta = acos(cosV);
		return ((-0.003) <(theta) && (theta) < 0.003);
	}

};

