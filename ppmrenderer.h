#ifndef PPMRENDERER_H
#define PPMRENDERER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::fabs;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degrees_to_radians(double degrees){
	return pi*degrees / 180.0;
}

inline float random_float(){
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;

	return distribution(generator);
}

inline float random_float(float min, float max){
	return min + (max - min)*random_float();
}

inline int random_int(int min, int max){
	return int(random_float(min, max+1));
}

#include "interval.h"
#include "color.h"
#include "rays.h"
#include "vector.h"
#include "hittable.h"

#endif
