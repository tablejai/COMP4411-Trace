#include "Texture.h"

Texture::Texture() {

}

void Texture::loadFile(const char* _fileName) {

}

vec3f Texture::getPixelColor(double _u, double _v) {
	int target = ((int)(height * _v)* width + (int)(_u * width)) * 3;
	double r = image[target], g = image[target + 1], b = image[target + 2];
	r /= 255.0f;
	g /= 255.0f;
	b /= 255.0f;
	return vec3f( r, g, b );
}