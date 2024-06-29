#ifndef PERLIN_H
#define PERLIN_H

#include "ppmrenderer.h"

class perlin {
	public:
		perlin() {
			randfloat = new float[point_count];
			for (int i = 0; i < point_count; i++){
				randfloat[i] = random_float();
			}

			perm_z = perlin_generate_perm();
			perm_y = perlin_generate_perm();
			perm_x = perlin_generate_perm();
		}

		~perlin() {
			delete[] randfloat;
			delete[] perm_x;
			delete[] perm_y;
			delete[] perm_z;
		}

		float noise(const point3& p) const {
			auto i = int(4*p.x()) & 255;
			auto j = int(4*p.y()) & 255;
			auto k = int(4*p.z()) & 255;

			return randfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
		}

	private:
		static const int point_count = 256;
		float* randfloat;
		int* perm_x;
		int* perm_y;
		int* perm_z;

		static int* perlin_generate_perm() {
			auto p = new int[point_count];
			for (int i = 0; i < point_count; i++){
				p[i] = i;
			}

			permute(p, point_count);

			return p;
		}

		static void permute(int* p, int point_count){
			for(int i = point_count; i > 0; i--){
				int target = random_int(0,i);
				int tmp = p[i];
				p[i] = p[target];
				p[target] = tmp;
			}
		}
};

#endif
