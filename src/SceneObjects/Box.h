#ifndef __BOX_H__
#define __BOX_H__

#include "../scene/scene.h"

class Box
	: public MaterialSceneObject
{
public:
	BoundingBox b;
	Box( Scene *scene, Material *mat )
		: MaterialSceneObject( scene, mat )
	{
	

	b=	ComputeLocalBoundingBox();
	}
	bool capped; //not sure what it means
	
	virtual bool intersectLocal( const ray& r, isect& i ) const;
	virtual bool intersectBody(const ray& r, isect& i) const ;
	virtual bool hasBoundingBoxCapability() const { return true; }
    virtual BoundingBox ComputeLocalBoundingBox()
    {
        BoundingBox localbounds;
        localbounds.max = vec3f(0.5, 0.5, 0.5);
		localbounds.min = vec3f(-0.5, -0.5, -0.5);
        return localbounds;
    }
};

#endif // __BOX_H__
