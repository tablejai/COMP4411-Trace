#include "Quadric.h"
//
using namespace std;
Quadric::Quadric(Scene* scene, Material* mat,string name) :MaterialSceneObject(scene, mat)
{

	type = name;
}


Quadric::~Quadric()
{
}





bool Quadric::intersectLocal(const ray& r, isect& i) const
{
	i.obj = this;
	#define	F(x, y, z)  (A*pow(x,2) + B*pow(y,2) + C*pow(z,2) + D*x*y + E*x*z + F*y*z + G*x + H*y + I*z + J)
	
	//#define partDVector(x,y,z)  {F(x+0.0001,y,z) -F(x,y,z),F(x,y+0.0001,z) -F(x,y,z),F(x,y,z+0.0001) -F(x,y,z)}
#define partDVector(x,y,z) {2*A*x+D*y+E*z+G,2*B*y+D*x+F*z+H,2*C*z+E*x+F*y+I}

	double A = 0;
	double B = 0;
	double C = 0;
	double D = 0;
	double E = 0;
	double F = 0;
	double G = 0;
	double H = 0;
	double I = 0;
	double J = 0;
	if (type == "paraboloid") {
		B = 1;
		C = 1;
		G = -1;
	}
	else if(type=="hyperbolic") {
		A = -1;
		B = 1;
		C = 1;
		J = -0.3;
		
	//	x ^ 2 - 2ax + a ^ 2 - y ^ 2 + 2by - b ^ 2 + z ^ 2 - 2cz + c ^ 2 - 1
	
	}
	else if (type == "cone") {
		A = -1;
		B - 1;
		C = 1;
	}
	else if (type == "plane") {
		G = 1;
	}
	else if (type == "cylinder") {
		B = 1;
		C = 1;
		J = -1;
	}
	vec3f dir = r.getDirection().normalize();
	double xd = dir[0];
	double yd = dir[1];
	double zd = dir[2];
	vec3f pos = r.getPosition();
	double x0 = pos[0];
	double y0 = pos[1];
	double z0 = pos[2];
	double	Aq = A * pow(xd, 2) + B * pow(yd, 2) + C * zd * zd + D * xd * yd + E * xd * zd + F * yd * zd;

	double	Bq = 2 * A * x0 * xd + 2 * B * y0 * yd + 2 * C*z0 * zd + D*(x0 * yd + y0 * xd) + E*(x0*zd + z0 * xd) + F*(y0 * zd + yd * z0) + G * xd + H * yd + I*zd;

	double	Cq = A * x0 * x0 + B * y0 * y0 + C * z0 * z0 + D * x0 * y0 + E * x0 * z0 + F * y0 * z0 + G * x0 + H * y0 + I * z0 + J;
	
	//Check Aq = 0 (If Aq = 0 then t = -Cq / Bq)
	vec3f pt;
	vec3f N;
	if (fabs(Aq) <= 0.0000001) {
		i.t = -Cq / Bq;
		 pt = r.at(i.t);
		 N = partDVector(pt[0], pt[1], pt[2]);
		i.N = N;
	}
	else {
		//	If Aq < > 0, then check the discriminant.If(Bq2 - 4AqCq) < 0.0 then there is no intersection
		double discriminant = (Bq * Bq - 4 * Aq * Cq);
		if (discriminant < 0.0) {
			return  false;
		}
		double t0 = (-Bq - powf((Bq * Bq - 4 * Aq * Cq), 0.5)) / (2 * Aq);
		double	t1 = (-Bq + powf((Bq * Bq - 4 * Aq * Cq), 0.5)) / (2 * Aq);
		if (t0 > 0) {
			i.t = t0;
		}
		else {
			i.t = t1;
		}
	
		pt = r.at(i.t);
		N = partDVector(pt[0], pt[1], pt[2]);
	}
	if (i.t < RAY_EPSILON)
		return false;
	i.N = ((N.dot(dir) > 0) ? -N : N);
	return true;
		

		//Compute t0 and if t0 > 0 then done else compute t1
		//Once t is found compute Ri = (xi yi zi)
		//To compute Rn which is the normal at Ri take the partial derivatives of F with respect to x, y, z
		//Rn = [xn yn zn] = [dF / dx dF / dy dF / dz]

		//which gives
		//xn = 2 * A * xi + D * yi + E * zi + G
		//yn = 2 * B * yi + D * xi + F * zi + H
		//z n = 2 * C * zi + E * xi + F * yi + I

		//Rn must be normalized and also we have to find the normal for surface facing the ray.
		//If R* Rd > 0 then reverse Rn.

		//Unit Quadric Shape Definitions(all shapes centered about origin(0, 0, 0) and are of size 1)
}








