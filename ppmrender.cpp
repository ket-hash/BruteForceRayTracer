#include "vector.h"
#include "color.h"
#include "rays.h"

#include <iostream>
#include <cmath>

using namespace std;


double hit_sphere(const point3& centre, float radius, const ray& r){
	vec3 oc = centre - r.origin();
	auto a = r.direction().length_squared();
	auto h = dot(r.direction(), oc);
	auto c = oc.length_squared() - radius*radius;
	auto discriminant = h*h - a*c;
	if (discriminant >= 0)
		return (h - sqrt(discriminant))/(a);
	else
		return -1.0;
	
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main(){
	auto aspect_ratio = 16.0/9.0;
	int img_width = 400;
	int img_height = int(img_width/aspect_ratio);
	img_height = (img_height < 1) ? 1 : img_height;
	
	//camera

	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(img_width)/img_height);
	auto camera_centre = point3(0,0,0);
	
	//V_u and V_v
	auto V_u = vec3(viewport_width, 0, 0);
	auto V_v = vec3(0, -viewport_height, 0);
	
	//del_u and del_v
	auto del_u = V_u/img_width;
	auto del_v = V_v/img_height;

	//Q and P_00
	auto viewport_upper_left = camera_centre - vec3(0,0,focal_length) - V_u/2 - V_v/2;
	auto pixel_loc_00 = viewport_upper_left + 0.5*(del_u + del_v);

	cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
	for (int j = 0; j < img_height; j++){
		clog << "\r Lines Remaining: " << (img_height - j) << ' ' << flush;
		for (int i = 0; i < img_width; i++){
			auto pixel_centre = pixel_loc_00 + (i*del_u) + (j*del_v);
			auto dir = pixel_centre - camera_centre;
			ray r = ray(camera_centre, dir);
			color pixel = ray_color(r);
			write_color(cout, pixel);
		}
	}
	clog << "\r Done Rendering.     \n";
	return 0;
}
