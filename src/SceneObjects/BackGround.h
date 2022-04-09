#ifndef _BackGround_H__
#define _BackGround_H__
#include "../scene/scene.h"

class BackGround
	: public MaterialSceneObject
{
public:
	BoundingBox b;
	BackGround(Scene* scene, Material* mat)
		: MaterialSceneObject(scene, mat)
	{
		b = ComputeLocalBoundingBox();
	}
	bool capped; //not sure what it means
	unsigned char * bitmap;
	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool intersectBody(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return true; }
	virtual BoundingBox ComputeLocalBoundingBox()
	{
		BoundingBox localbounds;
		localbounds.max = vec3f(0.5, 0.5, 0.5);
		localbounds.min = vec3f(-0.5, -0.5, -0.5);
		return localbounds;
	}
};
#endif
