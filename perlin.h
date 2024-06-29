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
			auto i = int(floor(p.x()));
			auto j = int(floor(p.y()));
			auto k = int(floor(p.z()));
			float c[2][2][2];

			for (int di = 0; di < 2; di++){
				for(int dj = 0; dj < 2; dj++){
					for(int dk = 0; dk < 2; dk++){
						c[di][dj][dk] = randfloat[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];

					}
				}
			}
			
			auto u = p.x() - floor(p.x());
			auto v = p.y() - floor(p.y());
			auto w = p.z() - floor(p.z());
		
			return trilinear_interpolation(c, u, v, w);
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

		static float trilinear_interpolation(float c[2][2][2], float u, float v, float w){
			float ac = 0.0;
			for (int i = 0; i < 2; i++){
				for(int j = 0; j < 2; j++){
					for(int k = 0; k < 2; k++){
						ac += (i*u + (1-i)*(1-u)) * (j*v + (1-j)*(1-j)) * (k*w + (1-k)*(1-w)) * c[i][j][k]; 
					}
				}
			}
			return ac;
		}
};

#endif
