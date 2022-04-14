#include "Texture.h"
#include <iostream>
using namespace std;

Texture::Texture() {

}

void Texture::loadFile(char* _fileName) {
	image = readBMP(_fileName, width, height);
	if (image == NULL) {
		hasTexture = false;
		cout << "Failed in loading the image" << endl;
	}
	else
	{
		hasTexture = true;
		cout << "Successfully loaded the image" << endl;
		cout << "Loaded Image" << " Width: " << width << " Height: " << height << endl;
	}
}

vec3f Texture::getPixelColor(double _u, double _v) const {
	int target = ((int)(height * _v)* width + (int)(_u * width)) * 3;
	double r = image[target], g = image[target + 1], b = image[target + 2];
	r /= 255.0f;
	g /= 255.0f;
	b /= 255.0f;
	return vec3f( r, g, b );
}