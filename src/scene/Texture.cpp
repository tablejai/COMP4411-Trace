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

Marble::Marble() {


}

vec3f Marble::getPixelColor(double _u, double _v) const {
	double x = (int)(_u * noiseWidth);
	double y = (int)(_v * noiseHeight);
	double output = marble_color(x, y);
	cout << output << endl;
	return { output, output, output };
}

double Marble::smooth_noise(double x, double y) const {
	double fractX = x - int(x);
	double fractY = y - int(y);

	//wrap around
	int x1 = (int(x) + noiseWidth) % noiseWidth;
	int y1 = (int(y) + noiseHeight) % noiseHeight;

	//neighbor values
	int x2 = (x1 + noiseWidth - 1) % noiseWidth;
	int y2 = (y1 + noiseHeight - 1) % noiseHeight;

	//smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX * fractY * noise[y1][x1];
	value += (1 - fractX) * fractY * noise[y1][x2];
	value += fractX * (1 - fractY) * noise[y2][x1];
	value += (1 - fractX) * (1 - fractY) * noise[y2][x2];

	return value;
}

void Marble::generate_noise() {
	for (int y = 0; y < noiseHeight; y++)
		for (int x = 0; x < noiseWidth; x++)
			noise[y][x] = (rand() % 32768) / 32768.0;;
}

double Marble::turbulance(double x, double y, double size) const {
	double value = 0.0, initialSize = size;
	while (size >= 1)
	{
		value += smooth_noise(x / size, y / size) * size;
		size /= 2.0;
	}
	return(value / initialSize / 2.0);
}

double Marble::marble_color(double x, double y) const {
	double xyValue = x * xPeriod / noiseWidth + y * yPeriod / noiseHeight + turbPower * turbulance(x, y, turbSize) / 256.0;
	return fabs(sin(xyValue * 3.14159));
}

