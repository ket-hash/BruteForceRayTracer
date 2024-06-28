#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable{
	public:
		sphere(const point3& centre, float radius) : centre(centre), radius(fmax(0, radius)) {}

		bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

			vec3 oc = centre - r.origin();
			auto a = r.direction().length_squared();
			auto h = dot(r.direction(), oc);
			auto c = oc.length_squared() - radius*radius;
			auto discriminant = h*h - a*c;

			if (discriminant < 0)
				return false;
			
			auto sqrt_d = sqrt(discriminant);

			auto root = (h - sqrt_d) / a;
			if (!ray_t.surrounds(root)) {
				root = (h + sqrt_d) / a;
				if (!ray_t.surrounds(root))
					return false;
			}
			
			rec.t = root;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - centre) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat = mat;
			return true;
		}
	private:
		point3 centre;
		float radius;
		shared_ptr<material> mat;
	
};

#endif
