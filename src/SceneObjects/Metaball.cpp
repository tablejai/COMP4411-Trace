#include "Metaball.h"
//
using namespace std;
Metaball::Metaball(Scene* scene, Material* mat) :MaterialSceneObject(scene, mat)
{
	mballs.push_back({ {0.0,1.0,0},2 });

	mballs.push_back({ {0.0,0.0,1.0},2 });
	mballs.push_back({ {1.0,0.0,0},2});


	
}


Metaball::~Metaball()
{
}
bool compareMetaBall(const Intersect& a, const Intersect& b){

		return a.t < b.t;
	
}
bool Metaball::intersectLocal(const ray& r, isect& i) const
{
	std::vector<Intersect> intersects;
	for (auto mball : mballs)
	{
		
		pair<double, double> isecInfo;
		bool isIntersect = intersectSphere(mball, r, isecInfo); 
		if (isIntersect)
		{
			intersects.push_back({ true,mball,isecInfo.first });
			intersects.push_back({ true,mball,isecInfo.second });

		}
	}
	sort(intersects.begin(), intersects.end(), compareMetaBall);
	int count = 0;
	for (auto&& intersect : intersects) {
		count++;
		if (count == intersects.size())break;
		int stepNum = 90;
		double tchange = (intersects[count].t - intersect.t)/ double(stepNum);
		vec3f norm ;
		double t = intersect.t;
		if (t < RAY_EPSILON)continue;
		for (int m = 0;m < stepNum;m++) {
			if (isPlane(mballs,r,norm,t)) {
				i.obj = this;
				i.N = norm;
				i.t = t;
				return true;
			}
			t += tchange;
		}
		
	}




	return false;
}





bool Metaball::isPlane(const vector<MBall>& mballs,ray r,vec3f&N, double t) const {
	if (getDensity(mballs,r.at(t)) > threshold - RAY_EPSILON) {
			double x = r.at(t)[0];
			double y = r.at(t)[1];
			double z = r.at(t)[2];
			double dx, dy, dz = 0.01;
			#define F(x,y,z) getDensity(mballs,{x,y,z})
			N = vec3f{F(x + dx, y, z) - F(x, y, z), F(x, y + dy, z) - F(x, y, z), F(x, y, z + dz) - F(x, y, z)};
			return true;
	};
	return false;

}



double densityFunction(MBall b,vec3f pos) {

	return pow((1 - pow(b.pos.distance(pos), 2)), 3)*b.density;
}

double Metaball::getDensity(const vector<MBall>& mballs, vec3f pos) const {
	
	double sumdense = 0;
	for (auto& b : mballs) {
	
		sumdense += densityFunction(b,pos);
	
	}
	return sumdense;

}

bool Metaball::intersectSphere(const MBall& ball, const ray& ray, std::pair<double, double>& isecInfo) const
{
	vec3f ballPosition = ball.pos;
	vec3f d = ray.getDirection();
	vec3f rStartPos = ray.getPosition();
	double radius = ball.density;
	double b = 2 * d.dot(rStartPos - ballPosition);
	double a = d.length_squared();

	double c = rStartPos.dot(rStartPos) - 2 * rStartPos.dot(ballPosition) + ballPosition.dot(ballPosition) -pow(radius, radius);
	double dis = b * b - 4 * a * c;
	if (dis < 0) {
		return false;
	}

	dis = sqrt(dis);
	double t2 = -b + dis;
	t2 /= 2 * a;

	double t1 = -b - dis;
	t1 /= 2 * a;

	if (t1 < t2) {
		isecInfo.first = t1;
		isecInfo.second = t2;
	}
	else {
		isecInfo.first = t2;
		isecInfo.second = t1;
	}

	return true;
}




