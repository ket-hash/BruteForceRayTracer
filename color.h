#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vector.h"

using color = vec3;

inline float lin_to_gamma(float linear) {
	if (linear >= 0.0){
		return sqrt(linear);
	}
	return 0;
}

void write_color(std::ostream& out, const color& pixels){
	auto r = pixels.x();
	auto g = pixels.y();
	auto b = pixels.z();

	r = lin_to_gamma(r);
	g = lin_to_gamma(g);
	b = lin_to_gamma(b);
	
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256*intensity.clamp(r));
	int gbyte = int(256*intensity.clamp(g));
	int bbyte = int(256*intensity.clamp(b));
	
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';

}

#endif
