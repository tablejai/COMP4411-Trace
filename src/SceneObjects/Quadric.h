#pragma once
#include <vector>
#include "../scene/scene.h"
#include <string>

class Quadric : public MaterialSceneObject
{

	string type = "";
public:
	Quadric(Scene* scene, Material* mat,string name);
	~Quadric();

	bool intersectLocal(const ray& r, isect& i) const override;

};

