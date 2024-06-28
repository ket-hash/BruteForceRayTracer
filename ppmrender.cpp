#include "ppmrenderer.h"

#include "materials.h"
#include "camera.h"
#include "hittable_list.h"
#include "hittable.h"
#include "sphere.h"

using namespace std;

int main(){
	hittable_list world;

	auto R = cos(pi/4);

    	auto material_left  = make_shared<lambertian>(color(0,0,1));
    	auto material_right = make_shared<metal>(color(0.5,0.5,0.8), 0.8);

    	world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    	world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));


	camera cam;

	cam.aspect_ratio = 16.0/9.0;
	cam.img_width = 400;
	cam.samples_per_pixel = 100;
	cam.vfov = 90;
	cam.render(world);
	return 0;
}
