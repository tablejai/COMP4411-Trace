#include "ray.h"
#include "material.h"
#include "light.h"
#define max(a,b) ((a>b) ? a:b)
// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE


	vec3f I;
	vec3f targetPt = r.at(i.t) ;
	I = ke+ ka.multEach(scene->Ia);
	for (auto iter = scene->beginLights();iter != scene->endLights();iter  = next(iter,1)) {
		Light* light = *iter;

		//double intensity = 0.299 * light->getColor()[0] + 0.587 * light->getColor()[1] + 0.144 * light->getColor()[2];
		vec3f shadowAtt = light->shadowAttenuation(targetPt);
		vec3f atten = light->distanceAttenuation(targetPt) *shadowAtt;
		vec3f Li = light->getDirection(targetPt);
		vec3f N = i.N;
		vec3f V = -r.getDirection();
		vec3f R = 2 * N.dot(Li) * N - Li;
		vec3f diff = max(0, (N.dot(Li))) * kd;
		vec3f specular = ks * pow(max(0,V.dot(R)), shininess * 128);
		I += atten.multEach(light->getColor()).multEach(diff + specular);
	}
	I.clamp();
	return I;

}
