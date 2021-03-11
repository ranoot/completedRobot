#include <iostream>

using std::cout;
using std::endl;

struct HSB {
	double hue;
	double saturation;
	double brightness;
};

HSB RGBtoHSB(double red, double green, double blue)
{
	double min, max, chroma;
	HSB out;

	min = red < green ? red : green;
	min = min < blue ? min : blue;

	max = red > green ? red : green;
	max = max > blue ? max : blue;

	chroma = max - min;

	out.brightness = max;
	out.saturation = out.brightness == 0 ? 0 : chroma/max;

	if (chroma == 0) {
		out.hue = 0;
	} else if (max == red) {
		out.hue = 60 * ((green - blue)/chroma);
	} else if (max == green) {
		out.hue = 60 * (2 + (blue-red)/chroma);
	} else if (max == blue) {
		out.hue = 60 * (4 + (red - green)/chroma);
	}

	return out;
}

int main()
{
    auto hsb = RGBtoHSB(244, 129, 48);
    cout << hsb.hue << endl;
    cout << hsb.saturation << endl;
    cout << hsb.brightness << endl;
}