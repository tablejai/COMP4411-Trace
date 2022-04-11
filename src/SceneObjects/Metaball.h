#pragma once
#include <vector>
#include "../scene/scene.h"


typedef struct {
	vec3f pos;
	double density;
	double radius;
}MBall;

typedef struct
{
	bool entry;
	MBall ball;
	double t;

} Intersect;

class Metaball : public MaterialSceneObject
{
public:
	Metaball(Scene* scene, Material* mat);
	~Metaball();
	std::vector<MBall> mballs;

	bool intersectLocal(const ray& r, isect& i) const override;
	bool intersectSphere(const MBall& ball, const ray& r, std::pair<double, double>& T) const;
	bool isPlane(const vector<MBall>& mballs, ray r, vec3f& N, double t) const;
	double getDensity(const vector<MBall>& mballs, vec3f pos) const;
	double threshold = 0.05;
};

