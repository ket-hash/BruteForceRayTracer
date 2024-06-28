#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vector.h"

class sphere : public hittable{
	public:
		sphere(const point3& centre, float radius) : centre(centre), radius(fmax(0, radius)) {}

		bool hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& rec) const override {

			vec3 oc = centre - r.origin();
			auto a = r.direction().length_squared();
			auto h = dot(r.direction(), oc);
			auto c = oc.length_squared() - radius*radius;
			auto discriminant = h*h - a*c;

			if (discriminant < 0)
				return false;
			
			auto sqrt_d = sqrt(discriminant);

			auto root = (h - sqrt_d) / a;
			if (root <= ray_tmin || ray_tmax <= root) {
				root = (h + sqrt_d) / a;
				if (root <= ray_tmin || ray_tmax <= root)
					return false;
			}
			
			rec.t = root;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - centre) / radius;
		}
	private:
		point3 centre;
		float radius;
	
};

#endif
