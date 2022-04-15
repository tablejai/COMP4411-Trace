#pragma once
#include "../vecmath/vecmath.h"
#include "../fileio/bitmap.h"

class ray;
class isect;

#define noiseWidth 128
#define noiseHeight 128

class Texture {
public:
	Texture();
	unsigned char* image = nullptr;
	void loadFile(char* _fileName);
	virtual vec3f getPixelColor(double _u, double _v) const;

	bool hasTexture = false;
	int width;
	int height;
};

class Marble : Texture {
public:
	Marble();
	vec3f getPixelColor(double _u, double _v) const;
	double marble_color(double x, double y) const;
	double smooth_noise(double x, double y) const;
	double turbulance(double x, double y, double size)const ;
	void generate_noise();

	double noise[noiseHeight][noiseWidth];

	double xPeriod = 5.0;
	double yPeriod = 10.0;
	double turbPower = 5.0;
	double turbSize = 8.0;


};
