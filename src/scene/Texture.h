#pragma once
#include "../vecmath/vecmath.h"

class Texture {
public:
	Texture();
	unsigned char* image = nullptr;
	void loadFile(const char* _fileName);
	vec3f getPixelColor(double _u, double _v);

	int width;
	int height;
};
