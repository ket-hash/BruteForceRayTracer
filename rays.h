#ifndef RAYS_H
#define RAYS_H

#include "vector.h"

class ray{
	public:
		ray() {}
		ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
		ray(const point3& origin, const vec3& direction, float time): orig(origin), dir(direction), tm(time) {}

		const point3& origin() const {return orig;}
		const vec3& direction() const {return dir;}
		const float time() const {return tm;}


		point3 at(float t) const{
			return t*dir + orig;
			}
	private:
		point3 orig;
		vec3 dir;
		float tm;

};

#endif
