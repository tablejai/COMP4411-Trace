#pragma once
#include "../vecmath/vecmath.h"
#include "../fileio/bitmap.h"

class ray;
class isect;

class Texture {
public:
	Texture();
	unsigned char* image = nullptr;
	void loadFile(char* _fileName);
	vec3f getPixelColor(double _u, double _v) const;

	bool hasTexture = false;
	int width;
	int height;
};
