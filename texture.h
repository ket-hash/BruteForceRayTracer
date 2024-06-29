#ifndef TEXTURE_H
#define TEXTURE_H

#include "ppmrenderer.h"
#include "image_util.h"

class texture {
	public:
		virtual ~texture() = default;
		virtual color value(float u, float v, const point3& p) const = 0;
};

class solid_color: public texture {
	public:
		solid_color(const color& albedo): albedo(albedo) {};

		solid_color(float red, float green, float blue): solid_color(color(red, green, blue)) {};

		color value(float u, float v, const point3& p) const override {
			return albedo;
		}

	private:
		color albedo;
};

class checker_texture: public texture {
	public:
		checker_texture(float scale, shared_ptr<texture> even, shared_ptr<texture> odd) : inv_scale(1.0/scale), even(even), odd(odd) {}
		checker_texture(float scale, const color& c1, const color& c2) : inv_scale(1.0/scale), even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

		color value(float u, float v, const point3& p) const override {
			auto xInt = int(std::floor(inv_scale*p.x()));
			auto yInt = int(std::floor(inv_scale*p.y()));
			auto zInt = int(std::floor(inv_scale*p.z()));

			bool is_even = (xInt + yInt + zInt) % 2 == 0;

			return is_even ? even->value(u,v,p) : odd->value(u,v,p);
	}
	private:
		float inv_scale;
		shared_ptr<texture> even;
		shared_ptr<texture> odd;
};

class image_texture : public texture{
	public:
		image_texture(const char* filename) : img(filename) {};

		color value(float u, float v, const point3& p) const override {
			if(img.height() <= 0) return color(0,1,1);

			u = interval(0,1).clamp(u);
			v = 1.0 - interval(0,1).clamp(v);

			auto i = int(u * img.width());
			auto j = int(v * img.height());

			auto pixel = img.pixel_data(i,j);

			auto color_scale = 1.0 / 255.0;
			return color(color_scale*pixel[0],color_scale*pixel[1],color_scale*pixel[2]);
		}
	private:
		rtw_image img;

};

#endif
