#include "ray.h"
#include "material.h"
#include "light.h"
#define max(a,b) ((a>b) ? a:b)
#define M_PI 3.14159265358979323846
// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::textureMapping(const ray& r, const isect& i) const
{
	vec3f globalP(0, 0, 1);
	vec3f globalE(1, 0, 0);
	vec3f globalN = i.N.normalize();
	double eDotN = globalE.dot(globalN);
	double pDotN = globalP.dot(globalN);
	double angle = acosf(eDotN / sqrtf(1 - pDotN * pDotN));
	double u = angle / 2 / M_PI;
	if (globalP.cross(globalE).dot(globalN) <= 0) {
		u = 1 - u;
	}
	double v = acosf(globalP.dot(globalN)) / M_PI;
	return isMarble? marble.getPixelColor(u,v) : diffuseTexture.getPixelColor(u, v);
}
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE
	vec3f I;
	vec3f targetPt = r.at(i.t) ;
	I = ke+ ka.multEach(scene->Ia);
	vec3f diff_mult = diffuseTexture.hasTexture || isMarble ? textureMapping(r, i) : kd;
	for (auto iter = scene->beginLights();iter != scene->endLights();iter  = next(iter,1)) {
		Light* light = *iter;

		//double intensity = 0.299 * light->getColor()[0] + 0.587 * light->getColor()[1] + 0.144 * light->getColor()[2];
		vec3f shadowAtt = light->shadowAttenuation(targetPt);
		vec3f atten = light->distanceAttenuation(targetPt) *shadowAtt;
		vec3f Li = light->getDirection(targetPt);
		vec3f N = i.N;
		vec3f V = -r.getDirection();
		vec3f R = 2 * N.dot(Li) * N - Li;
		vec3f diff = max(0, (N.dot(Li))) * diff_mult;
		vec3f specular = ks * pow(max(0,V.dot(R)), shininess * 128);
		I += atten.multEach(light->getColor()).multEach(diff + specular);
	}
	I.clamp();
	return I;

}
