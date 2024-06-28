#ifndef CAMERA_H
#define CAMERA_H

#include "ppmrenderer.h"
#include "hittable.h"
#include "materials.h"
class camera {
	public:
		float aspect_ratio = 1.0;
		int img_width = 100;
		int samples_per_pixel = 10;
		int max_depth = 10;
		
		float vfov = 90;
		
		point3 lookfrom = point3(0,0,0);
		point3 lookat = point3(0,0,-1);
		vec3 vup = vec3(0,1,0);
		
		float defocus_angle = 0;
		float focus_dist = 10;

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
		vec3 u,v,w;
		vec3 defocus_disk_u;
		vec3 defocus_disk_v;

		void initialize(){
			img_height = int(img_width/aspect_ratio);
			img_height = (img_height < 1) ? 1 : img_height;
			//camera

			auto theta = degrees_to_radians(vfov);
			auto h = tan(theta/2);
			auto viewport_height = 2.0*h*focus_dist;
			auto viewport_width = viewport_height * (double(img_width)/img_height);
			centre = lookfrom;
			pixel_samples_scale = 1.0 / samples_per_pixel;

			w = unit_vector(lookfrom - lookat);
			u = unit_vector(cross(vup, w));
			v = cross(w,u);

			//V_u and V_v
			auto V_u = viewport_width * u;
			auto V_v = viewport_height * -v;
	
			//del_u and del_v
			del_u = V_u/img_width;
			del_v = V_v/img_height;

			//Q and P_00
			auto viewport_upper_left = centre - (focus_dist * w) - V_u/2 - V_v/2;
			pixel_loc_00 = viewport_upper_left + 0.5*(del_u + del_v);

			auto defocus_radius  = focus_dist * tan(degrees_to_radians(defocus_angle/2));
			defocus_disk_u = u * defocus_radius;
			defocus_disk_v = v * defocus_radius;
		}
		
		ray get_ray(int i, int j) const {
			auto offset = sample_square();
			auto pixel_sample = pixel_loc_00 + ((i + offset.x()) * del_u) + ((j + offset.y())*del_v);
			auto ray_origin = (defocus_angle <= 0) ? centre : defocus_disk_sample();
			auto ray_direction = pixel_sample - ray_origin;
			return ray(ray_origin, ray_direction);
		}

		vec3 sample_square() const {
			return vec3(random_float() - 0.5, random_float() - 0.5, 0);
		}

		point3 defocus_disk_sample() const {
			auto p = random_on_unit_disk();
			return centre + (p[0] * defocus_disk_u) + (p[1]*defocus_disk_v);
		}

		color ray_color(const ray& r, const hittable& world, int depth){
			if (depth < 0)
				return color(0,0,0);
			hit_record rec;

			if (world.hit(r, interval(0.001, infinity), rec)) {
            			ray scattered;
            			color attenuation;
            			if (rec.mat->scatter(r, rec, attenuation, scattered))
                			return attenuation * ray_color(scattered, world, depth-1);
            			return color(0,0,0);
        			}

			vec3 uv = unit_vector(r.direction());
			auto a = 0.5*(uv.y() + 1.0);
			return (1-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
		}


};

#endif
