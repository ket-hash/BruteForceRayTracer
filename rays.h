#ifndef RAYS_H
#define RAYS_H

#include "vector.h"

class ray{
	public:
		ray() {}
		ray(const point3& origin, const vec3& direction): orig(origin), dir(direction) {}

		const point3& origin() const {return orig;}
		const vec3& direction() const {return dir;}

		point3 at(float t) const{
			return t*dir + orig;
			}
	private:
		point3 orig;
		vec3 dir;

};

#endif
