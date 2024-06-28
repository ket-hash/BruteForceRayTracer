#ifndef MATERIALS_H
#define MATERIALS_H

#include "ppmrenderer.h"

class hit_record;

class material {
	public:
		virtual ~material() = default;
		
		virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attentuation, ray&scattered) const {
			return false;
		}

};

class lambertian: public material {
	public:
		lambertian(const color& albedo): albedo(albedo) {}
		bool scatter(const ray& ray_in, const hit_record& rec, color& attentuation, ray&scattered) const override {
			float R = random_float();
			if (R <= 0.2){
				return false;
			}
			auto scatter_direction = rec.normal + random_unit_vector();
			if (scatter_direction.near_zero())
				scatter_direction = rec.normal;
			scattered = ray(rec.p, scatter_direction);
			attentuation = albedo/(0.8);
			return true;
		}


	private:
		color albedo;
};

class metal: public material {
	public:
		metal(const color& albedo, float fuzz): albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
	
		bool scatter(const ray& ray_in, const hit_record& rec, color& attentuation, ray& scattered) const override {
			auto reflected = reflect(ray_in.direction(), rec.normal);
			reflected = unit_vector(reflected) + (fuzz*random_unit_vector());
			scattered = ray(rec.p, reflected);
			attentuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0.0);
		}

	private:
		color albedo;
		float fuzz;

};

class dielectric: public material {
	public:
		dielectric(float refractive_index) : refractive_index(refractive_index) {}
		
		bool scatter(const ray& ray_in, const hit_record& rec, color& attentuation, ray& scattered) const override {
			attentuation = color(1.0,1.0,1.0);
			float ri = rec.front_face ? (1.0/refractive_index) : refractive_index;
			
			vec3 unit_direction = unit_vector(ray_in.direction());
			vec3 direction;

			float cos_theta = fmin(dot(rec.normal, -unit_direction), 1.0);
			float sin_theta = sqrt(1-cos_theta*cos_theta);
			bool cannot_refract = ri * sin_theta > 1.0;
			if (cannot_refract || reflectance(cos_theta, ri) > random_float()) {
				direction = reflect(unit_direction, rec.normal);	
			} else {
				direction = refract(unit_direction, rec.normal, ri);
			}
			scattered = ray(rec.p, direction);
			return true;
		}

	private:
		float refractive_index;
		static float reflectance(float cosine, float refractive_index) {
			auto r0 = (1 - refractive_index) / (1 + refractive_index);
			r0 = r0*r0;
			return r0 + (1-r0)*pow((1-cosine), 5);
		}
};

#endif
