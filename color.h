#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vector.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixels){
	auto r = pixels.x();
	auto g = pixels.y();
	auto b = pixels.z();
	
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256*intensity.clamp(r));
	int gbyte = int(256*intensity.clamp(g));
	int bbyte = int(256*intensity.clamp(b));
	
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';

}

#endif
