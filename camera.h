#ifndef CAMERA_H
#define CAMERA_H

#include "ppmrenderer.h"
#include "hittable.h"

class camera {
	public:
		float aspect_ratio = 1.0;
		int img_width = 100;
		int samples_per_pixel = 10;
		int max_depth = 10;

		void render(const hittable& world){
			initialize();
			std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
			for (int j = 0; j < img_height; j++){
				std::clog << "\r Lines Remaining: " << (img_height - j) << ' ' << std::flush;
				for (int i = 0; i < img_width; i++){
					color pixel_color(0,0,0);
					for (int sample = 0; sample < samples_per_pixel; sample++){
						ray r = get_ray(i, j);
						pixel_color += ray_color(r, world, max_depth);
					}
					write_color(std::cout, pixel_samples_scale*pixel_color);
				}
			}
			std::clog << "\r Done Rendering.     \n";
		}
	
	private:
		int img_height;
		point3 centre;
		point3 pixel_loc_00;
		vec3 del_u;
		vec3 del_v;
		float pixel_samples_scale;

		void initialize(){
			img_height = int(img_width/aspect_ratio);
			img_height = (img_height < 1) ? 1 : img_height;
			//camera

			auto focal_length = 1.0;
			auto viewport_height = 2.0;
			auto viewport_width = viewport_height * (double(img_width)/img_height);
			centre = point3(0,0,0);
			pixel_samples_scale = 1.0 / samples_per_pixel;

			//V_u and V_v
			auto V_u = vec3(viewport_width, 0, 0);
			auto V_v = vec3(0, -viewport_height, 0);
	
			//del_u and del_v
			del_u = V_u/img_width;
			del_v = V_v/img_height;

			//Q and P_00
			auto viewport_upper_left = centre - vec3(0,0,focal_length) - V_u/2 - V_v/2;
			pixel_loc_00 = viewport_upper_left + 0.5*(del_u + del_v);
		}
		
		ray get_ray(int i, int j) const {
			auto offset = sample_square();
			auto pixel_sample = pixel_loc_00 + ((i + offset.x()) * del_u)
				+ ((j + offset.y())*del_v);
			auto ray_origin = centre;
			auto ray_direction = pixel_sample - centre;
			return ray(ray_origin, ray_direction);
		}

		vec3 sample_square() const {
			return vec3(random_float() - 0.5, random_float() - 0.5, 0);
		}

		color ray_color(const ray& r, const hittable& world, int depth){
			if (depth < 0)
				return color(0,0,0);
			hit_record rec;

			if(world.hit(r, interval(1e-3, infinity), rec)){
				vec3 direction = rec.normal + random_unit_vector();
				return 0.7*ray_color(ray(rec.p, direction), world, depth - 1);
			}

			vec3 uv = unit_vector(r.direction());
			auto a = 0.5*(uv.y() + 1.0);
			return (1-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
		}


};

#endif
