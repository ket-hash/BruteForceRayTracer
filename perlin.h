#ifndef PERLIN_H
#define PERLIN_H

#include "ppmrenderer.h"

class perlin {
	public:
		perlin() {
			randvec = new vec3[point_count];
			for (int i = 0; i < point_count; i++){
				randvec[i] = unit_vector(vec3::random(-1, 1));
			}

			perm_z = perlin_generate_perm();
			perm_y = perlin_generate_perm();
			perm_x = perlin_generate_perm();
		}

		~perlin() {
			delete[] randvec;
			delete[] perm_x;
			delete[] perm_y;
			delete[] perm_z;
		}

		float noise(const point3& p) const {
			auto i = int(floor(p.x()));
			auto j = int(floor(p.y()));
			auto k = int(floor(p.z()));
			vec3 c[2][2][2];

			for (int di = 0; di < 2; di++){
				for(int dj = 0; dj < 2; dj++){
					for(int dk = 0; dk < 2; dk++){
						c[di][dj][dk] = randvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];

					}
				}
			}
			
			auto u = p.x() - floor(p.x());
			auto v = p.y() - floor(p.y());
			auto w = p.z() - floor(p.z());
			
			return perlin_interpolation(c, u, v, w);
		}

		float turb(const point3& p, int depth) const {
			float weight = 1.0;
			auto temp_p = p;
			float acc = 0.0;

			for (int i = 0; i < depth; i++){
				acc += weight * noise(temp_p);
				weight *= 0.5;
				temp_p *= 2.0;
			}

			return fabs(acc);
		}

	private:
		static const int point_count = 256;
		vec3* randvec;
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

		static float perlin_interpolation(vec3 c[2][2][2], float u, float v, float w){
			auto uu = u*u*(3 - 2*u);
			auto vv = v*v*(3 - 2*v);
			auto ww = w*w*(3 - 2*w);

			float ac = 0.0;
			for (int i = 0; i < 2; i++){
				for(int j = 0; j < 2; j++){
					for(int k = 0; k < 2; k++){
						vec3 weight(u-i, v-j, w-k);
						ac += (i*uu + (1-i)*(1-uu)) * (j*vv + (1-j)*(1-vv)) * (k*ww + (1-k)*(1-ww)) * dot(c[i][j][k], weight); 
					}
				}
			}
			return ac;
		}
};

#endif
