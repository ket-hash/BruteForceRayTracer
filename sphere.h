#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable{
	public:

		sphere(const point3& centre, float radius, shared_ptr<material> mat) : centre1(centre), radius(fmax(0, radius)), mat(mat), is_moving(false) {
			auto rvec = vec3(radius, radius, radius);
			bbox = aabb(centre1 - rvec, centre + rvec);
		}

		sphere(const point3& centre, const point3& centre2, float radius, shared_ptr<material> mat): centre1(centre), radius(fmax(0, radius)), mat(mat), is_moving(true) {
			centre_vec = centre2 - centre1;
			auto rvec = vec3(radius, radius, radius);
			aabb box0(centre1 - rvec, centre1 + rvec);
			aabb box1(centre2 - rvec, centre2 + rvec);
			bbox = aabb(box0, box1);
		}

		bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
			point3 centre = is_moving ? sphere_centre(r.time()) : centre1;
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
		
		aabb bounding_box() const override {
			return bbox;
		}

	private:
		point3 centre1;
		float radius;
		shared_ptr<material> mat;
		bool is_moving;
		vec3 centre_vec;
		aabb bbox;

		point3 sphere_centre(float time) const {
			return centre1 + time*centre_vec;
		}
	
};

#endif
