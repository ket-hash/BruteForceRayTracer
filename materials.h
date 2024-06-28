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
	
		bool scatter(const ray& ray_in, const hit_record& rec, color& attentuation, ray&scattered) const override {
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
#endif
