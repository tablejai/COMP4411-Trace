
#include "ParticleSystem.h"
int IntegerRand(int min, int max)
{
	return rand() % (max - min) + min;
}
double doubleRand(double low, double high)
{
	return (double)rand() / RAND_MAX * (high - low) + low;
}
#define parameter(ps,d,ui,vi,a,c,vmn,vmx,dmx,fNum,dm,ct)\
	pos =ps; dir =d;u = ui; v = vi;accel = a;colorInit = c;vmin = vmn;vmax = vmx;dmax = dmx;frameNum = fNum;dmin = dm;count = ct;
ParticleSystem::ParticleSystem(Scene* scene, Material* mat)
	: MaterialSceneObject(scene, mat)
{
	parameter(vec3f( 0,0,0 ), vec3f( 0,1,0 ), vec3f( 1,0,0 ), vec3f( 0,0,1 ), vec3f( 0,-0.009,0 ), vec3f( 0,0,1 ), 0.02, 0.1, 1, 50, 1, 2000);
	pts.clear();
	for (int i = 0;i < count; i++)
	{
		Particle pt = createParticle();
		int dt = IntegerRand(0, frameNum);
		pt.position += accel * dt * dt / 2.0+ pt.velocity * dt;
		pt.color *= pow(pt.decay, dt);
		pts.push_back(pt);
	}

}
ParticleSystem::~ParticleSystem()
{
}

Particle ParticleSystem::createParticle()
{
	vec3f position = pos + doubleRand(-0.07, 0.07) * u + doubleRand(-0.06, 00.6) * v;
	vec3f velocity =  doubleRand(vmin, vmax)* (dir + doubleRand(-0.27, 0.27) * u + doubleRand(-0.27, 0.27) * v);
	double d = doubleRand(dmin, dmax);
	int r = IntegerRand(0, 3);
	vec3f color;

	if (r==1) {
		color = { 1,1,1 };
	}
	else if (r==2) {
		color = colorInit;
	}
	color *= doubleRand(0.5, 1);
	Particle particle = { position, velocity, color,d};
	return particle;
}


bool ParticleSystem::intersectLocal(const ray& r, isect& i) const
{
	vec3f d = r.getDirection();
	bool intersect = false;
	vec3f p = r.getPosition();
	vec3f color(0, 0, 0);
	for (auto& dot : pts)
	{
		if (intersectParticle(dot.position, p, d)) {
			intersect = true;
			color = dot.color;
		}
	}
	if (intersect)
	{
		Material* m = new Material;
		m->ke = color;
		i.t = 1000;
		i.N = -d;
		i.material = m;
	}
	return intersect;
}