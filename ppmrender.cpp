#include "ppmrenderer.h"

#include "materials.h"
#include "camera.h"
#include "hittable_list.h"
#include "hittable.h"
#include "sphere.h"

using namespace std;

int main(){
	hittable_list world;
	
	auto ground_material = make_shared<lambertian>(color(0.5,0.5,0.5));
	world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b <11; b++){
			auto choose_mat = random_float();
			point3 centre(a + 0.9*random_float(), 0.2, b + 0.9*random_float());

			if((centre - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8){
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto centre2 = centre + vec3(0, random_float(0, 0.5), 0);
					world.add(make_shared<sphere>(centre, centre2, 0.2, sphere_material));
				} else if (choose_mat > 0.9) {
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_float(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(centre, 0.2, sphere_material));
				} else {
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(centre, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0,1,0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4,0.2,0.1));
	world.add(make_shared<sphere>(point3(-4,1,0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7,0.6,0.5), 0.5);
	world.add(make_shared<sphere>(point3(4,1,0), 1.0, material3));

	camera cam;

	cam.aspect_ratio = 16.0/9.0;
	cam.img_width = 400;
	cam.samples_per_pixel = 10;
	
	cam.vfov = 20;
	cam.lookfrom = point3(13,2,3);
	cam.lookat = point3(0,0,0);
	cam.vup = vec3(0,1,0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	cam.render(world);
	return 0;
}
